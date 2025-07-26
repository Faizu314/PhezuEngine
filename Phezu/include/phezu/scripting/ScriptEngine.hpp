#pragma once

#include "scripting/MonoLogger.hpp"
#include "scripting/MonoDefs.hpp"

#include <string>
#include <unordered_map>

namespace Phezu {

	class Engine;
	class Scene;
	class Entity;
	class ScriptClass;
	class ScriptComponent;
	class ScriptInstance;
	struct EntityInstance;
    
    struct InputFields {
        MonoClassField *W, *A, *S, *D, *Space;
    };
    
	class ScriptEngine {
	public:
		ScriptEngine(Engine* engine);
	public:
		void Init();
		void OnEntityCreated(std::shared_ptr<Entity> entity);
		void OnEntityDestroyed(std::shared_ptr<Entity> entity);
        void PreUpdate();
		void OnUpdate(float deltaTime);
		void Shutdown();
	public:
		MonoClass* GetBehaviourComponentClass();
		ScriptInstance* GetScriptInstance(uint64_t entityID, const std::string& classFullname);
	private:
		void InitMono();
		MonoAssembly* LoadAssembly(const std::string& assemblyPath);
        void GetInputClassAndFields();
		void GetScriptClasses();
		void PrintAssemblyClasses(MonoAssembly* assembly);
	private:
		Engine* m_Engine;
		MonoLogger m_MonoLogger;
		MonoDomain* m_RootDomain;
		MonoDomain* m_AppDomain;
		MonoAssembly* m_CoreAssembly;
	private:
		std::shared_ptr<ScriptClass> m_ObjectClass;
		std::shared_ptr<ScriptClass> m_EntityClass;
		std::shared_ptr<ScriptClass> m_BehaviourComponentClass;
    private:
        MonoVTable* m_InputClassVTable;
        InputFields m_InputFields;
	private:
		MonoMethod* m_ObjectGcHandleGetter;
		MonoClassField* m_EntityIdField;
		MonoMethod* m_BehaviourComponentEntitySetter;
	private:
		std::unordered_map<std::string, std::shared_ptr<ScriptClass>> m_ScriptClasses;
		std::unordered_map<uint64_t, std::shared_ptr<EntityInstance>> m_EntityInstances;
	};
}
