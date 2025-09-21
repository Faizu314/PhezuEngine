#include "scripting/ScriptEngine.hpp"

#include "Engine.hpp"
#include "Logger.hpp"
#include "scripting/ScriptGlue.hpp"
#include "scripting/ScriptClass.hpp"
#include "scripting/ScriptInstance.hpp"
#include "scripting/EntityData.hpp"
#include "scene/Scene.hpp"
#include "scene/Entity.hpp"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/debug-helpers.h"
#include "mono/metadata/mono-config.h"
#include <mono/metadata/object.h>
#include "mono/utils/mono-logger.h"

#include <fstream>

namespace Phezu {

    char* ReadBytes(const std::string& filepath, uint32_t* outSize) {
        std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

        if (!stream)
        {
            Log("Failed to read file at path: %s\n", filepath.c_str());
            return nullptr;
        }

        std::streampos end = stream.tellg();
        stream.seekg(0, std::ios::beg);
        size_t size = end - stream.tellg();

        if (size == 0)
        {
            Log("Trying to read an empty file at path: %s\n", filepath.c_str());
            return nullptr;
        }

        char* buffer = new char[size];
        stream.read((char*)buffer, size);
        stream.close();

        *outSize = static_cast<uint32_t>(size);
        return buffer;
    }

    std::string ToString(NativeType t) {
        switch (t) {
            case NativeType::None:   return "None";
            case NativeType::Transform: return "Transform";
            case NativeType::Physics:  return "Physics";
            case NativeType::Shape: return "Shape";
            case NativeType::Renderer: return "Renderer";
            case NativeType::ScriptComponent: return "ScriptComponent";
        }
        return "Unknown";
    }
    
    std::string GetMonoClassFullname(const std::string& nameSpace, const std::string& className) {
        if (nameSpace.size() == 0)
            return className;
        return nameSpace + "." + className;
    }

    ScriptEngine::ScriptEngine(Engine* engine) : m_Engine(engine), m_EngineDomain(nullptr), 
        m_RuntimeDomain(nullptr), m_CoreAssembly(nullptr),
        m_EntityIdField(nullptr), m_ComponentEntitySetter(nullptr) {}

    void ScriptEngine::Init() {
        InitMono();
        
        m_MonoLogger.Init();

        ScriptGlue::Init(m_Engine, this);
        
        ScriptGlue::Bind();

        std::filesystem::path scriptCoreDllPath = m_Engine->GetScriptCoreDllPath() / "Phezu-ScriptCore.dll";
        
        m_CoreAssembly = LoadAssembly(scriptCoreDllPath.string());

        GetScriptClasses();
    }

    void ScriptEngine::OnEntityCreated(Entity* entity) {
        auto entityID = entity->GetEntityID();
        
        EntityData* entityData =
            new EntityData(entity->GetEntityID(), m_RuntimeDomain, m_EntityClass);
        entityData->EntityScript.SetUlongField(m_EntityIdField, entity->GetEntityID());
        m_EntityDatas[entity->GetEntityID()] = entityData;
        
        // Create Native Components
        
        ComponentInstance transform(m_RuntimeDomain, m_NativeComponentClasses[NativeType::Transform]);
        transform.SetEntityProperty(m_ComponentEntitySetter, entityData->EntityScript.GetMonoGcHandle());
        entityData->NativeComponents.emplace(NativeType::Transform, std::move(transform));
        
        if (entity->GetPhysicsData() != nullptr) {
            ComponentInstance physics(m_RuntimeDomain, m_NativeComponentClasses[NativeType::Physics]);
            physics.SetEntityProperty(m_ComponentEntitySetter, entityData->EntityScript.GetMonoGcHandle());
            entityData->NativeComponents.emplace(NativeType::Physics, std::move(physics));
        }
        
        // Create Script Components
        
        size_t compCount = entity->GetScriptComponentCount();

        for (size_t i = 0; i < compCount; i++) {
            ScriptComponent* comp = entity->GetScriptComponent(i);
            //TODO: Log error if script class not found
            auto scriptClass = m_ScriptClasses[comp->GetScriptClassFullname()];
            entityData->BehaviourScripts.emplace_back(m_RuntimeDomain, scriptClass);

            entityData->BehaviourScripts[i].SetEntityProperty(
                m_ComponentEntitySetter, entityData->EntityScript.GetMonoGcHandle());
        }
        
        for (size_t i = 0; i < compCount; i++) {
            entityData->BehaviourScripts[i].InvokeOnCreate();
        }
    }

