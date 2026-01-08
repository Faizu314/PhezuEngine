#pragma once

#include <vector>

#include "Asset/Core/Asset.hpp"
#include "Asset/Blueprint/Blueprint.hpp"

namespace Phezu {
    
    class PrefabAsset : public IAsset {
    public:
        AssetType GetAssetType() override { return AssetType::Prefab; }
        void Deserialize(const std::string& data) override;
    public:
        const Blueprint& GetBlueprint() const { return m_Blueprint; }
    private:
        Blueprint m_Blueprint;
    };
    
}
