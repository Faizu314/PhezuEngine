#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "Asset/Types/ImageAsset.hpp"

namespace Phezu {

	void ImageAsset::Deserialize(const std::string& data) {
		int channelsInFile = 0;
		Data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(data.data()), static_cast<int>(data.size()), &Width, &Height, &channelsInFile, 4);
	}

}