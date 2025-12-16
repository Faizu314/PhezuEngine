#include "Asset/Blueprint/Blueprint.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components/ShapeData.hpp"
#include "Scene/Components/RenderData.hpp"
#include "Scene/Components/PhysicsData.hpp"
#include "Scripting/ScriptInstance.hpp"
#include "nlohmann/json.hpp"
#include "Asset/Types/PrefabAsset.hpp"
#include "Core/Engine.hpp"
#include "Core/Types/Color.hpp"

namespace Phezu {
    
    void Blueprint::Deserialize(const nlohmann::json& j) {
        EntityEntries.clear();
        PrefabEntries.clear();
        ComponentEntries.clear();
        ScriptEntries.clear();
        
        m_Guid.Value = j["Guid"].get<uint64_t>();
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
