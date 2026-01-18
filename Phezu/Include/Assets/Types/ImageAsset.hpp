#pragma once

#include "Core/Types/Types.hpp"
#include "Assets/Core/Asset.hpp"

namespace Phezu {

	class ImageAsset : public IAsset {
	public:
		~ImageAsset();
	public:
		AssetType GetAssetType() override { return AssetType::Image; }
		void Deserialize(const std::string& data) override;
	public:
		unsigned char* Data = nullptr;
		int Width = 0;
		int Height = 0;
	};

}
