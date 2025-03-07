#pragma once

#include <unordered_map>
#include <string>
#include <typeindex>
#include <functional>

namespace Phezu {
    
    class Entity;
    
    class DataComponent {
    public:
        DataComponent() = delete;
        DataComponent(Entity* entity) : m_Entity(entity) {}
    public:
        Entity* GetEntity() const { return m_Entity; }
    protected:
        Entity* const m_Entity;
    };
}
