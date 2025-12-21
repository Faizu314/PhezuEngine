#include "nlohmann/json.hpp"

#include "Core/Platform.hpp"
#include "Asset/Blueprint/Blueprint.hpp"

namespace Phezu {
    
    void Blueprint::Deserialize(const nlohmann::json& j) {
        EntityEntries.clear();
        PrefabEntries.clear();
        ComponentEntries.clear();
        ScriptEntries.clear();
        
        for (const auto& entryJson : j["Entries"]) {
            BlueprintEntry entry;
            entry.Deserialize(entryJson);

            switch (entry.TypeID) {
                case EntryType::Entity:
                    EntityEntries.push_back(entry);
                    break;
                case EntryType::TransformData:
                    ComponentEntries.push_back(entry);
                    break;
                case EntryType::ShapeData:
                    ComponentEntries.push_back(entry);
                    break;
                case EntryType::RenderData:
                    ComponentEntries.push_back(entry);
                    break;
                case EntryType::PhysicsData:
                    ComponentEntries.push_back(entry);
                    break;
                case EntryType::Script:
                    ScriptEntries.push_back(entry);
                    break;
                case EntryType::PrefabRef:
                    PrefabEntries.push_back(entry);
                    break;
                case EntryType::Invalid:
                    Log("Assert here\n");
                    break;
            }
        }
    }
}
