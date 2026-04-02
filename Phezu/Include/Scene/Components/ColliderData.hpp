// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "Maths/Math.hpp"
#include "Scene/Components/DataComponent.hpp"

#include <vector>
#include <variant>

namespace Phezu {

    enum class ColliderType {
        Circle,
        Box,
        Polygon
    };

    struct CircleCollider {
        float Radius;
    };

    struct BoxCollider {
        float Width;
        float Height;
    };

    struct PolygonCollider {
        std::vector<Vector2> Vertices;
    };

    using ColliderVariant = std::variant<CircleCollider, BoxCollider, PolygonCollider>;

    class ColliderData : public DataComponent {
    public:
        ColliderData(Entity* entity) : DataComponent(entity) {}
    public:
        ColliderType Type = ColliderType::Circle;
        Vector2 Offset = Vector2::Zero;
        ColliderVariant Collider;
    };
}
