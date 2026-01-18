#pragma once

#include <cstdarg>

namespace Phezu {   

    class LoggerWin32 {
    public:
        void Init();
        void Destroy();
        void Log(const char* msg, va_list args);
    private:
        static constexpr int LOG_BUFFER_SIZE = 256;
        char m_LogBuffer[LOG_BUFFER_SIZE];
    };
}
