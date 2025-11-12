#pragma once

#include <functional>
#include <unordered_map>

#include "Maths/Math.hpp"
#include "Scene/Components/DataComponent.hpp"

namespace Phezu {
    
    class Physics;
    class BehaviourComponent;
    
    class PhysicsData : public DataComponent {
    public:
        PhysicsData(Entity* entity, bool isStatic = false, Vector2 velocity = Vector2::Zero) : DataComponent(entity), IsStatic(isStatic), Velocity(velocity) {}
    public:
        Vector2 Velocity;
        bool IsStatic;
        
        friend Physics;
    };

    struct Collision {
        Collision(PhysicsData* other) : Other(other) {}
        PhysicsData* Other;
        Vector2 Point;
        Vector2 Normal;
    };
}
