#include "Scripting/BehaviourInstance.hpp"
#include "Logger.hpp"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include <mono/metadata/object.h>

namespace Phezu {

    BehaviourInstance::BehaviourInstance(MonoDomain* domain, ScriptClass* scriptClass)
    : ComponentInstance(domain, scriptClass), m_OnCreateMethod(nullptr), m_OnUpdateMethod(nullptr), m_OnCollisionEnterMethod(nullptr), m_OnCollisionStayMethod(nullptr), m_OnCollisionExitMethod(nullptr) {
        m_OnCreateMethod = scriptClass->GetMonoMethod("OnCreate", 0);
        m_OnUpdateMethod = scriptClass->GetMonoMethod("OnUpdate", 1);
        m_OnDestroyMethod = scriptClass->GetMonoMethod("OnDestroy", 0);
        
        m_OnCollisionEnterMethod = scriptClass->GetMonoMethod("OnCollisionEnter", 1);
        m_OnCollisionStayMethod = scriptClass->GetMonoMethod("OnCollisionStay", 1);
        m_OnCollisionExitMethod = scriptClass->GetMonoMethod("OnCollisionExit", 1);
	}
    
    BehaviourInstance::BehaviourInstance(BehaviourInstance&& other) noexcept : ComponentInstance(std::move(other)) {
        m_OnCreateMethod = other.m_OnCreateMethod;
        m_OnUpdateMethod = other.m_OnUpdateMethod;
        m_OnCollisionEnterMethod = other.m_OnCollisionEnterMethod;
        m_OnCollisionStayMethod = other.m_OnCollisionStayMethod;
        m_OnCollisionExitMethod = other.m_OnCollisionExitMethod;
        
        other.m_OnCreateMethod = nullptr;
        other.m_OnUpdateMethod = nullptr;
        other.m_OnCollisionEnterMethod = nullptr;
        other.m_OnCollisionStayMethod = nullptr;
        other.m_OnCollisionExitMethod = nullptr;
    }
    
    BehaviourInstance& BehaviourInstance::operator=(BehaviourInstance&& other) noexcept {
        if (this != &other) {
            ScriptInstance::operator=(std::move(other));
            
            m_OnCreateMethod = other.m_OnCreateMethod;
            m_OnUpdateMethod = other.m_OnUpdateMethod;
            m_OnCollisionEnterMethod = other.m_OnCollisionEnterMethod;
            m_OnCollisionStayMethod = other.m_OnCollisionStayMethod;
            m_OnCollisionExitMethod = other.m_OnCollisionExitMethod;
            
            other.m_OnCreateMethod = nullptr;
            other.m_OnUpdateMethod = nullptr;
            other.m_OnCollisionEnterMethod = nullptr;
            other.m_OnCollisionStayMethod = nullptr;
            other.m_OnCollisionExitMethod = nullptr;
        }
        
        return *this;
    }

	void BehaviourInstance::InvokeOnCreate() {
		MonoObject* exception = nullptr;

		mono_runtime_invoke(m_OnCreateMethod, m_Instance, nullptr, &exception);

		if (exception) {
			MonoString* exceptionString = mono_object_to_string(exception, nullptr);
			char* exceptionCStr = mono_string_to_utf8(exceptionString);

			Log("Mono Exception: %s\n", exceptionCStr);

			mono_free(exceptionCStr);
		}
	}

	void BehaviourInstance::InvokeOnUpdate(float deltaTime) {
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
    
    void BehaviourInstance::InvokeOnDestroy() {
        MonoObject* exception = nullptr;

        mono_runtime_invoke(m_OnDestroyMethod, m_Instance, nullptr, &exception);

        if (exception) {
            MonoString* exceptionString = mono_object_to_string(exception, nullptr);
            char* exceptionCStr = mono_string_to_utf8(exceptionString);

            Log("Mono Exception: %s\n", exceptionCStr);

            mono_free(exceptionCStr);
        }
    }
    
    void BehaviourInstance::TryInvokeOnCollisionEnter(uint32_t otherEntity) {
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
    
    void BehaviourInstance::TryInvokeOnCollisionStay(uint32_t otherEntity) {
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
    
    void BehaviourInstance::TryInvokeOnCollisionExit(uint32_t otherEntity) {
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
};
