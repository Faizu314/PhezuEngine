#include "Scripting/ComponentInstance.hpp"
#include "Logger.hpp"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include <mono/metadata/object.h>

namespace Phezu {

    ComponentInstance::ComponentInstance(MonoDomain* domain, ScriptClass* scriptClass)
	: ScriptInstance(domain, scriptClass) {}
    
    ComponentInstance::ComponentInstance(ComponentInstance&& other) noexcept : ScriptInstance(std::move(other)) {}

	void ComponentInstance::SetEntityProperty(MonoMethod* propertySetter, uint32_t value) {
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
};
