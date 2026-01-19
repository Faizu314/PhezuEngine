// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <string>

#include "Assets/Core/Asset.hpp"
#include "Core/Types/Types.hpp"

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