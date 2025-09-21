#pragma once

#include <functional>
#include <unordered_map>

#include "maths/Math.hpp"
#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class Physics;
    class BehaviourComponent;
    struct Collision;
    
    class PhysicsData : public DataComponent {
    public:
        PhysicsData(Entity* entity, bool isStatic) : DataComponent(entity), m_IsStatic(isStatic) {}
    public:
        bool IsStatic() const { return m_IsStatic; }
    public:
        Vector2 Velocity;
    private:
        const bool m_IsStatic;
        
        friend Physics;
    };

    struct Collision {
        Collision(PhysicsData* other) : Other(other) {}
        PhysicsData* Other;
        Vector2 Point;
        Vector2 Normal;
    };
}
