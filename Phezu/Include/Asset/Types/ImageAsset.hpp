#pragma once

#include "Core/Types/TextureTypes.hpp"
#include "Asset/Core/Asset.hpp"

namespace Phezu {

	class ImageAsset : public IAsset {
	public:
		void Deserialize(const std::string& data) override;
	public:
		unsigned char* Data;
		int Width;
		int Height;
	};

}
