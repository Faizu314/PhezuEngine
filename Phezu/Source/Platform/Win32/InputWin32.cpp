#include <string>

#include "Platform/Win32/InputWin32.hpp"

namespace Phezu {
        
    void InputWin32::Init() {
        std::memset(&m_InputData, 0, sizeof(m_InputData));
    }
    
    void InputWin32::Destroy() {
        std::memset(&m_InputData, 0, sizeof(m_InputData));
    }

    const InputData& InputWin32::GetInput() {
        return m_InputData;
    }
}
