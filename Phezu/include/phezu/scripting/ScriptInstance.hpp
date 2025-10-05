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
        // Primitive Fields
        void SetUlongField(MonoClassField* field, uint64_t value);
    public:
        void SetStructField(MonoClassField* field, std::byte* data);
    public:
        void SetInstanceRefField(MonoClassField* field, uint32_t gcHandle);
        void SetAssetRefField(MonoClassField* field, uint64_t guid);
	protected:
        ScriptClass* m_Class;
        MonoObject* m_Instance;
        uint32_t m_GcHandle;
	};
}
