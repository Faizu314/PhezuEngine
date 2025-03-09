#pragma once

#include <unordered_map>
#include <string>

namespace Phezu {
    
    enum class EntryType : uint16_t {
        Invalid = 0,
        Entity = 1,
        TransformData = 2,
        ShapeData = 3,
        RenderData = 4,
        PhysicsData = 5,
        BehaviourComponent = 6,
        PrefabRef = 100,
        Stripped = 101,
    };
    
    class BlueprintEntry {
    public:
        BlueprintEntry(uint64_t guid, uint64_t fileID, EntryType typeID, std::string typeName)
        : Guid(guid), FileID(fileID), TypeID(typeID), TypeName(typeName) {}
    public:
        uint64_t Guid;
        uint64_t FileID;
        EntryType TypeID;
        std::string TypeName;
    public:
        std::unordered_map<std::string, std::string> Properties;
    };
}
