#include "scene/BlueprintEntry.hpp"

namespace Phezu {
    
    void BlueprintEntry::Serialize(nlohmann::json& j) const {
        j["Guid"] = Guid.Value;
        j["FileID"] = FileID;
        j["TypeID"] = static_cast<uint16_t>(TypeID);
        j["TypeName"] = TypeName;
        j["Properties"] = Properties;
    }
    
    void BlueprintEntry::Deserialize(const nlohmann::json& j) {
        Guid.Value = j["Guid"].get<uint64_t>();
        FileID = j["FileID"].get<uint64_t>();
        TypeID = static_cast<EntryType>(j["TypeID"].get<uint16_t>());
        TypeName = j["TypeName"].get<std::string>();
        Properties = j["Properties"].get<std::unordered_map<std::string, std::string>>();
    }
}