    void ScriptEngine::OnEntityDestroyed(Entity* entity) {
        auto entityData = m_EntityDatas[entity->GetEntityID()];

        for (size_t i = 0; i < entityData->BehaviourScripts.size(); i++) {
            //Invoke On Destroy
            
            //entityData->BehaviourScripts[i].InvokeOnCreate();
        }

        m_EntityDatas.erase(entity->GetEntityID());
    }
    
    void ScriptEngine::PreUpdate() {
        bool a = m_Engine->GetInput().A;
        
        mono_field_static_set_value(m_InputClassVTable, m_InputFields.W, (void*)&(m_Engine->GetInput().W));
        mono_field_static_set_value(m_InputClassVTable, m_InputFields.A, (void*)&(m_Engine->GetInput().A));
        mono_field_static_set_value(m_InputClassVTable, m_InputFields.S, (void*)&(m_Engine->GetInput().S));
        mono_field_static_set_value(m_InputClassVTable, m_InputFields.D, (void*)&(m_Engine->GetInput().D));
        mono_field_static_set_value(m_InputClassVTable, m_InputFields.Space, (void*)&(m_Engine->GetInput().Space));
    }

    void ScriptEngine::OnUpdate(float deltaTime) {
        for (auto it = m_EntityDatas.begin(); it != m_EntityDatas.end(); it++) {
            auto entityData = it->second;

            for (size_t i = 0; i < entityData->BehaviourScripts.size(); i++) {
                entityData->BehaviourScripts[i].InvokeOnUpdate(deltaTime);
            }
        }
    }

    void ScriptEngine::InitMono() {
        mono_set_assemblies_path(m_Engine->GetMonoCoreLibsPath().c_str());
        
        mono_config_parse(NULL);
        
        m_EngineDomain = mono_jit_init("PhezuEngineDomain");
        if (m_EngineDomain == nullptr)
        {
            Log("Error initializing mono jit\n");
            return;
        }

        m_RuntimeDomain = mono_domain_create_appdomain("PhezuRuntimeDomain", nullptr);
        mono_domain_set(m_RuntimeDomain, true);
    }

    MonoAssembly* ScriptEngine::LoadAssembly(const std::string& assemblyPath) {
        uint32_t fileSize = 0;
        char* fileData = ReadBytes(assemblyPath, &fileSize);

        MonoImageOpenStatus status;
        MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

        if (status != MONO_IMAGE_OK)
        {
            const char* errorMessage = mono_image_strerror(status);
            Log("Error loading assembly | %s", errorMessage);
            return nullptr;
        }

        MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
        mono_image_close(image);

        delete[] fileData;

        return assembly;
    }
    
    void ScriptEngine::GetEngineClasses() {
        m_ObjectClass = new ScriptClass(m_CoreAssembly, "PhezuEngine", "Object", ScriptClassType::Object);
        m_ComponentClass = new ScriptClass(m_CoreAssembly, "PhezuEngine", "Component", ScriptClassType::Component);
        m_BehaviourComponentClass = new ScriptClass(m_CoreAssembly, "PhezuEngine", "BehaviourComponent", ScriptClassType::BehaviourComponent);
        m_ComponentEntitySetter = m_ComponentClass->GetMonoMethod("SetEntity", 1);
        m_EntityClass = new ScriptClass(m_CoreAssembly, "PhezuEngine", "Entity", ScriptClassType::Entity);
        
        m_NativeComponentClasses[NativeType::Transform] = new ScriptClass(m_CoreAssembly, "PhezuEngine", "Transform", ScriptClassType::NativeComponent);
        m_NativeComponentClasses[NativeType::Physics] = new ScriptClass(m_CoreAssembly, "PhezuEngine", "Physics", ScriptClassType::NativeComponent);
    }
    
    void ScriptEngine::GetScriptClasses() {
        MonoImage* image = mono_assembly_get_image(m_CoreAssembly);
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

        GetEngineClasses();
        
        m_EntityIdField = m_EntityClass->GetMonoClassField("ID");

        for (int32_t i = 0; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

            auto monoClass = new ScriptClass(m_CoreAssembly, nameSpace, name, ScriptClassType::ScriptComponent);

            if (monoClass->GetMonoClass() == m_BehaviourComponentClass->GetMonoClass())
                continue;
            if (!mono_class_is_subclass_of(monoClass->GetMonoClass(), m_BehaviourComponentClass->GetMonoClass(), false))
                continue;

            std::string fullname = GetMonoClassFullname(nameSpace, name);
            m_ScriptClasses.insert(std::make_pair(fullname, monoClass));
        }
        
        GetInputClassAndFields();
    }
    
