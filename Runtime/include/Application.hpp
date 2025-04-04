#pragma once

#if _WIN32
#include <windows.h>
#endif

#include <filesystem>

namespace PhezuRuntime {
    
#if __APPLE__
        
    std::filesystem::path GetProjectPath(const char* argv[]) {
        return std::filesystem::path(argv[0]).parent_path().parent_path() / "Project";
    }
        
#elif _WIN32

    std::string GetExePath() {
        wchar_t path[MAX_PATH];
        GetModuleFileNameW(NULL, path, MAX_PATH);
        return std::filesystem::path(path).parent_path().string();
    }

    std::string GetProjectPath() {
        return GetExePath();
    }
    
#endif
}
