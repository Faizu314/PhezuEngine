// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <string>
#include <vector>

#include "Assets/Core/Asset.hpp"

namespace Phezu {
    struct BuildScenesConfig {
        AssetHandle MasterScene;
        std::vector<AssetHandle> BuildScenes;
        
        BuildScenesConfig() = default;
        void Deserialize(const std::string& data);
    };
}
