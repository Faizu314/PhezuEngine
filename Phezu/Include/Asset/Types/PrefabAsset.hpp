#pragma once

#include <vector>
#include "Asset/Blueprint/Blueprint.hpp"
#include "Core/Types/GUID.hpp"

namespace Phezu {
    
    class PrefabAsset {
    public:
        PrefabAsset() = default;
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
