#pragma once

#include "scripting/MonoDefs.hpp"
#include "scripting/ScriptClass.hpp"

#include <string>
#include <memory>

namespace Phezu {
	class ScriptInstance {
	public:
		ScriptInstance(MonoDomain* domain, std::shared_ptr<ScriptClass> scriptClass, MonoMethod* gcHandleGetter);
	public:
		void InvokeOnCreate();
		void InvokeOnUpdate(float deltaTime);
		void SetGcHandleProperty(MonoMethod* propertySetter, intptr_t value);
		void SetUlongField(MonoClassField* field, uint64_t value);
		std::string GetFullname() { return m_Class->GetFullname(); }
		MonoObject* GetMonoObject() { return m_Instance; }
		intptr_t GetMonoGcHandle() { return m_IntPtr; }
	private:
		std::shared_ptr<ScriptClass> m_Class;
		MonoObject* m_Instance;
		MonoMethod* m_OnCreateMethod;
		MonoMethod* m_OnUpdateMethod;
		intptr_t m_IntPtr;
	};
}