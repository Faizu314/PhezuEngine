#pragma once

#include "scripting/MonoDefs.hpp"
#include "scripting/ScriptClass.hpp"
#include "scripting/ScriptEngine.hpp"
#include "scripting/ScriptInstance.hpp"

#include <vector>
#include <unordered_map>

namespace Phezu {
	struct EntityInstance {
	public:
		EntityInstance(uint64_t entityID, MonoDomain* domain, std::shared_ptr<ScriptClass> entityClass);
        EntityInstance(const EntityInstance&) = delete;
        EntityInstance& operator=(const EntityInstance&) = delete;
	public:
		uint64_t EntityID;
		ScriptInstance EntityScript;
		std::vector<ScriptInstance> BehaviourScripts;
        std::unordered_map<NativeType, ScriptInstance> NativeComponents;
	};
}
