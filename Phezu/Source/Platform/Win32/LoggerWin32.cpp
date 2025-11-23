#include "Platform/Win32/LoggerWin32.hpp"

#include <iostream>
#include <windows.h>

namespace Phezu {

    void LoggerWin32::Init() {
        if (!GetConsoleWindow()) {
            if (!AttachConsole(ATTACH_PARENT_PROCESS))
                AllocConsole();

            FILE* fp;
            freopen_s(&fp, "CONOUT$", "w", stdout);
            freopen_s(&fp, "CONOUT$", "w", stderr);
            std::cout.clear();
            std::cerr.clear();
        }
    }

    void LoggerWin32::Log(const char* msg, ...) {
        va_list args;

        va_start(args, msg);

        int length = vsnprintf(m_LogBuffer, LOG_BUFFER_SIZE, msg, args);
        m_LogBuffer[LOG_BUFFER_SIZE - 1] = '\0';
        std::cout << m_LogBuffer;

        va_end(args);
    }
}