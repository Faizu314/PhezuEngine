#pragma once

#include "Scripting/ScriptEngine.hpp"
#include "Scripting/ScriptClass.hpp"
#include "Scripting/EntityInstance.hpp"
#include "Scripting/BehaviourInstance.hpp"
#include "Scripting/ComponentInstance.hpp"

#include <vector>
#include <unordered_map>

namespace Phezu {
	struct EntityScriptingContext {
	public:
        EntityScriptingContext(uint64_t entityID, MonoDomain* domain, ScriptClass* entityClass)
        : EntityID(entityID), EntityScript(domain, entityClass) { }
    public:
        EntityScriptingContext(const EntityScriptingContext&) = delete;
        EntityScriptingContext& operator=(const EntityScriptingContext&) = delete;
	public:
		uint64_t EntityID;
    public:
		EntityInstance EntityScript;
        std::unordered_map<ManagedType, ComponentInstance> EngineComponents;
        std::vector<BehaviourInstance> BehaviourComponents;
	};
}
