#include <string>

#include "Platform/Win32/InputWin32.hpp"
#include "Maths/Math.hpp"

namespace Phezu {
        
    void InputWin32::Init() {
        std::memset(&m_InputData, 0, sizeof(m_InputData));
    }
    
    void InputWin32::Destroy() {
        std::memset(&m_InputData, 0, sizeof(m_InputData));
    }

    int InputWin32::WindowProc(UINT msg, WPARAM wParam) {
        bool isKeyDown = msg == WM_KEYDOWN;

		switch (wParam) {
            case 'W': m_InputData.W = isKeyDown; break;
            case 'A': m_InputData.A = isKeyDown; break;
            case 'S': m_InputData.S = isKeyDown; break;
            case 'D': m_InputData.D = isKeyDown; break;

            case 'F':      m_InputData.F = isKeyDown; break;
            case 'G':      m_InputData.G = isKeyDown; break;
            case 'H':      m_InputData.H = isKeyDown; break;

            case VK_UP:    m_InputData.Up    = isKeyDown; break;
            case VK_RIGHT: m_InputData.Right = isKeyDown; break;
            case VK_DOWN:  m_InputData.Down  = isKeyDown; break;
            case VK_LEFT:  m_InputData.Left  = isKeyDown; break;
		}

        return 0;
    }

    const InputData& InputWin32::GetInput() {
        return m_InputData;
    }
}
