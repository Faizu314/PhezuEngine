#pragma once

#include "scripting/MonoLogger.hpp"
#include "scripting/MonoDefs.hpp"

namespace Phezu {

	class Engine;

	class ScriptEngine {
	public:
		ScriptEngine(Engine* engine);
	public:
		void Init();
		void Shutdown();
	private:
		void InitMono();
		void PrintAssemblyClasses(MonoAssembly* assembly);
	private:
		Engine* m_Engine;
		MonoLogger m_MonoLogger;
		MonoDomain* m_RootDomain;
		MonoDomain* m_AppDomain;
		MonoAssembly* m_CoreAssembly;
	};
}