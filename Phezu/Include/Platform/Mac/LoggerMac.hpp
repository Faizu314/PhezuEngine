#pragma once

#include "Core/Logger.hpp"

namespace Phezu {    

    class LoggerMac : ILogger {
        void InitLogger() override;
        void Log(const char* msg, ...) override;
    };
}
