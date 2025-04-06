#include "scripting/MonoLogger.hpp"
#include "Logger.hpp"

#include <cstdio>
#include <cstring>
#include <iostream>

#if defined(_WIN32)
#include <io.h>
#include <fcntl.h>
#endif

namespace Phezu {

    static constexpr size_t BUFFER_SIZE = 512;

    void MonoLogger::Start() {
        m_Running = true;

#if defined(_WIN32)
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
#elif __APPLE__
        int pipefd[2];
        pipe(pipefd);

        // Redirect stdout
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);

        m_PipeRead = pipefd[0];
#endif

        m_ReadThread = std::thread([this]() {
            char buffer[BUFFER_SIZE];
            while (m_Running) {
#if defined(_WIN32)
                DWORD bytesRead;
                if (ReadFile(m_PipeRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
                    buffer[bytesRead] = '\0';
                    OnLog(buffer);
                }
#elif __APPLE__
                ssize_t bytesRead = read(m_PipeRead, buffer, sizeof(buffer) - 1);
                if (bytesRead > 0) {
                    buffer[bytesRead] = '\0';
                    OnLog(buffer);
                }
#endif
            }
        });
    }

    void MonoLogger::Stop() {
        m_Running = false;
        if (m_ReadThread.joinable()) m_ReadThread.join();
    }

    void MonoLogger::OnLog(const char* msg) {
        Log("PhezuScriptCore: %s\n", msg);
    }
}
