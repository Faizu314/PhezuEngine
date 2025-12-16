#include "Asset/Core/MetaData.hpp"
#include "nlohmann/json.hpp"

namespace Phezu {

    std::string MetaData::Serialize() const {
        nlohmann::json j;
        
        j["Guid"] = Guid.Value;

        return j.dump(4);
    }
    
    void MetaData::Deserialize(const std::string& data) {
        nlohmann::json j = nlohmann::json::parse(data);

        Guid.Value = j["Guid"];
    }
}
