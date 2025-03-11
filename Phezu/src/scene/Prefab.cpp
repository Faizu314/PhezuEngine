#include "scene/Prefab.hpp"
#include "nlohmann/json.hpp"

namespace Phezu {
    
    Prefab::Prefab(GUID guid) {
        m_Guid = guid;
    }
    
    std::string Prefab::Serialize() const {
        nlohmann::json j;
        
        m_Blueprint.Serialize(j);

        return j.dump(4);
    }
    
    void Prefab::Deserialize(const std::string& data) {
        nlohmann::json j = nlohmann::json::parse(data);

        m_Blueprint.Deserialize(j);
    }
    
    std::vector<std::shared_ptr<Entity>> Prefab::Instantiate(std::shared_ptr<Scene> scene) const {
        return m_Blueprint.Instantiate(scene);
    }
}
