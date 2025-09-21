#include "scripting/EntityInstance.hpp"
#include "Logger.hpp"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include <mono/metadata/object.h>

namespace Phezu {

    EntityInstance::EntityInstance(MonoDomain* domain, ScriptClass* scriptClass)
	: ScriptInstance(domain, scriptClass) {}
    
    void EntityInstance::SetUlongField(MonoClassField* field, uint64_t value) {
        mono_field_set_value(m_Instance, field, &value);
    }
};
