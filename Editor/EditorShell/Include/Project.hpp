#pragma once

#include <filesystem>
#include <vector>

namespace Phezu::Editor {

    struct Project {
        std::filesystem::path Path;
        std::vector<std::filesystem::path> ScriptFiles;
        std::vector<std::filesystem::path> SceneFiles;
        std::vector<std::filesystem::path> PrefabFiles;
        std::vector<std::filesystem::path> ConfigFiles;
    };
}