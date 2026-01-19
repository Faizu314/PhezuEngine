// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "Scripting/Core/MonoDefs.hpp"
#include "Scripting/Managed/ScriptClass.hpp"
#include "Scripting/Managed/ScriptInstance.hpp"

namespace Phezu {
	class EntityInstance : public ScriptInstance {
	public:
        EntityInstance(MonoDomain* domain, ScriptClass* scriptClass);
    public:
        EntityInstance(const EntityInstance&) = delete;
        EntityInstance& operator=(const EntityInstance&) = delete;
	};
}
