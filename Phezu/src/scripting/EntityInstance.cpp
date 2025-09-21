#include "scripting/MonoDefs.hpp"
#include "scripting/EntityInstance.hpp"

namespace Phezu {

	EntityInstance::EntityInstance(uint64_t entityID, MonoDomain* domain, ScriptClass* entityClass)
		: EntityID(entityID), EntityScript(domain, entityClass) {}
}
