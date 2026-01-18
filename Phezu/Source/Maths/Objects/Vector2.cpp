#include "Maths/Objects/Vector2.hpp"
#include "Maths/Objects/Vector3.hpp"

namespace Phezu {

    const Vector2 Vector2::One = Vector2(1.0f, 1.0f);
    const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);
    const Vector2 Vector2::Right = Vector2(1.0f, 0.0f);
    const Vector2 Vector2::Left = Vector2(-1.0f, 0.0f);
    const Vector2 Vector2::Up = Vector2(0.0f, 1.0f);
    const Vector2 Vector2::Down = Vector2(0.0f, -1.0f);

    Vector2::Vector2(const Vector3& vec) : m_Vector(vec.X(), vec.Y()) {}

    void Vector2::Normalize() {
        if (glm::length(m_Vector) == 0)
            return;
        m_Vector = glm::normalize(m_Vector);
    }

    float Vector2::SqrDistance(const Vector2& a, const Vector2& b) {
        float dx = a.X() - b.X();
        float dy = a.Y() - b.Y();

        return (dx * dx) + (dy * dy);
    }
}