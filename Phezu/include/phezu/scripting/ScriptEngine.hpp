#pragma once

#include "scripting/MonoLogger.hpp"
#include "scripting/MonoDefs.hpp"
#include "scripting/ScriptClass.hpp"

#include <string>
#include <unordered_map>

namespace Phezu {

	class Engine;

	class ScriptEngine {
	public:
		ScriptEngine(Engine* engine);
	public:
		void Init();
		void OnEntityCreated(std::shared_ptr<Scene> scene, uint64_t entityID);
		void OnUpdate();
		void OnEntityDestroyed(std::shared_ptr<Scene> scene, uint64_t entityID);
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
	};
}