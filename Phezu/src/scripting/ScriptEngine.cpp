#include "scripting/ScriptEngine.hpp"

#include "Engine.hpp"
#include "Logger.hpp"
#include "scripting/ScriptGlue.hpp"
#include "scripting/ScriptClass.hpp"
#include "scripting/ScriptInstance.hpp"
#include "scripting/EntityInstance.hpp"
#include "scene/Scene.hpp"
#include "scene/Entity.hpp"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/debug-helpers.h"
#include "mono/metadata/mono-config.h"
#include "mono/utils/mono-logger.h"

#include <fstream>

namespace Phezu {

    void MonoLog(const char* log_domain, const char* log_level, const char* message, mono_bool fatal, void* userData) {
        Log("Domain : %s\n", log_domain);
        Log("Level  : %s\n", log_level);
        Log("Message: %s\n", message);
        Log("Fatal  : %s\n\n", fatal ? "true" : "false");
    }

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

    std::string GetMonoClassFullname(const std::string& nameSpace, const std::string& className) {
        if (nameSpace.size() == 0)
            return className;
        return nameSpace + "." + className;
    }

    ScriptEngine::ScriptEngine(Engine* engine) : m_Engine(engine), m_RootDomain(nullptr), 
        m_AppDomain(nullptr), m_CoreAssembly(nullptr), m_ObjectGcHandleGetter(nullptr),
        m_EntityIdField(nullptr), m_BehaviourComponentEntitySetter(nullptr) {}

    void ScriptEngine::Init() {
        InitMono();

        ScriptGlue::Init(m_Engine, this);
        ScriptGlue::Bind();

        std::filesystem::path scriptCoreDllPath = m_Engine->GetScriptCoreDllPath() / "Phezu-ScriptCore.dll";
        
        m_CoreAssembly = LoadAssembly(scriptCoreDllPath.string());

        GetScriptClasses();
    }

    void ScriptEngine::OnEntityCreated(std::shared_ptr<Entity> entity) {
        std::shared_ptr<EntityInstance> entityInstance = 
            std::make_shared<EntityInstance>(entity->GetEntityID(), m_AppDomain, m_EntityClass, m_ObjectGcHandleGetter);
        entityInstance->EntityScript.SetUlongField(m_EntityIdField, entity->GetEntityID());
        m_EntityInstances[entity->GetEntityID()] = entityInstance;

        size_t compCount = entity->GetScriptComponentCount();

        for (size_t i = 0; i < compCount; i++) {
            ScriptComponent* comp = entity->GetScriptComponent(i);
            auto scriptClass = m_ScriptClasses[comp->GetScriptClassFullname()];
            entityInstance->BehaviourScripts.emplace_back(m_AppDomain, scriptClass, m_ObjectGcHandleGetter);

            entityInstance->BehaviourScripts[i].SetGcHandleProperty(
                m_BehaviourComponentEntitySetter, entityInstance->EntityScript.GetMonoGcHandle());
        }

        for (size_t i = 0; i < compCount; i++) {
            entityInstance->BehaviourScripts[i].InvokeOnCreate();
        }
    }

    void ScriptEngine::OnEntityDestroyed(std::shared_ptr<Entity> entity) {
        auto entityInstance = m_EntityInstances[entity->GetEntityID()];

        for (size_t i = 0; i < entityInstance->BehaviourScripts.size(); i++) {
            //Invoke On Destroy
            
            //entityInstance->BehaviourScripts[i].InvokeOnCreate();
        }

        m_EntityInstances.erase(entity->GetEntityID());
    }

    void ScriptEngine::OnUpdate(float deltaTime) {
        for (auto it = m_EntityInstances.begin(); it != m_EntityInstances.end(); it++) {
            auto entityInstance = it->second;

            for (size_t i = 0; i < entityInstance->BehaviourScripts.size(); i++) {
                entityInstance->BehaviourScripts[i].InvokeOnUpdate(deltaTime);
            }
        }
    }

    void ScriptEngine::InitMono() {
#if _WIN32
        m_MonoLogger.Start();
#endif

        mono_trace_set_level_string("debug");
        mono_trace_set_log_handler(MonoLog, nullptr);
        mono_set_assemblies_path(m_Engine->GetMonoCoreLibsPath().c_str());

        m_RootDomain = mono_jit_init("MyScriptRuntime");
        if (m_RootDomain == nullptr)
        {
            Log("Error initializing mono jit\n");
            return;
        }

        m_AppDomain = mono_domain_create_appdomain("MyAppDomain", nullptr);
        mono_domain_set(m_AppDomain, true);
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

    void ScriptEngine::GetScriptClasses() {
        MonoImage* image = mono_assembly_get_image(m_CoreAssembly);
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

        m_ObjectClass = std::make_shared<ScriptClass>(m_CoreAssembly, "PhezuEngine", "Object", ScriptClassType::Object);
        m_EntityClass = std::make_shared<ScriptClass>(m_CoreAssembly, "PhezuEngine", "Entity", ScriptClassType::Entity);
        m_ObjectGcHandleGetter = m_ObjectClass->GetMonoMethod("GetGcHandle", 0);
        m_EntityIdField = m_EntityClass->GetMonoClassField("ID");
        m_BehaviourComponentClass = std::make_shared<ScriptClass>(m_CoreAssembly, "PhezuEngine", "BehaviourComponent", ScriptClassType::BehaviourComponent);
        m_BehaviourComponentEntitySetter = m_BehaviourComponentClass->GetMonoMethod("SetEntity", 1);

        for (int32_t i = 0; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

            auto monoClass = std::make_shared<ScriptClass>(m_CoreAssembly, nameSpace, name, ScriptClassType::ScriptComponent);

            if (monoClass->GetMonoClass() == m_BehaviourComponentClass->GetMonoClass())
                continue;
            if (!mono_class_is_subclass_of(monoClass->GetMonoClass(), m_BehaviourComponentClass->GetMonoClass(), false))
                continue;

            std::string fullname = GetMonoClassFullname(nameSpace, name);
            m_ScriptClasses.insert(std::make_pair(fullname, monoClass));
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
        //m_MonoLogger.Stop();
        ScriptGlue::Destroy();
    }

    ScriptInstance* ScriptEngine::GetScriptInstance(uint64_t entityID, const std::string& classFullname) {
        if (m_EntityInstances.find(entityID) == m_EntityInstances.end()) {
            Log("Entity not found");
            return nullptr;
        }

        auto entityInstance = m_EntityInstances.at(entityID);

        for (size_t i = 0; i < entityInstance->BehaviourScripts.size(); i++) {
            if (entityInstance->BehaviourScripts[i].GetFullname() == classFullname)
                return &entityInstance->BehaviourScripts[i];
        }

        Log("Script component on Entity not found\n");
        return nullptr;
    }

    MonoClass* ScriptEngine::GetBehaviourComponentClass() {
        return m_BehaviourComponentClass->GetMonoClass();
    }

}
