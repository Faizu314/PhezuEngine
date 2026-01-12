#pragma once

#include <Windows.h>

#include "Core/Platform.hpp"

namespace Phezu {
    
    class InputWin32 {
    public:
        void Init();
        void Destroy();
        int WindowProc(UINT msg, WPARAM wParam);
        const InputData& GetInput();
    private:
        InputData m_InputData;
    };
}
