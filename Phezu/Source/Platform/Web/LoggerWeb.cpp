#include <iostream>

#include "Platform/Web/LoggerWeb.hpp"

namespace Phezu {

    void LoggerWeb::Init() {
        std::memset(m_LogBuffer, 0, LOG_BUFFER_SIZE);
    }

    void LoggerWeb::Destroy() {
        fflush(stdout);
    }

    void LoggerWeb::Log(const char* msg, va_list args) {
        vsnprintf(m_LogBuffer, LOG_BUFFER_SIZE, msg, args);
        m_LogBuffer[LOG_BUFFER_SIZE - 1] = '\0';
        std::fputs(m_LogBuffer, stdout);
        std::fflush(stdout);
    }
}