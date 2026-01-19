// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>

#include "MonoDefs.hpp"
#include "ScriptDefs.hpp"
#include "Scripting/Managed/EntityInstance.hpp"
#include "Scripting/Managed/BehaviourInstance.hpp"
#include "Scripting/Managed/ComponentInstance.hpp"
#include "Scripting/Managed/ScriptClass.hpp"

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
