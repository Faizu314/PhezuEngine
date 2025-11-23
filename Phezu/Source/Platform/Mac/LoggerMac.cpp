#include "Platform/LoggerMac.hpp"

#include <iostream>

namespace Phezu {

    void LoggerMac::InitLogger() override {

    }

    void LoggerMac::Log(const char* msg, ...) override {
        va_list args;

        va_start(args, msg);
        vprintf(msg, args);
        va_end(args);
    }
}