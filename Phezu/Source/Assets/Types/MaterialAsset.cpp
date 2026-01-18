#include "Serialization/CustomSerialization.hpp"
#include "Assets/Types/MaterialAsset.hpp"

namespace Phezu {

	void MaterialAsset::Deserialize(const std::string& data)
	{
		nlohmann::json j = nlohmann::json::parse(data);

		ShaderRef = j["Shader"].get<AssetHandle>();
		Textures = j["Textures"].get<std::unordered_map<std::string, AssetHandle>>();
		Properties = j["Properties"].get<std::unordered_map<std::string, MaterialProperty>>();
	}

}