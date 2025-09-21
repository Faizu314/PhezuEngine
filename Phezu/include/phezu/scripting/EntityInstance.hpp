#pragma once

#include "scripting/MonoDefs.hpp"
#include "scripting/ScriptClass.hpp"
#include "scripting/ScriptInstance.hpp"

namespace Phezu {
	class EntityInstance : public ScriptInstance {
	public:
        EntityInstance(MonoDomain* domain, ScriptClass* scriptClass);
    public:
        EntityInstance(const EntityInstance&) = delete;
        EntityInstance& operator=(const EntityInstance&) = delete;
    public:
		void SetUlongField(MonoClassField* field, uint64_t value);
	};
}
