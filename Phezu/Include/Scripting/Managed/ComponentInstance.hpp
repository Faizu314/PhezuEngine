// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <cstdint>

#include "Scripting/Core/MonoDefs.hpp"
#include "Scripting/Managed/ScriptClass.hpp"
#include "Scripting/Managed/ScriptInstance.hpp"

namespace Phezu {
	class ComponentInstance : public ScriptInstance {
	public:
        ComponentInstance(MonoDomain* domain, ScriptClass* scriptClass);
        ComponentInstance(ComponentInstance&& other) noexcept;
    public:
        ComponentInstance(const ComponentInstance&) = delete;
        ComponentInstance& operator=(const ComponentInstance&) = delete;
    public:
		void SetEntityProperty(MonoMethod* propertySetter, uint32_t value);
	};
}
