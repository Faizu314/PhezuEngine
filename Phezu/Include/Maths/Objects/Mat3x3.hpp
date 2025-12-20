#pragma once

#include "glm/glm.hpp"

#include "Maths/Objects/Vector3.hpp"

namespace Phezu {

    class Mat3x3 {
    public:
        Mat3x3() : m_Matrix(1.0f) {}
    public:
        float Get(int i, int j) { return m_Matrix[i][j]; }
        void Set(int i, int j, float value) { m_Matrix[i][j] = value; }
    public:
        Mat3x3 Inversed() { return glm::inverse(m_Matrix); }
    public:
        Mat3x3 operator*(const Mat3x3& other) const { return m_Matrix * other.m_Matrix; }
        Vector3 operator*(const Vector3& other) const { return m_Matrix * other.m_Vector; }
        operator glm::mat3& () { return m_Matrix; }
        operator const glm::mat3& () const { return m_Matrix; }
    private:
        Mat3x3(glm::mat3 matrix) : m_Matrix(matrix) {}
    private:
        glm::mat3 m_Matrix;
    };
}