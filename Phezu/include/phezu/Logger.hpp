#pragma once

#include <iostream>

#if _WIN32

#include <windows.h>

#endif

namespace Phezu {    

    void Log(const char* msg, ...);
}
