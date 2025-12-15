#pragma once

#include <vector>
#include "AssetManagement/Blueprint.hpp"
#include "Core/Types/GUID.hpp"

namespace Phezu {
    
    class Prefab {
    public:
        Prefab() = default;
    public:
        GUID GetGuid() { return m_Guid; }
    public:
        void Deserialize(const std::string& data);
    public:
        const Blueprint& GetBlueprint() const { return m_Blueprint; }
    private:
        GUID m_Guid;
        Blueprint m_Blueprint;
    };
    
}
