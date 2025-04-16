#pragma once

#include "scripting/MonoDefs.hpp"
#include "scripting/EntityInstance.hpp"

namespace Phezu {

	EntityInstance::EntityInstance(uint64_t entityID, MonoDomain* domain, std::shared_ptr<ScriptClass> entityClass)
		: EntityID(entityID), EntityScript(domain, entityClass) {}
}