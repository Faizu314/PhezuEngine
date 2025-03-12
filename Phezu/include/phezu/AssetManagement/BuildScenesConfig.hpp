#pragma once

#include <string>
#include <vector>
#include "GUID.hpp"

namespace Phezu {
    struct BuildScenesConfig {
        std::vector<GUID> BuildScenes;
        
        BuildScenesConfig() = default;
        std::string Serialize() const;
        void Deserialize(const std::string& data);
    };
}
