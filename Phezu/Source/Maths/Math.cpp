#include "Maths/Math.hpp"

namespace Phezu {
    
    int RoundToPixel(float value) {
        float x = glm::round(glm::abs(value));
        return static_cast<int>(glm::round(value + x) - x);
    }
    
    void RoundToPixel(Vector2& vec) {
        vec.Set(static_cast<float>(RoundToPixel(vec.X())), static_cast<float>(RoundToPixel(vec.Y())));
    }
}
