#include "Platform/Input.hpp"

#include <cstring>

namespace Phezu {
    
    static InputData s_InputData;
    
    void Input::Init() {
        memset(&s_InputData, 0, sizeof(s_InputData));
    }
    
    void Input::Destroy() {
        memset(&s_InputData, 0, sizeof(s_InputData));
    }
        
    bool Input::PollInput() { 
        return true;
    }

    const InputData& Input::GetInput() {
        return s_InputData;
    }
}
