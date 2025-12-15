#include "AssetManagement/SceneAsset.hpp"
#include "nlohmann/json.hpp"

namespace Phezu {

    void SceneAsset::Deserialize(const std::string& data) {
        nlohmann::json j = nlohmann::json::parse(data);

        m_Name = j["Name"].get<std::string>();
        m_Blueprint.Deserialize(j);
    }
}