    void ScriptEngine::GetInputClassAndFields() {
        ScriptClass* inputClass = new ScriptClass(m_CoreAssembly, "PhezuEngine", "Input", ScriptClassType::CSharpClass);
        m_InputClassVTable = mono_class_vtable(m_RuntimeDomain, inputClass->GetMonoClass());
        mono_runtime_class_init(m_InputClassVTable);
        
        m_InputFields.W = inputClass->GetMonoClassField("_W");
        m_InputFields.A = inputClass->GetMonoClassField("_A");
        m_InputFields.S = inputClass->GetMonoClassField("_S");
        m_InputFields.D = inputClass->GetMonoClassField("_D");
        m_InputFields.Space = inputClass->GetMonoClassField("_Space");
    }
    
    void ScriptEngine::FirePhysicsCollisionEvent(uint64_t entityA, uint64_t entityB, PhysicsEventType eventType) {
        auto entityDataA = m_EntityDatas[entityA];
        auto entityDataB = m_EntityDatas[entityB];
        
        for (size_t i = 0; i < entityDataA->BehaviourScripts.size(); i++) {
            if (eventType == PhysicsEventType::CollisionEnter)
                entityDataA->BehaviourScripts[i].TryInvokeOnCollisionEnter(entityDataB->EntityScript.GetMonoGcHandle());
            else if (eventType == PhysicsEventType::CollisionStay)
                entityDataA->BehaviourScripts[i].TryInvokeOnCollisionStay(entityDataB->EntityScript.GetMonoGcHandle());
            else if (eventType == PhysicsEventType::CollisionExit)
                entityDataA->BehaviourScripts[i].TryInvokeOnCollisionExit(entityDataB->EntityScript.GetMonoGcHandle());
        }
        
        for (size_t i = 0; i < entityDataB->BehaviourScripts.size(); i++) {
            if (eventType == PhysicsEventType::CollisionEnter)
                entityDataB->BehaviourScripts[i].TryInvokeOnCollisionEnter(entityDataA->EntityScript.GetMonoGcHandle());
            else if (eventType == PhysicsEventType::CollisionStay)
                entityDataB->BehaviourScripts[i].TryInvokeOnCollisionStay(entityDataA->EntityScript.GetMonoGcHandle());
            else if (eventType == PhysicsEventType::CollisionExit)
                entityDataB->BehaviourScripts[i].TryInvokeOnCollisionExit(entityDataA->EntityScript.GetMonoGcHandle());
        }
    }

    void ScriptEngine::PrintAssemblyClasses(MonoAssembly* assembly) {
        MonoImage* image = mono_assembly_get_image(assembly);
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

        for (int32_t i = 0; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

            Log("%s.%s\n", nameSpace, name);
        }
    }

    void ScriptEngine::Shutdown() {
        m_MonoLogger.Destroy();
        ScriptGlue::Destroy();
    }

    ScriptInstance* ScriptEngine::GetBehaviourScriptInstance(uint64_t entityID, const std::string& classFullname) {
        if (m_EntityDatas.find(entityID) == m_EntityDatas.end()) {
            Log("Entity not found");
            return nullptr;
        }

        auto entityData = m_EntityDatas.at(entityID);

        for (size_t i = 0; i < entityData->BehaviourScripts.size(); i++) {
            if (entityData->BehaviourScripts[i].GetFullname() == classFullname)
                return &entityData->BehaviourScripts[i];
        }

        Log("Behaviour Script %s on Entity not found\n", classFullname.c_str());
        
        return nullptr;
    }
    
    ScriptInstance* ScriptEngine::GetNativeComponentInstance(uint64_t entityID, const NativeType componentType) {
        if (m_EntityDatas.find(entityID) == m_EntityDatas.end()) {
            Log("Entity not found");
            return nullptr;
        }

        auto entityData = m_EntityDatas.at(entityID);
        
        if (entityData->NativeComponents.find(componentType) == entityData->NativeComponents.end()) {
            Log("Native Component %s on Entity not found\n", ToString(componentType).c_str());
            return nullptr;
        }
        
        return &entityData->NativeComponents.at(componentType);
    }

    MonoClass* ScriptEngine::GetBehaviourComponentClass() {
        return m_BehaviourComponentClass->GetMonoClass();
    }

}
