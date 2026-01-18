#include <iostream>
#include <windows.h>

#include "Platform/Win32/LoggerWin32.hpp"

namespace Phezu {

    void LoggerWin32::Init() {
        if (!GetConsoleWindow()) {
            if (!AttachConsole(ATTACH_PARENT_PROCESS))
                AllocConsole();

            FILE* fp;
            freopen_s(&fp, "CONOUT$", "w", stdout);
            freopen_s(&fp, "CONOUT$", "w", stderr);
            freopen_s(&fp, "CONIN$", "r", stdin);
            std::cout.clear();
            std::cerr.clear();
            std::cin.clear();
        }

        std::memset(m_LogBuffer, 0, LOG_BUFFER_SIZE);
    }

    void LoggerWin32::Destroy() {
        fflush(stdout);
        fflush(stderr);

        FILE* fp;
        freopen_s(&fp, "NUL", "w", stdout);
        freopen_s(&fp, "NUL", "w", stderr);
        freopen_s(&fp, "NUL", "r", stdin);

        std::cout.clear();
        std::cerr.clear();
        std::cin.clear();

        FreeConsole();
    }

    void LoggerWin32::Log(const char* msg, va_list args) {
        int length = vsnprintf(m_LogBuffer, LOG_BUFFER_SIZE, msg, args);
        m_LogBuffer[LOG_BUFFER_SIZE - 1] = '\0';
        std::cout << m_LogBuffer;
    }
}