#include "scripting/ScriptInstance.hpp"
#include "Logger.hpp"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include <mono/metadata/object.h>

namespace Phezu {

	ScriptInstance::ScriptInstance(MonoDomain* domain, ScriptClass* scriptClass)
	: m_Class(scriptClass), m_GcHandle(0) {
        
		m_Instance = mono_object_new(domain, scriptClass->GetMonoClass());

        if (m_Instance == nullptr)
        {
            Log("Error instantiating mono class %s", scriptClass->GetFullname().c_str());
            return;
        }

        m_GcHandle = mono_gchandle_new(m_Instance, false);
        mono_runtime_object_init(m_Instance);
	}
    
    ScriptInstance::ScriptInstance(ScriptInstance&& other) noexcept {
        m_Class = other.m_Class;
        m_GcHandle = other.m_GcHandle;
        m_Instance = other.m_Instance;
        
        other.m_Class = nullptr;
        other.m_GcHandle = 0;
        other.m_Instance = nullptr;
    }
    
    ScriptInstance& ScriptInstance::operator=(ScriptInstance&& other) noexcept {
        if (this != &other) {
            m_Class = other.m_Class;
            m_GcHandle = other.m_GcHandle;
            m_Instance = other.m_Instance;
            
            other.m_Class = nullptr;
            other.m_GcHandle = 0;
            other.m_Instance = nullptr;
        }
        
        return *this;
    }
    
    ScriptInstance::~ScriptInstance() {
        if (m_GcHandle > 0)
            mono_gchandle_free(m_GcHandle);
    }
    
    void ScriptInstance::SetUlongField(MonoClassField* field, uint64_t value) {
        mono_field_set_value(m_Instance, field, &value);
    }
    
    void ScriptInstance::SetInstanceRefField(MonoClassField* field, uint32_t gcHandle) {
        mono_field_set_value(m_Instance, field, &gcHandle);
    }
    
    void ScriptInstance::SetAssetRefField(MonoClassField* field, uint64_t guid) {
        mono_field_set_value(m_Instance, field, &guid);
    }
};
