#include <fstream>

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/debug-helpers.h"
#include "mono/metadata/mono-config.h"
#include <mono/metadata/object.h>
#include "mono/utils/mono-logger.h"

#include "Core/Engine.hpp"
#include "Core/Platform.hpp"
#include "Scripting/ScriptEngine.hpp"
#include "Scripting/ScriptGlue.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components/ScriptComponent.hpp"

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

    void MonoLog(const char* log_domain, const char* log_level, const char* message, mono_bool fatal, void* userData) {
        Log("Domain : %s\n", log_domain);
        Log("Level  : %s\n", log_level);
        Log("Message: %s\n", message);
        Log("Fatal  : %s\n\n", fatal ? "true" : "false");
    }

    std::string ToString(ManagedType t) {
        switch (t) {
            case ManagedType::None: return "None";
            case ManagedType::Transform: return "Transform";
            case ManagedType::Physics:  return "Physics";
            case ManagedType::Shape: return "Shape";
            case ManagedType::Renderer: return "Renderer";
            case ManagedType::ScriptComponent: return "ScriptComponent";
        }
        return "Unknown";
    }
    
    std::string GetMonoClassFullname(const std::string& nameSpace, const std::string& className) {
        if (nameSpace.size() == 0)
            return className;
        return nameSpace + "." + className;
    }

    ScriptEngine::ScriptEngine(Engine* engine) : m_Engine(engine), m_RootDomain(nullptr), m_EngineAssembly(nullptr), m_GameAssembly(nullptr), m_ObjectClass(nullptr), m_ComponentClass(nullptr), m_BehaviourComponentClass(nullptr), m_EntityClass(nullptr), m_InputClassVTable(nullptr), m_EntityIdField(nullptr), m_ComponentEntitySetter(nullptr) {
        std::memset(&m_InputFields, 0, sizeof(m_InputFields));
    }

    void ScriptEngine::Init() {
        InitMono();

        ScriptGlue::Init(m_Engine, this);
        
        ScriptGlue::Bind();

        GetScriptClasses();
    }

    void ScriptEngine::InitMono() {
        mono_trace_set_level_string("debug");
        mono_trace_set_log_handler(MonoLog, nullptr);

        mono_set_assemblies_path(m_Engine->GetMonoCoreLibsPath().u8string().c_str());

        mono_config_parse(nullptr);

        m_RootDomain = mono_jit_init("PhezuEngineDomain");
        if (m_RootDomain == nullptr)
        {
            Log("Assert Here, Error initializing mono jit\n");
            return;
        }

        std::filesystem::path scriptCoreDllPath = m_Engine->GetScriptCoreDllPath() / "Phezu-ScriptCore.dll";

        m_EngineAssembly = LoadAssembly(scriptCoreDllPath.string());

        std::filesystem::path gameDllPath = m_Engine->GetScriptCoreDllPath() / "Game.dll";

        m_GameAssembly = LoadAssembly(gameDllPath.string());
    }

    void ScriptEngine::ShutdownMono() {
        if (m_RootDomain)
        {
            mono_jit_cleanup(m_RootDomain);
            m_RootDomain = nullptr;
        }
    }

    void ScriptEngine::CreateManagedScripts(Entity* entity) {
        m_Entities.try_emplace(
            entity->GetEntityID(),
            entity->GetEntityID(), m_RootDomain, m_EntityClass
        );
        EntityScriptingContext& entityData = m_Entities.at(entity->GetEntityID());
        entityData.EntityScript.SetUlongField(m_EntityIdField, entity->GetEntityID());
        
        // Create Engine Components
        
        ComponentInstance transform(m_RootDomain, m_EngineComponentClasses[ManagedType::Transform]);
        transform.SetEntityProperty(m_ComponentEntitySetter, entityData.EntityScript.GetMonoGcHandle());
        entityData.EngineComponents.emplace(ManagedType::Transform, std::move(transform));
        
        if (entity->HasDataComponent(ComponentType::Physics)) {
            ComponentInstance physics(m_RootDomain, m_EngineComponentClasses[ManagedType::Physics]);
            physics.SetEntityProperty(m_ComponentEntitySetter, entityData.EntityScript.GetMonoGcHandle());
            entityData.EngineComponents.emplace(ManagedType::Physics, std::move(physics));
        }
        
        if (entity->HasDataComponent(ComponentType::Render)) {
            ComponentInstance renderer(m_RootDomain, m_EngineComponentClasses[ManagedType::Renderer]);
            renderer.SetEntityProperty(m_ComponentEntitySetter, entityData.EntityScript.GetMonoGcHandle());
            entityData.EngineComponents.emplace(ManagedType::Renderer, std::move(renderer));
        }
        
        // Create Script Components
        
        size_t compCount = entity->GetScriptComponentCount();

        for (size_t i = 0; i < compCount; i++) {
            ScriptComponent* comp = entity->GetScriptComponent(i);
            //TODO: Log error if script class not found
            auto scriptClass = m_ScriptClasses[comp->GetScriptClassFullname()];
            entityData.BehaviourComponents.emplace_back(m_RootDomain, scriptClass);

            entityData.BehaviourComponents[i].SetEntityProperty(
                m_ComponentEntitySetter, entityData.EntityScript.GetMonoGcHandle());
        }
    }
    
    void ScriptEngine::InitializeManagedScripts(Entity* entity) {
        EntityScriptingContext& entityData = m_Entities.at(entity->GetEntityID());
        
        for (size_t i = 0; i < entity->GetScriptComponentCount(); i++) {
            if (entityData.BehaviourComponents[i].HasOnCreate())
                entityData.BehaviourComponents[i].InvokeOnCreate();
        }
    }

    void ScriptEngine::OnEntityDestroyed(Entity* entity) {
        auto& entityData = m_Entities.at(entity->GetEntityID());

        for (size_t i = 0; i < entityData.BehaviourComponents.size(); i++) {
            if (entityData.BehaviourComponents[i].HasOnDestroy())
                entityData.BehaviourComponents[i].InvokeOnDestroy();
        }

        m_Entities.erase(entity->GetEntityID());
    }
    
    void ScriptEngine::PreUpdate() {
        mono_field_static_set_value(m_InputClassVTable, m_InputFields.W, (void*)&(m_Engine->GetInput().W));
        mono_field_static_set_value(m_InputClassVTable, m_InputFields.A, (void*)&(m_Engine->GetInput().A));
        mono_field_static_set_value(m_InputClassVTable, m_InputFields.S, (void*)&(m_Engine->GetInput().S));
        mono_field_static_set_value(m_InputClassVTable, m_InputFields.D, (void*)&(m_Engine->GetInput().D));
        mono_field_static_set_value(m_InputClassVTable, m_InputFields.Space, (void*)&(m_Engine->GetInput().Space));
    }

    void ScriptEngine::OnUpdate(float deltaTime) {
        for (auto it = m_Entities.begin(); it != m_Entities.end(); it++) {
            auto& entityData = it->second;

            for (size_t i = 0; i < entityData.BehaviourComponents.size(); i++) {
                if (entityData.BehaviourComponents[i].HasOnUpdate())
                    entityData.BehaviourComponents[i].InvokeOnUpdate(deltaTime);
            }
        }
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
        m_ObjectClass = ScriptClass::TryCreate(m_EngineAssembly, "PhezuEngine", "Object", ScriptClassType::Object);
        m_ComponentClass = ScriptClass::TryCreate(m_EngineAssembly, "PhezuEngine", "Component", ScriptClassType::Component);
        m_BehaviourComponentClass = ScriptClass::TryCreate(m_EngineAssembly, "PhezuEngine", "BehaviourComponent", ScriptClassType::BehaviourComponent);
        m_EntityClass = ScriptClass::TryCreate(m_EngineAssembly, "PhezuEngine", "Entity", ScriptClassType::Entity);

        m_EngineComponentClasses[ManagedType::Transform] = ScriptClass::TryCreate(m_EngineAssembly, "PhezuEngine", "Transform", ScriptClassType::EngineComponent);
        m_EngineComponentClasses[ManagedType::Physics] = ScriptClass::TryCreate(m_EngineAssembly, "PhezuEngine", "Physics", ScriptClassType::EngineComponent);
        m_EngineComponentClasses[ManagedType::Renderer] = ScriptClass::TryCreate(m_EngineAssembly, "PhezuEngine", "Renderer", ScriptClassType::EngineComponent);

        m_EntityIdField = m_EntityClass->GetMonoClassField("ID");
        m_ComponentEntitySetter = m_ComponentClass->GetMonoMethod("SetEntity", 1);
    }
    
    void ScriptEngine::GetScriptClasses() {
        GetEngineClasses();
        GetInputClassAndFields();

        MonoImage* image = mono_assembly_get_image(m_GameAssembly);
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

        for (int32_t i = 0; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

            auto monoClass = ScriptClass::TryCreate(m_GameAssembly, nameSpace, name, ScriptClassType::ScriptComponent);

            if (monoClass == nullptr)
                continue;
            if (!mono_class_is_subclass_of(monoClass->GetMonoClass(), m_BehaviourComponentClass->GetMonoClass(), false))
                continue;

            std::string fullname = GetMonoClassFullname(nameSpace, name);
            m_ScriptClasses.insert(std::make_pair(fullname, monoClass));
        }
    }
    
    void ScriptEngine::GetInputClassAndFields() {
        ScriptClass* inputClass = ScriptClass::TryCreate(m_EngineAssembly, "PhezuEngine", "Input", ScriptClassType::CSharpClass);
        m_InputClassVTable = mono_class_vtable(m_RootDomain, inputClass->GetMonoClass());
        mono_runtime_class_init(m_InputClassVTable);
        
        m_InputFields.W = inputClass->GetMonoClassField("_W");
        m_InputFields.A = inputClass->GetMonoClassField("_A");
        m_InputFields.S = inputClass->GetMonoClassField("_S");
        m_InputFields.D = inputClass->GetMonoClassField("_D");
        m_InputFields.Space = inputClass->GetMonoClassField("_Space");
    }
    
    void ScriptEngine::FirePhysicsCollisionEvent(uint64_t entityA, uint64_t entityB, PhysicsEventType eventType) {
        auto& entityDataA = m_Entities.at(entityA);
        auto& entityDataB = m_Entities.at(entityB);
        
        for (size_t i = 0; i < entityDataA.BehaviourComponents.size(); i++) {
            if (eventType == PhysicsEventType::CollisionEnter)
                entityDataA.BehaviourComponents[i].TryInvokeOnCollisionEnter(entityDataB.EntityScript.GetMonoGcHandle());
            else if (eventType == PhysicsEventType::CollisionStay)
                entityDataA.BehaviourComponents[i].TryInvokeOnCollisionStay(entityDataB.EntityScript.GetMonoGcHandle());
            else if (eventType == PhysicsEventType::CollisionExit)
                entityDataA.BehaviourComponents[i].TryInvokeOnCollisionExit(entityDataB.EntityScript.GetMonoGcHandle());
        }
        
        for (size_t i = 0; i < entityDataB.BehaviourComponents.size(); i++) {
            if (eventType == PhysicsEventType::CollisionEnter)
                entityDataB.BehaviourComponents[i].TryInvokeOnCollisionEnter(entityDataA.EntityScript.GetMonoGcHandle());
            else if (eventType == PhysicsEventType::CollisionStay)
                entityDataB.BehaviourComponents[i].TryInvokeOnCollisionStay(entityDataA.EntityScript.GetMonoGcHandle());
            else if (eventType == PhysicsEventType::CollisionExit)
                entityDataB.BehaviourComponents[i].TryInvokeOnCollisionExit(entityDataA.EntityScript.GetMonoGcHandle());
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
        ScriptGlue::Destroy();
        ShutdownMono();
    }
    
    //----Script-Glue-Functions----//

    ScriptInstance* ScriptEngine::GetBehaviourScriptInstance(uint64_t entityID, const std::string& classFullname) {
        if (m_Entities.find(entityID) == m_Entities.end()) {
            Log("Entity not found for behaviour script: %s", classFullname.c_str());
            return nullptr;
        }

        auto& entityData = m_Entities.at(entityID);

        for (size_t i = 0; i < entityData.BehaviourComponents.size(); i++) {
            if (entityData.BehaviourComponents[i].GetFullname() == classFullname)
                return &entityData.BehaviourComponents[i];
        }

        Log("Behaviour Script %s on Entity not found\n", classFullname.c_str());
        
        return nullptr;
    }
    
    ScriptInstance* ScriptEngine::GetEngineComponentInstance(uint64_t entityID, ManagedType componentType) {
        if (m_Entities.find(entityID) == m_Entities.end()) {
            Log("Entity not found for engine component of type %i", static_cast<int>(componentType));
            return nullptr;
        }

        auto& entityData = m_Entities.at(entityID);
        
        if (entityData.EngineComponents.find(componentType) == entityData.EngineComponents.end()) {
            Log("Native Component %s on Entity not found\n", ToString(componentType).c_str());
            return nullptr;
        }
        
        return &entityData.EngineComponents.at(componentType);
    }
    
    void ScriptEngine::RemoveBehaviourScriptInstance(uint64_t entityID, const std::string& classFullname) {
        if (m_Entities.find(entityID) == m_Entities.end()) {
            Log("Entity not found for behaviour script: %s", classFullname.c_str());
            return;
        }

        auto entity = m_Engine->GetSceneManager().FindEntity(entityID);
        auto& entityData = m_Entities.at(entityID);
        
        if (entity == nullptr) {
            //TODO: assert not null
        }
        
        entity->RemoveScriptComponent(classFullname);

        for (size_t i = 0; i < entityData.BehaviourComponents.size(); i++) {
            if (entityData.BehaviourComponents[i].GetFullname() == classFullname) {
                entityData.BehaviourComponents.erase(entityData.BehaviourComponents.begin() + i);
                return;
            }
        }

        Log("Behaviour Script %s on Entity not found\n", classFullname.c_str());
    }
    
    MonoClass* ScriptEngine::GetBehaviourComponentClass() {
        return m_BehaviourComponentClass->GetMonoClass();
    }
    
    uint32_t ScriptEngine::GetEntityScriptInstanceGcHandle(uint64_t entityID) {
        return m_Entities.at(entityID).EntityScript.GetMonoGcHandle();
    }

}
