#pragma once

#include "Core/Types/Types.hpp"
#include "Assets/Types/ImageAsset.hpp"

namespace Phezu {

	class TextureAsset : public IAsset {
	public:
		AssetType GetAssetType() override { return AssetType::Texture; }
		void Deserialize(const std::string& data) override;
	public:
		AssetHandle ImageRef;
		TextureFilteringMode FilteringMode = TextureFilteringMode::Point;
		TextureWrapMode WrapMode = TextureWrapMode::Repeat;
	};

}