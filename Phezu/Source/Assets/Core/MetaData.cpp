#include "nlohmann/json.hpp"

#include "Core/Types/Types.hpp"
#include "Assets/Core/MetaData.hpp"

namespace Phezu {

    void MetaData::Deserialize(const std::string& data) {
        nlohmann::json j = nlohmann::json::parse(data);

        Guid.Value = j["Guid"];
        Type = ToAssetType(j["Type"].get<std::string>());
    }
}
