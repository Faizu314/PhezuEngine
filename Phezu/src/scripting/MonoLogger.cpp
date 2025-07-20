#include "scripting/MonoLogger.hpp"
#include "Logger.hpp"

#include "mono/utils/mono-logger.h"

#if defined(_WIN32)
#include <cstdio>
#include <cstring>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#endif

namespace Phezu {

    void MonoLog(const char* log_domain, const char* log_level, const char* message, mono_bool fatal, void* userData) {
        Log("Domain : %s\n", log_domain);
        Log("Level  : %s\n", log_level);
        Log("Message: %s\n", message);
        Log("Fatal  : %s\n\n", fatal ? "true" : "false");
    }
    
#if defined(_WIN32)
    static constexpr size_t BUFFER_SIZE = 512;

    void MonoLogger::Init() {
        mono_trace_set_level_string("debug");
        mono_trace_set_log_handler(MonoLog, nullptr);
        
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

    void MonoLogger::Destroy() {
        m_Running = false;
        if (m_ReadThread.joinable()) m_ReadThread.join();
    }

    void MonoLogger::OnLog(const char* msg) {
        Log("Mono: %s\n", msg);
    }
#else
    void MonoLogger::Init() {
        mono_trace_set_level_string("debug");
        mono_trace_set_log_handler(MonoLog, nullptr);
    }
    
    void MonoLogger::Destroy() {
        
    }
#endif
}
