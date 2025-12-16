#pragma once

#include <string>
#include "Core/Types/GUID.hpp"

namespace Phezu {
    struct MetaData {
        GUID Guid;
        
        MetaData() = default;
        std::string Serialize() const;
        void Deserialize(const std::string& data);
    };
}
