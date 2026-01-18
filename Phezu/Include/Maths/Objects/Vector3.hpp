#pragma once

#include "glm/glm.hpp"

namespace Phezu {

    class Vector2;

    class Vector3 {
    public:
        Vector3() : m_Vector(0.0f, 0.0f, 0.0f) {}
        Vector3(float x, float y) : m_Vector(x, y, 0.0f) {}
        Vector3(float x, float y, float z) : m_Vector(x, y, z) {}
        Vector3(const Vector2& vec);
        Vector3(const Vector3& vec) : m_Vector(vec.X(), vec.Y(), vec.Z()) {}
    public:
        float X() const { return m_Vector.x; }
        float Y() const { return m_Vector.y; }
        float Z() const { return m_Vector.z; }
        Vector3 SetX(float x) { m_Vector.x = x; return *this; }
        Vector3 SetY(float y) { m_Vector.y = y; return *this; }
        Vector3 SetZ(float z) { m_Vector.z = z; return *this; }
        Vector3 Set(float x, float y, float z) { SetX(x); SetY(y); return SetZ(z); }
    public:
        void Normalize();
        Vector3 Normalized() const { Vector3 a(*this); a.Normalize(); return a; }
        float Length() const { return Distance(*this, Zero); }
        float SqrLength() const { return SqrDistance(*this, Zero); }
    public:
        float Dot(const Vector3& other) const { return Dot(*this, other); }
        float Distance(const Vector3& other) const { return Distance(*this, other); }
        float SqrDistance(const Vector3& other) const { return SqrDistance(*this, other); }
    public:
        static float Dot(const Vector3& a, const Vector3& b) { return glm::dot(a.m_Vector, b.m_Vector); }
        static float Distance(const Vector3& a, const Vector3& b) { return glm::distance(a.m_Vector, b.m_Vector); }
        static float SqrDistance(const Vector3& a, const Vector3& b);
    public:
        static const Vector3 One;
        static const Vector3 Zero;
        static const Vector3 Right;
        static const Vector3 Left;
        static const Vector3 Up;
        static const Vector3 Down;
        static const Vector3 Forward;
        static const Vector3 Back;
    public:
        Vector3 operator+(const Vector3& other) const { return m_Vector + other.m_Vector; }
        Vector3 operator-(const Vector3& other) const { return Vector3(m_Vector.x - other.m_Vector.x, m_Vector.y - other.m_Vector.y); }
        Vector3& operator=(const Vector3& other) { Set(other.X(), other.Y(), other.Z()); return *this; }
        Vector3 operator*(float value) const { return Vector3(m_Vector * value); }
        Vector3 operator/(float value) const { return Vector3(m_Vector / value); }
        bool operator==(const Vector3& other) const { return m_Vector == other.m_Vector; }
        operator glm::vec3& () { return m_Vector; }
        operator const glm::vec3& () const { return m_Vector; }
    private:
        Vector3(const glm::vec2& other) : m_Vector(other.x, other.y, 0.0f) {}
        Vector3(const glm::vec3& other) : m_Vector(other.x, other.y, other.z) {}
    private:
        glm::vec3 m_Vector;

        friend class Mat3x3;
    };

}