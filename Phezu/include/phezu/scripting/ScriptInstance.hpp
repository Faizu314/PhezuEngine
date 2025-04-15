#pragma once

#include "scripting/MonoDefs.hpp"
#include "scripting/ScriptClass.hpp"

#include <string>
#include <memory>

namespace Phezu {
	class ScriptInstance {
	public:
		ScriptInstance(MonoDomain* domain, std::shared_ptr<ScriptClass> scriptClass);
	public:
		void InvokeOnCreate();
		void InvokeOnUpdate(float deltaTime);
	private:
		std::shared_ptr<ScriptClass> m_Class;
		MonoObject* m_Instance;
		MonoMethod* m_OnCreateMethod;
		MonoMethod* m_OnUpdateMethod;
	};
}