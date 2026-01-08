#include "Serialization/CustomSerialization.hpp"
#include "Asset/Types/TextureAsset.hpp"

namespace Phezu {

	void TextureAsset::Deserialize(const std::string& data) {
		nlohmann::json j = nlohmann::json::parse(data);

		ImageRef = j["Image"].get<AssetHandle>();
		FilteringMode = ToTextureFilteringMode(j["FilteringMode"].get<std::string>());
		WrapMode = ToTextureWrapMode(j["WrapMode"].get<std::string>());
	}

}