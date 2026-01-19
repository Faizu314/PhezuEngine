// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <string>

#include "Assets/Core/Asset.hpp"
#include "Assets/Blueprint/Blueprint.hpp"

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
