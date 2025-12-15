#include "Scene/Prefab.hpp"
#include "nlohmann/json.hpp"

namespace Phezu {
    
    void Prefab::Deserialize(const std::string& data) {
        nlohmann::json j = nlohmann::json::parse(data);

        m_Blueprint.Deserialize(j);
    }
}
