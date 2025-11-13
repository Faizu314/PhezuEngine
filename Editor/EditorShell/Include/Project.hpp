#pragma once

#include <filesystem>
#include <vector>

namespace Phezu::Editor {

    struct Project {
        std::filesystem::path Path;
        std::vector<std::filesystem::path> Scripts;
    };
}