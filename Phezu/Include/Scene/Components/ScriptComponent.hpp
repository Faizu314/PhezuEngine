#pragma once

#include "Scene/Components/DataComponent.hpp"

#include <string>

namespace Phezu {
    
    class ScriptComponent : public DataComponent {
    public:
        ScriptComponent(Entity* entity, const std::string& classFullName) : DataComponent(entity), m_ClassFullname(classFullName) {}
        ScriptComponent(ScriptComponent&& other) noexcept
            : DataComponent(other.m_Entity), m_ClassFullname(std::move(other.m_ClassFullname)) {}
    public:
        std::string GetScriptClassFullname() const { return m_ClassFullname; }
    private:
        std::string m_ClassFullname;
    };
}
