#include "Logger.hpp"

#include <iostream>

#if _WIN32

#include <windows.h>

#endif

namespace Phezu {

#if _WIN32

    constexpr int LOG_BUFFER_SIZE = 256;

    static char s_LogBuffer[LOG_BUFFER_SIZE];

    void Log(const char* msg, ...) {
        va_list args;

        va_start(args, msg);

        int length = vsnprintf(s_LogBuffer, LOG_BUFFER_SIZE, msg, args);
        s_LogBuffer[LOG_BUFFER_SIZE - 1] = '\0';

        va_end(args);

        OutputDebugString(s_LogBuffer);
    }

#elif __APPLE__

    void Log(const char* msg, ...) {
        va_list args;

        va_start(args, msg);

        vprintf(msg, args);

        va_end(args);
    }

#endif

}