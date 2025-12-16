#include "Asset/Configs/BuildScenesConfig.hpp"
#include "nlohmann/json.hpp"

namespace Phezu {
    
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
