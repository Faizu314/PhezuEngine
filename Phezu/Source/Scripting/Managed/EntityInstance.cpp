#include "Scripting/Managed/EntityInstance.hpp"
#include "Core/Platform.hpp"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include <mono/metadata/object.h>

namespace Phezu {

    EntityInstance::EntityInstance(MonoDomain* domain, ScriptClass* scriptClass)
	: ScriptInstance(domain, scriptClass) {}
};
