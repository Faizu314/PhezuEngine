#pragma once

#include "Core/Types/TextureTypes.hpp"
#include "Asset/Core/Asset.hpp"

namespace Phezu {

	class ImageAsset : public IAsset {
	public:
		ImageAsset(GUID guid) : IAsset(guid) {}
	public:
		void Deserialize(const std::string& data) override;
	public:
		unsigned char* Data = nullptr;
		int Width = 0;
		int Height = 0;
	};

}
