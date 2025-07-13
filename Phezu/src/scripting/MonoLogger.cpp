#if defined(_WIN32)

#include "scripting/MonoLogger.hpp"
#include "Logger.hpp"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <io.h>
#include <fcntl.h>

namespace Phezu {

    static constexpr size_t BUFFER_SIZE = 512;

    void MonoLogger::Start() {
        m_Running = true;

        // Create pipe
        SECURITY_ATTRIBUTES saAttr = {};
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;
        saAttr.lpSecurityDescriptor = NULL;

        HANDLE readHandle, writeHandle;
        CreatePipe(&readHandle, &writeHandle, &saAttr, 0);
        SetStdHandle(STD_OUTPUT_HANDLE, writeHandle);
        SetStdHandle(STD_ERROR_HANDLE, writeHandle);

        // Redirect C stdout
        int fd = _open_osfhandle((intptr_t)writeHandle, _O_TEXT);
        FILE* newStdout = _fdopen(fd, "w");
        *stdout = *newStdout;
        setvbuf(stdout, NULL, _IONBF, 0);

        m_PipeRead = readHandle;

        m_ReadThread = std::thread([this]() {
            char buffer[BUFFER_SIZE];
            while (m_Running) {
                DWORD bytesRead;
                if (ReadFile(m_PipeRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
                    buffer[bytesRead] = '\0';
                    OnLog(buffer);
                }
            }
        });
    }

    void MonoLogger::Stop() {
        m_Running = false;
        if (m_ReadThread.joinable()) m_ReadThread.join();
    }

    void MonoLogger::OnLog(const char* msg) {
        Log("Mono: %s\n", msg);
    }
}

#endif
