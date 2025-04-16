#pragma once

#include "scene/components/DataComponent.hpp"

#include <string>

namespace Phezu {
    
    class ScriptComponent : public DataComponent {
    public:
        ScriptComponent(Entity* entity, const std::string& classFullName) : DataComponent(entity), m_ClassFullname(classFullName) {}
    public:
        std::string GetScriptClassFullname() const { return m_ClassFullname; }
    private:
        std::string m_ClassFullname;
    };
}
