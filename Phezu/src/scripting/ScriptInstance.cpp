#include "scripting/ScriptInstance.hpp"
#include "Logger.hpp"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

namespace Phezu {

	ScriptInstance::ScriptInstance(MonoDomain* domain, std::shared_ptr<ScriptClass> scriptClass)
	: m_Class(scriptClass) {
		m_Instance = mono_object_new(domain, scriptClass->GetMonoClass());

		if (m_Instance == nullptr)
		{
			Log("Error instantiating mono class");
		}

		mono_runtime_object_init(m_Instance);

		m_OnCreateMethod = scriptClass->GetMonoMethod("OnCreated", 0);
		m_OnUpdateMethod = scriptClass->GetMonoMethod("OnUpdate", 1);
	}

	void ScriptInstance::InvokeOnCreate() {
		MonoObject* exception = nullptr;

		mono_runtime_invoke(m_OnCreateMethod, m_Class->GetMonoClass(), nullptr, &exception);

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

		mono_runtime_invoke(m_OnUpdateMethod, m_Class->GetMonoClass(), params, &exception);

		if (exception) {
			MonoString* exceptionString = mono_object_to_string(exception, nullptr);
			char* exceptionCStr = mono_string_to_utf8(exceptionString);

			Log("Mono Exception: %s\n", exceptionCStr);

			mono_free(exceptionCStr);
		}
	}
};