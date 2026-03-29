// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "Maths/Math.hpp"
#include "Scene/Components/DataComponent.hpp"

namespace Phezu {
    
    enum class RigidbodyType {
        Kinematic = 0,
        Dynamic
    };

    class RigidbodyData : public DataComponent {
    public:
        RigidbodyData(Entity* entity, RigidbodyType type = RigidbodyType::Kinematic, float mass = 1.0f)
            : DataComponent(entity), Type(type), Mass(mass) {}
    public:
        RigidbodyType Type;
        Vector2 LinearVelocity = Vector2::Zero;
        float Mass;
        float AngularVelocity = 0.0f;
    };
}
