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
    
    enum class PhysicsEventType {
        CollisionEnter,
        CollisionStay,
        CollisionExit
    };
    
    enum class NativeType {
        None,
        Transform,
        Shape,
        Renderer,
        Physics,
        ScriptComponent
    };
    
    std::string ToString(NativeType t);
    
	class ScriptEngine {
	public:
		ScriptEngine(Engine* engine);
	public:
		void Init();
		void OnEntityCreated(std::shared_ptr<Entity> entity);
		void OnEntityDestroyed(std::shared_ptr<Entity> entity);
        void PreUpdate();
		void OnUpdate(float deltaTime);
        void FirePhysicsCollisionEvent(uint64_t entityA, uint64_t entityB, PhysicsEventType eventType);
		void Shutdown();
	public:
		MonoClass* GetBehaviourComponentClass();
		ScriptInstance* GetBehaviourScriptInstance(uint64_t entityID, const std::string& classFullname);
        ScriptInstance* GetNativeComponentInstance(uint64_t entityID, const NativeType componentType);
	private:
		void InitMono();
		MonoAssembly* LoadAssembly(const std::string& assemblyPath);
        void GetInputClassAndFields();
        void GetEngineClasses();
		void GetScriptClasses();
    private:
        void PrintAssemblyClasses(MonoAssembly* assembly);
	private:
		Engine* m_Engine;
		MonoLogger m_MonoLogger;
		MonoDomain* m_EngineDomain;
		MonoDomain* m_RuntimeDomain;
		MonoAssembly* m_CoreAssembly;
	private:
        std::shared_ptr<ScriptClass> m_ObjectClass;
        std::shared_ptr<ScriptClass> m_ComponentClass;
        std::shared_ptr<ScriptClass> m_BehaviourComponentClass;
        std::shared_ptr<ScriptClass> m_EntityClass;
    private:
        MonoVTable* m_InputClassVTable;
        InputFields m_InputFields;
	private:
		MonoClassField* m_EntityIdField;
		MonoMethod* m_ComponentEntitySetter;
	private:
		std::unordered_map<std::string, std::shared_ptr<ScriptClass>> m_ScriptClasses;
        std::unordered_map<NativeType, std::shared_ptr<ScriptClass>> m_NativeComponentClasses;
		std::unordered_map<uint64_t, std::shared_ptr<EntityInstance>> m_EntityInstances;
	};
}
