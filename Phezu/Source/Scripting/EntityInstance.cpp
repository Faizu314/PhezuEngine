#include "Scripting/EntityInstance.hpp"
#include "Platform/Logger.hpp"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include <mono/metadata/object.h>

namespace Phezu {

    EntityInstance::EntityInstance(MonoDomain* domain, ScriptClass* scriptClass)
	: ScriptInstance(domain, scriptClass) {}
};
