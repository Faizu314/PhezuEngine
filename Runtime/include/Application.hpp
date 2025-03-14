#pragma once

#include <filesystem>

namespace PhezuRuntime {
    
#if __APPLE__
        
    std::filesystem::path GetExePath(const char* argv[]) {
        return std::filesystem::path(argv[0]);
    }
        
#elif _WIN32
        
#include <windows.h>
#include <iostream>
        
    std::filesystem::path GetExePath() {
        wchar_t path[MAX_PATH];
        GetModuleFileNameW(NULL, path, MAX_PATH);
        return std::filesystem::path(path);
    }
    
#endif
}
