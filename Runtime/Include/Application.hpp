// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#if _WIN32
#include <windows.h>
#endif

#include <filesystem>

namespace Phezu::Runtime {
    
#if defined(PZ_PLATFORM_WINDOWS)

    std::filesystem::path GetExePath() {
        wchar_t path[MAX_PATH];
        GetModuleFileNameW(NULL, path, MAX_PATH);
        return std::filesystem::path(path).parent_path();
    }

    std::filesystem::path GetAssetsPath() {
        return GetExePath() / "Assets";
    }
    
    std::filesystem::path GetScriptCoreDllPath() {
        return GetExePath();
    }
    
    std::filesystem::path GetMonoCoreLibsPath() {
        return GetExePath() / "mono" / "lib" / "mono" / "4.5";
    }

#elif defined(PZ_PLATFORM_WEB)

    std::filesystem::path GetAssetsPath() {
        return std::filesystem::path() / "Assets";
    }

#endif
}
