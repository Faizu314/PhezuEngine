#pragma once

#if _WIN32
#include <windows.h>
#endif

#include <filesystem>
#include "RuntimeDefines.hpp"

namespace PhezuRuntime {
    
#if __APPLE__
        
    std::filesystem::path GetProjectPath(const char* argv[]) {
        return std::filesystem::path(argv[0]).parent_path().parent_path() / "Project";
    }
    
    std::filesystem::path GetExePath(const char* argv[]) {
        return std::filesystem::path(argv[0]).parent_path();
    }
    
    std::filesystem::path GetScriptCoreDllPath(const char* argv[]) {
        return GetExePath(argv) / "../MonoAssemblies";
    }
    
    std::filesystem::path GetMonoCoreLibsPath(const char* argv[]) {
        return GetExePath(argv) / "../MonoCoreLibs/mono";
    }
        
#elif _WIN32

    std::filesystem::path GetExePath() {
        wchar_t path[MAX_PATH];
        GetModuleFileNameW(NULL, path, MAX_PATH);
        return std::filesystem::path(path).parent_path();
    }

    std::filesystem::path GetProjectPath() {
        return GetExePath() / "Project";
    }
    
    std::filesystem::path GetScriptCoreDllPath() {
        return GetExePath();
    }
    
    std::filesystem::path GetMonoCoreLibsPath() {
        return GetExePath() / "mono" / "lib" / "4.5";
    }
    
#endif
}
