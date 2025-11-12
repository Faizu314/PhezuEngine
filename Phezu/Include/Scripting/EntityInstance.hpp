#pragma once

#include "Scripting/MonoDefs.hpp"
#include "Scripting/ScriptClass.hpp"
#include "Scripting/ScriptInstance.hpp"

namespace Phezu {
	class EntityInstance : public ScriptInstance {
	public:
        EntityInstance(MonoDomain* domain, ScriptClass* scriptClass);
    public:
        EntityInstance(const EntityInstance&) = delete;
        EntityInstance& operator=(const EntityInstance&) = delete;
	};
}
