#pragma once

#include "scripting/MonoDefs.hpp"
#include "scripting/ScriptClass.hpp"
#include "scripting/ScriptInstance.hpp"

#include <vector>

namespace Phezu {
	struct EntityInstance {
	public:
		EntityInstance(uint64_t entityID, MonoDomain* domain, std::shared_ptr<ScriptClass> entityClass, MonoMethod* gcHandleGetter);
	public:
		uint64_t EntityID;
		ScriptInstance EntityScript;
		std::vector<ScriptInstance> BehaviourScripts;
	};
}
