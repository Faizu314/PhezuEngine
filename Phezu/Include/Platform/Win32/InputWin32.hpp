#pragma once

#include "Core/Input.hpp"
#include "Maths/Math.hpp"

namespace Phezu {
    
    class InputWin32 : public IInput {
    public:
        void Init() override;
        void Destroy() override;
        bool PollInput() override;
        const InputData& GetInput() override;
    private:
        InputData m_InputData;
    };
}
