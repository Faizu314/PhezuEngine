#include "nlohmann/json.hpp"
#include "Asset/Core/MetaData.hpp"

namespace Phezu {

    void MetaData::Deserialize(const std::string& data) {
        nlohmann::json j = nlohmann::json::parse(data);

        Guid.Value = j["Guid"];
    }
}
