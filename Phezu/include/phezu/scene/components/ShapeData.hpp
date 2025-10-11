#pragma once

#include "maths/Math.hpp"
#include "scene/components/DataComponent.hpp"

class mat3;

namespace Phezu {
    
    class ShapeData : public DataComponent {
    public:
        enum VertexType {
            None = 0,
            UpLeft = 1,
            UpRight = 2,
            DownLeft = 3,
            DownRight = 4,
        };
    public:
        ShapeData(Entity* entity, Vector2 pivot = Vector2::Zero, Vector2 size = Vector2::One);
    public:
        void SetPivot(Vector2 pivot);
        void SetSize(Vector2 size);
        void Set(Vector2 pivot, Vector2 size);
    public:
        Vector2 GetVertexPosition(VertexType vertexType) const;
    private:
        Vector2 m_Pivot;
        Vector2 m_Size;
    };
}
