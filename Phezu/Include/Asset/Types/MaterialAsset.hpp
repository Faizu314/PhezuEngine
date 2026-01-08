#pragma once

#include <unordered_map>
#include <variant>

#include "Asset/Types/ShaderAsset.hpp"
#include "Core/Types/Types.hpp"
#include "Core/Types/Color.hpp"
#include "Maths/Objects/Vector2.hpp"
#include "Maths/Objects/Vector3.hpp"

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