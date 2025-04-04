#include "Logger.hpp"

namespace Phezu {

#if _WIN32

    constexpr int LOG_BUFFER_SIZE = 256;

    static char _logBuffer[LOG_BUFFER_SIZE];

    void Log(const char* msg, ...) {
        va_list args;

        va_start(args, msg);

        int length = vsnprintf(_logBuffer, LOG_BUFFER_SIZE, msg, args);

        va_end(args);

        OutputDebugString(_logBuffer);
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