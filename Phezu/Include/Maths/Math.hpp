#pragma once

#include "glm/glm.hpp"

#include "Maths/Objects/Vector3.hpp"
#include "Maths/Objects/Vector2.hpp"
#include "Maths/Objects/Mat3x3.hpp"

namespace Phezu {

    enum class AngleUnit {
        Degrees, Radians
    };
    
    inline float Clamp01(float value) {
        return glm::clamp<float>(value, 0, 1);
    }
    
    inline float Clamp(float value, float min, float max) {
        return glm::clamp(value, min, max);
    }
    
    inline float Lerp(float start, float end, float t) {
        t = Clamp01(t);
        return start + (end - start) * t;
    }
    
    inline int Sign(float value) {
        return static_cast<int>(glm::sign(value));
    }
    
    inline float Sin(float angle, AngleUnit units = AngleUnit::Degrees) {
        float appAngle = units == AngleUnit::Radians ? angle : glm::radians(angle);
        return glm::sin(appAngle);
    }
    
    inline float Cos(float angle, AngleUnit units = AngleUnit::Degrees) {
        float appAngle = units == AngleUnit::Radians ? angle : glm::radians(angle);
        return glm::cos(appAngle);
    }
    
    int RoundToPixel(float value);
    void RoundToPixel(Vector2& vec);
}
