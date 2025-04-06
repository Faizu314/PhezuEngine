#pragma once

#include "scripting/MonoLogger.hpp"

#ifndef _MONO_UTILS_FORWARD_
#define _MONO_UTILS_FORWARD_

struct _MonoDomain;
typedef struct _MonoDomain MonoDomain;

struct _MonoJitInfo;
typedef struct _MonoJitInfo MonoJitInfo;

#endif

namespace Phezu {

	class Engine;

	class ScriptEngine {
	public:
		ScriptEngine(Engine* engine);
	public:
		void Init();
		void Shutdown();
	private:
		Engine* m_Engine;
		MonoLogger m_MonoLogger;
		MonoDomain* m_RootDomain;
		MonoDomain* m_AppDomain;
	};
}