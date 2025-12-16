#pragma once

#include <string>
#include <unordered_map>

#include "Scripting/MonoDefs.hpp"
#include "Scripting/ScriptDefs.hpp"
#include "Scripting/EntityScriptingContext.hpp"

namespace Phezu {

	class Engine;
	class Entity;
	class ScriptClass;
	class ScriptInstance;
    
    std::string ToString(ManagedType t);
    
	class ScriptEngine {
	public:
		ScriptEngine(Engine* engine);
    public:
        ScriptEngine() = delete;
        ScriptEngine(const ScriptEngine&) = delete;
        ScriptEngine& operator=(const ScriptEngine&) = delete;
	public:
		void Init();
		void CreateManagedScripts(Entity* entity);
        void InitializeManagedScripts(Entity* entity);
		void OnEntityDestroyed(Entity* entity);
        void PreUpdate();
		void OnUpdate(float deltaTime);
        void FirePhysicsCollisionEvent(uint64_t entityA, uint64_t entityB, PhysicsEventType eventType);
		void Shutdown();
	public:
		MonoClass* GetBehaviourComponentClass();
		ScriptInstance* GetBehaviourScriptInstance(uint64_t entityID, const std::string& classFullname);
        ScriptInstance* GetEngineComponentInstance(uint64_t entityID, ManagedType componentType);
        void RemoveBehaviourScriptInstance(uint64_t entityID, const std::string& classFullname);
        void RemoveEngineComponentInstance(uint64_t entityID, const std::string& classFullname);
        uint32_t GetEntityScriptInstanceGcHandle(uint64_t entityID);
	private:
		void InitMono();
		void ShutdownMono();
        void GetInputClassAndFields();
        void GetEngineClasses();
		void GetScriptClasses();
		MonoAssembly* LoadAssembly(const std::string& assemblyPath);
    private:
        void PrintAssemblyClasses(MonoAssembly* assembly);
	private:
		Engine* m_Engine;
		MonoDomain* m_RootDomain;
		MonoAssembly* m_EngineAssembly;
		MonoAssembly* m_GameAssembly;
	private:
        ScriptClass* m_ObjectClass;
        ScriptClass* m_ComponentClass;
        ScriptClass* m_BehaviourComponentClass;
        ScriptClass* m_EntityClass;
    private:
        MonoVTable* m_InputClassVTable;
        InputFields m_InputFields{};
	private:
		MonoClassField* m_EntityIdField;
		MonoMethod* m_ComponentEntitySetter;
	private:
		std::unordered_map<std::string, ScriptClass*> m_ScriptClasses;
        std::unordered_map<ManagedType, ScriptClass*> m_EngineComponentClasses;
		std::unordered_map<uint64_t, EntityScriptingContext> m_Entities;
	};
}
