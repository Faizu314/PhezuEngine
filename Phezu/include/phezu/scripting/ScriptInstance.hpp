#pragma once

#include "scripting/MonoDefs.hpp"
#include "scripting/ScriptClass.hpp"

#include <string>
#include <memory>

namespace Phezu {
	class ScriptInstance {
	public:
		ScriptInstance(MonoDomain* domain, std::shared_ptr<ScriptClass> scriptClass);
        ScriptInstance(ScriptInstance&& other) noexcept;
        ScriptInstance& operator=(ScriptInstance&& other) noexcept;
        ~ScriptInstance();
    public:
        ScriptInstance(const ScriptInstance&) = delete;
        ScriptInstance& operator=(const ScriptInstance&) = delete;
	public:
		void InvokeOnCreate();
		void InvokeOnUpdate(float deltaTime);
    public:
        void TryInvokeOnCollisionEnter(uint32_t otherEntity);
        void TryInvokeOnCollisionStay(uint32_t otherEntity);
        void TryInvokeOnCollisionExit(uint32_t otherEntity);
    public:
		void SetEntityProperty(MonoMethod* propertySetter, uint32_t value);
		void SetUlongField(MonoClassField* field, uint64_t value);
		std::string GetFullname() { return m_Class->GetFullname(); }
		MonoObject* GetMonoObject() { return m_Instance; }
        uint32_t GetMonoGcHandle() { return m_GcHandle; }
	private:
		std::shared_ptr<ScriptClass> m_Class;
        uint32_t m_GcHandle;
        MonoObject* m_Instance;
        MonoMethod* m_OnCreateMethod;
        MonoMethod* m_OnUpdateMethod;
        MonoMethod* m_OnCollisionEnterMethod;
        MonoMethod* m_OnCollisionStayMethod;
        MonoMethod* m_OnCollisionExitMethod;
	};
}
