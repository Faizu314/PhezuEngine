#include "scene/Prefab.hpp"
#include "nlohmann/json.hpp"

namespace Phezu {
    
    Prefab::Prefab(Engine* engine, GUID guid) {
        m_Engine = engine;
        m_Guid = guid;
    }
    
    std::string Prefab::Serialize() const {
        nlohmann::json j;
        
        j["Guid"] = m_Guid.Value;
        m_Blueprint.Serialize(j);

        return j.dump(4);
    }
    
    void Prefab::Deserialize(const std::string& data) {
        nlohmann::json j = nlohmann::json::parse(data);

        m_Guid.Value = j["Guid"].get<uint64_t>();
        m_Blueprint.Deserialize(j);
        m_Blueprint.Initialize(m_Engine, m_Guid);
    }
    
    Entity* Prefab::Instantiate(Scene* scene) const {
        return m_Blueprint.Instantiate(scene);
    }
}
