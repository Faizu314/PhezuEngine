#include "nlohmann/json.hpp"

#include "Asset/Types/PrefabAsset.hpp"

namespace Phezu {
    
    void PrefabAsset::Deserialize(const std::string& data) {
        nlohmann::json j = nlohmann::json::parse(data);

        m_Blueprint.Deserialize(j);
    }
}
