#include "scripting/ScriptInstance.hpp"
#include "Logger.hpp"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

namespace Phezu {

	ScriptInstance::ScriptInstance(MonoDomain* domain, std::shared_ptr<ScriptClass> scriptClass, MonoMethod* gcHandleGetter)
	: m_Class(scriptClass), m_OnCreateMethod(nullptr), m_OnUpdateMethod(nullptr), m_IntPtr(0) {
		m_Instance = mono_object_new(domain, scriptClass->GetMonoClass());

		if (m_Instance == nullptr)
		{
			Log("Error instantiating mono class");
		}

		mono_runtime_object_init(m_Instance);

		MonoObject* result = mono_runtime_invoke(gcHandleGetter, m_Instance, nullptr, nullptr);
		m_IntPtr = *(intptr_t*)mono_object_unbox(result);

		if (scriptClass->GetScriptClassType() == ScriptClassType::BehaviourComponent ||
			scriptClass->GetScriptClassType() == ScriptClassType::ScriptComponent) {
			m_OnCreateMethod = scriptClass->GetMonoMethod("OnCreated", 0);
			m_OnUpdateMethod = scriptClass->GetMonoMethod("OnUpdate", 1);
		}
	}

	void ScriptInstance::InvokeOnCreate() {
		MonoObject* exception = nullptr;

		mono_runtime_invoke(m_OnCreateMethod, m_Instance, nullptr, &exception);

		if (exception) {
			MonoString* exceptionString = mono_object_to_string(exception, nullptr);
			char* exceptionCStr = mono_string_to_utf8(exceptionString);

			Log("Mono Exception: %s\n", exceptionCStr);

			mono_free(exceptionCStr);
		}
	}

	void ScriptInstance::InvokeOnUpdate(float deltaTime) {
		MonoObject* exception = nullptr;

		void* params[] =
		{
			&deltaTime
		};

		mono_runtime_invoke(m_OnUpdateMethod, m_Instance, params, &exception);

		if (exception) {
			MonoString* exceptionString = mono_object_to_string(exception, nullptr);
			char* exceptionCStr = mono_string_to_utf8(exceptionString);

			Log("Mono Exception: %s\n", exceptionCStr);

			mono_free(exceptionCStr);
		}
	}

	void ScriptInstance::SetGcHandleProperty(MonoMethod* propertySetter, intptr_t value) {
		MonoObject* exception = nullptr;

		void* params[] =
		{
			&value
		};

		mono_runtime_invoke(propertySetter, m_Instance, params, &exception);

		if (exception) {
			MonoString* exceptionString = mono_object_to_string(exception, nullptr);
			char* exceptionCStr = mono_string_to_utf8(exceptionString);

			Log("Mono Exception: %s\n", exceptionCStr);

			mono_free(exceptionCStr);
		}
	}

	void ScriptInstance::SetUlongField(MonoClassField* field, uint64_t value) {
		mono_field_set_value(m_Instance, field, &value);
	}
};