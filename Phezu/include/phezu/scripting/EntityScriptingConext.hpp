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
	struct EntityScriptingConext {
	public:
        EntityScriptingConext(uint64_t entityID, MonoDomain* domain, ScriptClass* entityClass)
        : EntityID(entityID), EntityScript(domain, entityClass) { }
    public:
        EntityScriptingConext(const EntityScriptingConext&) = delete;
        EntityScriptingConext& operator=(const EntityScriptingConext&) = delete;
	public:
		uint64_t EntityID;
    public:
		EntityInstance EntityScript;
        std::unordered_map<ManagedType, ComponentInstance> EngineComponents;
        std::vector<BehaviourInstance> BehaviourComponents;
	};
}
