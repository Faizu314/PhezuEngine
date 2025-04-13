#include "scripting/ScriptInstance.hpp"
#include "Logger.hpp"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

namespace Phezu {

	ScriptInstance::ScriptInstance(MonoDomain* domain, std::shared_ptr<ScriptClass> scriptClass)
	: m_Class(scriptClass){
		m_Instance = mono_object_new(domain, scriptClass->GetMonoClass());

		if (m_Instance == nullptr)
		{
			Log("Error instantiating mono class");
		}

		mono_runtime_object_init(m_Instance);
	}

	void ScriptInstance::InvokeOnCreate(MonoMethod* method) {
		MonoObject* exception = nullptr;

		mono_runtime_invoke(method, m_Class->GetMonoClass(), nullptr, &exception);

		if (exception) {
			MonoString* exceptionString = mono_object_to_string(exception, nullptr);
			char* exceptionCStr = mono_string_to_utf8(exceptionString);

			Log("Mono Exception: %s\n", exceptionCStr);

			mono_free(exceptionCStr);
		}
	}

	void ScriptInstance::InvokeOnUpdate(MonoMethod* method, float deltaTime) {
		MonoObject* exception = nullptr;

		void* params[] =
		{
			&deltaTime
		};

		mono_runtime_invoke(method, m_Class->GetMonoClass(), params, &exception);

		if (exception) {
			MonoString* exceptionString = mono_object_to_string(exception, nullptr);
			char* exceptionCStr = mono_string_to_utf8(exceptionString);

			Log("Mono Exception: %s\n", exceptionCStr);

			mono_free(exceptionCStr);
		}
	}
};