#pragma once

#include <memory>

namespace Phezu {
	class ScriptEngine;
	class Engine;

	class ScriptGlue {
	public:
		void static Init(Engine* engine, ScriptEngine* scriptEngine);
		void static Bind();
		void static Destroy();
	};
}