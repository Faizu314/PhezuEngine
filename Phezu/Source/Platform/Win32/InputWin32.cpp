#include "Platform/Win32/InputWin32.hpp"

#include <cstring>

namespace Phezu {
        
    void InputWin32::Init() {
        memset(&m_InputData, 0, sizeof(m_InputData));
    }
    
    void InputWin32::Destroy() {
        memset(&m_InputData, 0, sizeof(m_InputData));
    }
        
    bool InputWin32::PollInput() {
        return true;
    }

    const InputData& InputWin32::GetInput() {
        return m_InputData;
    }
}
