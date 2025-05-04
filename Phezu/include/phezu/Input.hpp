#pragma once

#include "maths/Math.hpp"

namespace Phezu {
    
    struct InputData {
        bool W, A, S, D;
        bool Up, Right, Down, Left;
        bool Space, F, G, H;
        Vector2 MousePos;
        bool LeftMouse, RightMouse, MiddleMouse;
    };
    
    class Input {
    public:
        static void Init();
        static void Destroy();
        static bool PollInput();
        static const InputData& GetInput();
    };
}
