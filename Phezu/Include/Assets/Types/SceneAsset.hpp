// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <string>

#include "Assets/Core/Asset.hpp"
#include "Assets/Blueprint/Blueprint.hpp"

namespace Phezu {

	class SceneAsset : public IAsset {
    public:
        AssetType GetAssetType() override { return AssetType::Scene; }
        void Deserialize(const std::string& data) override;
    public:
        const Blueprint& GetBlueprint() const { return m_Blueprint; }
        std::string GetName() const { return m_Name; }
	private:
        std::string m_Name;
		Blueprint m_Blueprint;
	};

}