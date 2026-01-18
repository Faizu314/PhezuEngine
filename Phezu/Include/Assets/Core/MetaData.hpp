#pragma once

#include <string>
#include "Assets/Core/Asset.hpp"

namespace Phezu {

    struct MetaData {
        GUID Guid;
        AssetType Type = AssetType::None;
        
        MetaData() = default;
        void Deserialize(const std::string& data);
    };
}
