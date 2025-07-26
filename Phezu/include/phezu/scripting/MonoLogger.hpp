#pragma once

#if defined(_WIN32)
#include <thread>
#include <atomic>
#include <Windows.h>
#endif

namespace Phezu {

    class MonoLogger {
    public:
        void Init();
        void Destroy();
        
#if defined(_WIN32)
    private:
        void OnLog(const char* msg);
    private:
        std::thread m_ReadThread;
        std::atomic<bool> m_Running{ false };
        HANDLE m_PipeRead;
#endif
        
    };
}
