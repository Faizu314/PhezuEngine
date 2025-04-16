#pragma once

#include "scripting/MonoLogger.hpp"
#include "scripting/MonoDefs.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace Phezu {

	class Engine;
	class Scene;
	class Entity;
	class ScriptClass;
	struct EntityInstance;
	class ScriptComponent;

	class ScriptEngine {
	public:
		ScriptEngine(Engine* engine);
	public:
		void Init();
		void OnEntityCreated(std::shared_ptr<Entity> entity);
		void OnScriptComponentAddedToEntity(std::shared_ptr<Entity> entity, ScriptComponent* script);
		void OnEntityDestroyed(std::shared_ptr<Entity> entity);
		void OnUpdate(float deltaTime);
		void Shutdown();
	private:
		void InitMono();
		MonoAssembly* LoadAssembly(const std::string& assemblyPath);
		void GetScriptClasses();
		void PrintAssemblyClasses(MonoAssembly* assembly);
	private:
		Engine* m_Engine;
		MonoLogger m_MonoLogger;
		MonoDomain* m_RootDomain;
		MonoDomain* m_AppDomain;
		MonoAssembly* m_CoreAssembly;
	private:
		std::unordered_map<std::string, std::shared_ptr<ScriptClass>> m_BehaviourClasses;
		std::shared_ptr<ScriptClass> m_EntityClass;
		std::unordered_map<uint64_t, std::shared_ptr<EntityInstance>> m_EntityInstances;
	};
}