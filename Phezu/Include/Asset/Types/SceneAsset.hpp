#pragma once

#include <string>

#include "Asset/Core/Asset.hpp"
#include "Asset/Blueprint/Blueprint.hpp"

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