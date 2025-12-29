#include "nlohmann/json.hpp"

#include "Asset/Types/TextureAsset.hpp"

namespace Phezu {

	void TextureAsset::Deserialize(const std::string& data) {
		nlohmann::json j = nlohmann::json::parse(data);

		ImageRef = j["Image"].get<uint64_t>();

	}

}