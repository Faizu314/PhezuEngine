#pragma once

#include <unordered_map>
#include <string>
#include "GUID.hpp"
#include "nlohmann/json.hpp"

namespace Phezu {
    
    enum class EntryType : uint16_t {
        Invalid = 0,
        
        Entity = 1,
        
        // Engine Components
        TransformData = 2,
        ShapeData = 3,
        RenderData = 4,
        PhysicsData = 5,
        Camera = 6,
        
        Script = 101,
        PrefabRef = 102,
    };
    
    class BlueprintEntry {
    public:
        BlueprintEntry() = default;
        BlueprintEntry(uint64_t fileID, EntryType typeID, std::string typeName)
        : FileID(fileID), TypeID(typeID), TypeName(typeName) {}
    public:
        uint64_t FileID;
        EntryType TypeID;
        std::string TypeName;
    public:
        std::unordered_map<std::string, nlohmann::json> Properties;
    public:
        void Serialize(nlohmann::json& j) const;
        void Deserialize(const nlohmann::json& j);
    };
}
