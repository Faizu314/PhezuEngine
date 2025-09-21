#pragma once

#include "scripting/MonoDefs.hpp"
#include "scripting/ScriptClass.hpp"
#include "scripting/ComponentInstance.hpp"

namespace Phezu {
	class BehaviourInstance : public ComponentInstance {
	public:
        BehaviourInstance(MonoDomain* domain, ScriptClass* scriptClass);
        BehaviourInstance(BehaviourInstance&& other) noexcept;
        BehaviourInstance& operator=(BehaviourInstance&& other) noexcept;
    public:
        BehaviourInstance(const BehaviourInstance&) = delete;
        BehaviourInstance& operator=(const BehaviourInstance&) = delete;
	public:
		void InvokeOnCreate();
		void InvokeOnUpdate(float deltaTime);
    public:
        void TryInvokeOnCollisionEnter(uint32_t otherEntity);
        void TryInvokeOnCollisionStay(uint32_t otherEntity);
        void TryInvokeOnCollisionExit(uint32_t otherEntity);
	private:
        MonoMethod* m_OnCreateMethod;
        MonoMethod* m_OnUpdateMethod;
        MonoMethod* m_OnCollisionEnterMethod;
        MonoMethod* m_OnCollisionStayMethod;
        MonoMethod* m_OnCollisionExitMethod;
	};
}
