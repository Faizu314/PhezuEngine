// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

namespace Phezu {
    
    class Entity;
    
    enum class ComponentType {
        None = 0,
        Transform,
        Shape,
        Render,
        Physics,
        Camera,
    };
    
    class DataComponent {
    public:
        DataComponent(Entity* entity) : m_Entity(entity) {}
        DataComponent(DataComponent&& other) noexcept : m_Entity(other.m_Entity) {}
    public:
        DataComponent() = delete;
        DataComponent(const DataComponent&) = delete;
        DataComponent& operator=(const DataComponent&) = delete;
        virtual ~DataComponent() = default; 
    public:
        Entity* GetEntity() const { return m_Entity; }
    protected:
        Entity* const m_Entity;
    };
}
