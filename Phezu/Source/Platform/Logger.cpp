#include "Platform/Logger.hpp"

#include <iostream>

#if _WIN32
#include <windows.h>
#endif

namespace Phezu {

#if _WIN32
    constexpr int LOG_BUFFER_SIZE = 256;
    static char s_LogBuffer[LOG_BUFFER_SIZE];
#endif

    void InitLogger() {
#if _WIN32
        // Ensure a console is attached on windows for logging
        if (!GetConsoleWindow()) {
            if (!AttachConsole(ATTACH_PARENT_PROCESS))
                AllocConsole();

            FILE* fp;
            freopen_s(&fp, "CONOUT$", "w", stdout);
            freopen_s(&fp, "CONOUT$", "w", stderr);
            std::cout.clear();
            std::cerr.clear();
        }
#endif
    }

    void Log(const char* msg, ...) {
        va_list args;

        va_start(args, msg);

#if _WIN32
        int length = vsnprintf(s_LogBuffer, LOG_BUFFER_SIZE, msg, args);
        s_LogBuffer[LOG_BUFFER_SIZE - 1] = '\0';
        std::cout << s_LogBuffer;
#elif __APPLE__
        vprintf(msg, args);
#endif

        va_end(args);
    }

}