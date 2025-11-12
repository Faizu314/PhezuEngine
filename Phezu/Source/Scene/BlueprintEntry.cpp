#include "Scene/BlueprintEntry.hpp"

namespace Phezu {
    
    void BlueprintEntry::Serialize(nlohmann::json& j) const {
        j["FileID"] = FileID;
        j["TypeID"] = static_cast<uint16_t>(TypeID);
        j["TypeName"] = TypeName;
        j["Properties"] = Properties;
    }
    
    void BlueprintEntry::Deserialize(const nlohmann::json& j) {
        FileID = j["FileID"].get<uint64_t>();
        TypeID = static_cast<EntryType>(j["TypeID"].get<uint16_t>());
        TypeName = j["TypeName"].get<std::string>();
        Properties = j["Properties"].get<std::unordered_map<std::string, nlohmann::json>>();
    }
}
