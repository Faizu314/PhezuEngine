#pragma once

#include <string>
#include <vector>
#include "Asset/Core/Asset.hpp"

namespace Phezu {
    struct BuildScenesConfig {
        AssetHandle MasterScene;
        std::vector<AssetHandle> BuildScenes;
        
        BuildScenesConfig() = default;
        void Deserialize(const std::string& data);
    };
}
