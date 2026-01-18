#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <vector>

#include "Assets/Types/ImageAsset.hpp"

namespace Phezu {

	ImageAsset::~ImageAsset() {
		if (Data != nullptr) {
			delete[] Data;
			Data = nullptr;
		}
	}

	void ImageAsset::Deserialize(const std::string& data) {
		int channelsInFile = 0;
		stbi_set_flip_vertically_on_load(true);

		Data = stbi_load_from_memory(
			reinterpret_cast<const stbi_uc*>(data.data()),
			static_cast<int>(data.size()),
			&Width,
			&Height,
			&channelsInFile,
			4
		);
	}

}