#pragma once

#include <string>
#include "Core/Types/GUID.hpp"

namespace Phezu {
    struct MetaData {
        GUID Guid;
        
        MetaData() = default;
        void Deserialize(const std::string& data);
    };
}
