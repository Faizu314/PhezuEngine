#pragma once

namespace Phezu {
    
    class Entity;
    
    class DataComponent {
    public:
        DataComponent(Entity* entity) : m_Entity(entity) {}
        DataComponent(DataComponent&& other) noexcept : m_Entity(other.m_Entity) {}
    public:
        DataComponent() = delete;
        DataComponent(const DataComponent&) = delete;
        DataComponent& operator=(const DataComponent&) = delete;
    public:
        Entity* GetEntity() const { return m_Entity; }
    protected:
        Entity* const m_Entity;
    };
}
