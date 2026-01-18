#pragma once

#include <unordered_map>
#include <string>

#include "Assets/Core/Asset.hpp"
#include "Core/Types/MaterialTypes.hpp"

namespace Phezu {

	class MaterialAsset : public IAsset {
	public:
		AssetType GetAssetType() override { return AssetType::Material; }
		void Deserialize(const std::string& data) override;
	public:
		AssetHandle ShaderRef;
		std::unordered_map<std::string, AssetHandle> Textures;
		std::unordered_map<std::string, MaterialProperty> Properties;
	};

}