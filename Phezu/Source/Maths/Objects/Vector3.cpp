#include "Maths/Objects/Vector3.hpp"
#include "Maths/Objects/Vector2.hpp"

namespace Phezu {

    const Vector3 Vector3::One = Vector3(1.0f, 1.0f, 1.0f);
    const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);
    const Vector3 Vector3::Right = Vector3(1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::Left = Vector3(-1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::Up = Vector3(0.0f, 1.0f, 0.0f);
    const Vector3 Vector3::Down = Vector3(0.0f, -1.0f, 0.0f);
    const Vector3 Vector3::Forward = Vector3(0.0f, 0.0f, 1.0f);
    const Vector3 Vector3::Back = Vector3(0.0f, 0.0f, -1.0f);

    Vector3::Vector3(const Vector2& vec) : m_Vector(vec.X(), vec.Y(), 0.0f) {}

    void Vector3::Normalize() {
        if (glm::length(m_Vector) == 0)
            return;
        m_Vector = glm::normalize(m_Vector);
    }

    float Vector3::SqrDistance(const Vector3& a, const Vector3& b) {
        float dx = a.X() - b.X();
        float dy = a.Y() - b.Y();
        float dz = a.Z() - b.Z();

        return (dx * dx) + (dy * dy) + (dz * dz);
    }
}