#pragma once

#include <string>
#include "Asset/Core/Asset.hpp"

namespace Phezu {

    struct MetaData {
        GUID Guid;
        AssetType Type;
        
        MetaData() = default;
        void Deserialize(const std::string& data);
    };
}
