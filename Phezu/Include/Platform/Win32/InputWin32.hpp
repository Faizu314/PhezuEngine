#pragma once

#include "Core/Platform.hpp"
#include "Maths/Math.hpp"

namespace Phezu {
    
    class InputWin32 {
    public:
        void Init();
        void Destroy();
        const InputData& GetInput();
    private:
        InputData m_InputData;
    };
}
