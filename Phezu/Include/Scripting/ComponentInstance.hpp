#pragma once

#include "Scripting/MonoDefs.hpp"
#include "Scripting/ScriptClass.hpp"
#include "Scripting/ScriptInstance.hpp"

#include <string>

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
