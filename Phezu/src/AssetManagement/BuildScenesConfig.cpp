#include "AssetManagement/BuildScenesConfig.hpp"
#include "nlohmann/json.hpp"

namespace Phezu {

    std::string BuildScenesConfig::Serialize() const {
        nlohmann::json j;
        
        j["MasterScene"] = MasterScene.Value;
        j["BuildScenes"] = nlohmann::json::array();
        
        for (const auto& guid : BuildScenes) {
            nlohmann::json guidJson;
            guidJson["Guid"] = guid.Value;
            j["BuildScenes"].push_back(guidJson);
        }

        return j.dump(4);
    }
    
    void BuildScenesConfig::Deserialize(const std::string& data) {
        nlohmann::json j = nlohmann::json::parse(data);

        MasterScene.Value = j["MasterScene"];
        BuildScenes.clear();
        
        for (const auto& guidJson : j["BuildScenes"]) {
            GUID guid;
            guid.Value = guidJson["Guid"];
            
            BuildScenes.push_back(guid);
        }
    }
    
}
