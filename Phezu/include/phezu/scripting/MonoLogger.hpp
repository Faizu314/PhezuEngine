#pragma once

#include <thread>
#include <atomic>

#if defined(_WIN32)
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace Phezu {

    class MonoLogger {
    public:
        void Start();
        void Stop();
    private:
        void OnLog(const char* msg);
    private:
        std::thread m_ReadThread;
        std::atomic<bool> m_Running{ false };
#if defined(_WIN32)
        HANDLE m_PipeRead;
#elif __APPLE__
        int m_PipeRead;
#endif
    };

}