#pragma once

#include "Core/Logger.hpp"

namespace Phezu {   

    class LoggerWin32 : public ILogger {
    public:
        void Init() override;
        void Log(const char* msg, ...) override;
    private:
        static constexpr int LOG_BUFFER_SIZE = 256;
        char m_LogBuffer[LOG_BUFFER_SIZE];
    };
}
