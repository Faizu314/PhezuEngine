#pragma once

#include "scripting/MonoDefs.hpp"
#include "scripting/ScriptClass.hpp"
#include "scripting/ScriptEngine.hpp"

#include "scripting/EntityInstance.hpp"
#include "scripting/BehaviourInstance.hpp"
#include "scripting/ComponentInstance.hpp"

#include <vector>
#include <unordered_map>

namespace Phezu {
	struct EntityData {
	public:
        EntityData(uint64_t entityID, MonoDomain* domain, ScriptClass* entityClass) : EntityID(entityID), EntityScript(domain, entityClass) { }
        EntityData(const EntityData&) = delete;
        EntityData& operator=(const EntityData&) = delete;
	public:
		uint64_t EntityID;
		EntityInstance EntityScript;
		std::vector<BehaviourInstance> BehaviourScripts;
        std::unordered_map<NativeType, ComponentInstance> NativeComponents;
	};
}
