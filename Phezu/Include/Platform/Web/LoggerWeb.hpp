// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <cstdarg>

namespace Phezu {   

    class LoggerWeb {
    public:
        void Init();
        void Destroy();
        void Log(const char* msg, va_list args);
    private:
        static constexpr int LOG_BUFFER_SIZE = 256 * 16 * 4;
        char m_LogBuffer[LOG_BUFFER_SIZE];
    };
}
