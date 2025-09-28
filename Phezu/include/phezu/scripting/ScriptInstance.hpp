#pragma once

#include "scripting/MonoDefs.hpp"
#include "scripting/ScriptClass.hpp"

#include <string>

namespace Phezu {
	class ScriptInstance {
	public:
		ScriptInstance(MonoDomain* domain, ScriptClass* scriptClass);
        ScriptInstance(ScriptInstance&& other) noexcept;
        ScriptInstance& operator=(ScriptInstance&& other) noexcept;
        ~ScriptInstance();
    public:
        ScriptInstance(const ScriptInstance&) = delete;
        ScriptInstance& operator=(const ScriptInstance&) = delete;
    public:
		std::string GetFullname() { return m_Class->GetFullname(); }
        ScriptClass* GetScriptClass() { return m_Class; }
		MonoObject* GetMonoObject() { return m_Instance; }
        uint32_t GetMonoGcHandle() { return m_GcHandle; }
    public:
        void SetUlongField(MonoClassField* field, uint64_t value);
        void SetClassRefField(MonoClassField* field, MonoObject* value);
	protected:
        ScriptClass* m_Class;
        MonoObject* m_Instance;
        uint32_t m_GcHandle;
	};
}
