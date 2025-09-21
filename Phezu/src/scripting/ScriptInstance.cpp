#include "scripting/ScriptInstance.hpp"
#include "Logger.hpp"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include <mono/metadata/object.h>

namespace Phezu {

	ScriptInstance::ScriptInstance(MonoDomain* domain, ScriptClass* scriptClass)
	: m_Class(scriptClass), m_OnCreateMethod(nullptr), m_OnUpdateMethod(nullptr), m_GcHandle(0) {
        
		m_Instance = mono_object_new(domain, scriptClass->GetMonoClass());

        if (m_Instance == nullptr)
        {
            Log("Error instantiating mono class %s", scriptClass->GetFullname().c_str());
            return;
        }

        m_GcHandle = mono_gchandle_new(m_Instance, false);
        mono_runtime_object_init(m_Instance);

		if (scriptClass->GetScriptClassType() == ScriptClassType::ScriptComponent) {
			m_OnCreateMethod = scriptClass->GetMonoMethod("OnCreated", 0);
			m_OnUpdateMethod = scriptClass->GetMonoMethod("OnUpdate", 1);
            
            m_OnCollisionEnterMethod = scriptClass->GetMonoMethod("OnCollisionEnter", 1);
            m_OnCollisionStayMethod = scriptClass->GetMonoMethod("OnCollisionStay", 1);
            m_OnCollisionExitMethod = scriptClass->GetMonoMethod("OnCollisionExit", 1);
		}
	}
    
    ScriptInstance::ScriptInstance(ScriptInstance&& other) noexcept {
        m_Class = other.m_Class;
        m_GcHandle = other.m_GcHandle;
        m_Instance = other.m_Instance;
        m_OnCreateMethod = other.m_OnCreateMethod;
        m_OnUpdateMethod = other.m_OnUpdateMethod;
        m_OnCollisionEnterMethod = other.m_OnCollisionEnterMethod;
        m_OnCollisionStayMethod = other.m_OnCollisionStayMethod;
        m_OnCollisionExitMethod = other.m_OnCollisionExitMethod;
        
        other.m_Class = nullptr;
        other.m_GcHandle = 0;
        other.m_Instance = nullptr;
        other.m_OnCreateMethod = nullptr;
        other.m_OnUpdateMethod = nullptr;
        other.m_OnCollisionEnterMethod = nullptr;
        other.m_OnCollisionStayMethod = nullptr;
        other.m_OnCollisionExitMethod = nullptr;
    }
    
    ScriptInstance& ScriptInstance::operator=(ScriptInstance&& other) noexcept {
        m_Class = other.m_Class;
        m_GcHandle = other.m_GcHandle;
        m_Instance = other.m_Instance;
        m_OnCreateMethod = other.m_OnCreateMethod;
        m_OnUpdateMethod = other.m_OnUpdateMethod;
        m_OnCollisionEnterMethod = other.m_OnCollisionEnterMethod;
        m_OnCollisionStayMethod = other.m_OnCollisionStayMethod;
        m_OnCollisionExitMethod = other.m_OnCollisionExitMethod;
        
        other.m_Class = nullptr;
        other.m_GcHandle = 0;
        other.m_Instance = nullptr;
        other.m_OnCreateMethod = nullptr;
        other.m_OnUpdateMethod = nullptr;
        other.m_OnCollisionEnterMethod = nullptr;
        other.m_OnCollisionStayMethod = nullptr;
        other.m_OnCollisionExitMethod = nullptr;
    }
    
    ScriptInstance::~ScriptInstance() {
        if (m_GcHandle > 0)
            mono_gchandle_free(m_GcHandle);
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
    
    void ScriptInstance::TryInvokeOnCollisionEnter(uint32_t otherEntity) {
        if (m_OnCollisionEnterMethod == nullptr)
            return;
        
        MonoObject* exception = nullptr;

        void* params[] =
        {
            &otherEntity
        };

        mono_runtime_invoke(m_OnCollisionEnterMethod, m_Instance, params, &exception);

        if (exception) {
            MonoString* exceptionString = mono_object_to_string(exception, nullptr);
            char* exceptionCStr = mono_string_to_utf8(exceptionString);

            Log("Mono Exception: %s\n", exceptionCStr);

            mono_free(exceptionCStr);
        }
    }
    
    void ScriptInstance::TryInvokeOnCollisionStay(uint32_t otherEntity) {
        if (m_OnCollisionStayMethod == nullptr)
            return;
        
        MonoObject* exception = nullptr;

        void* params[] =
        {
            &otherEntity
        };

        mono_runtime_invoke(m_OnCollisionStayMethod, m_Instance, params, &exception);

        if (exception) {
            MonoString* exceptionString = mono_object_to_string(exception, nullptr);
            char* exceptionCStr = mono_string_to_utf8(exceptionString);

            Log("Mono Exception: %s\n", exceptionCStr);

            mono_free(exceptionCStr);
        }
    }
    
    void ScriptInstance::TryInvokeOnCollisionExit(uint32_t otherEntity) {
        if (m_OnCollisionExitMethod == nullptr)
            return;
        
        MonoObject* exception = nullptr;

        void* params[] =
        {
            &otherEntity
        };

        mono_runtime_invoke(m_OnCollisionExitMethod, m_Instance, params, &exception);

        if (exception) {
            MonoString* exceptionString = mono_object_to_string(exception, nullptr);
            char* exceptionCStr = mono_string_to_utf8(exceptionString);

            Log("Mono Exception: %s\n", exceptionCStr);

            mono_free(exceptionCStr);
        }
    }

	void ScriptInstance::SetEntityProperty(MonoMethod* propertySetter, uint32_t value) {
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
