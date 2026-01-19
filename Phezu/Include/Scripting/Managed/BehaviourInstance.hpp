// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <cstdint>

#include "Scripting/Core/MonoDefs.hpp"
#include "Scripting/Managed/ScriptClass.hpp"
#include "Scripting/Managed/ComponentInstance.hpp"

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
        bool HasOnCreate() { return m_OnCreateMethod != nullptr; }
        bool HasOnUpdate() { return m_OnUpdateMethod != nullptr; }
        bool HasOnDestroy() { return m_OnDestroyMethod != nullptr; }
		void InvokeOnCreate();
		void InvokeOnUpdate(float deltaTime);
        void InvokeOnDestroy();
    public:
        void TryInvokeOnCollisionEnter(uint32_t otherEntity);
        void TryInvokeOnCollisionStay(uint32_t otherEntity);
        void TryInvokeOnCollisionExit(uint32_t otherEntity);
	private:
        MonoMethod* m_OnCreateMethod;
        MonoMethod* m_OnUpdateMethod;
        MonoMethod* m_OnDestroyMethod;
        MonoMethod* m_OnCollisionEnterMethod;
        MonoMethod* m_OnCollisionStayMethod;
        MonoMethod* m_OnCollisionExitMethod;
	};
}
