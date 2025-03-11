#include "scene/Blueprint.hpp"
#include "scene/Scene.hpp"
#include "scene/Entity.hpp"
#include "scene/components/ShapeData.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/PhysicsData.hpp"
#include "nlohmann/json.hpp"
#include "serialization/CustomSerialization.hpp"

namespace Phezu {
    
    void Blueprint::Serialize(nlohmann::json &j) const {
        j["Entries"] = nlohmann::json::array();
        
        for (const auto& entry : Entries) {
            nlohmann::json entryJson;
            entry.Serialize(entryJson);
            j["Entries"].push_back(entryJson);
        }
    }
    
    void Blueprint::Deserialize(const nlohmann::json& j) {
        Entries.clear();
        
        for (const auto& entryJson : j["Entries"]) {
            BlueprintEntry entry;
            entry.Deserialize(entryJson);
            Entries.push_back(entry);
        }
    }
    
    std::vector<std::shared_ptr<Entity>> Blueprint::Instantiate(std::shared_ptr<Scene> scene) const {
        std::unordered_map<uint64_t, std::shared_ptr<Entity>> entities;
        std::unordered_map<uint64_t, std::shared_ptr<DataComponent>> components;
        
        std::vector<std::shared_ptr<Entity>> rootEntities;
        
        /*-----– First Pass -------*/
        
        for (size_t i = 0; i < Entries.size(); i++) {
            const BlueprintEntry& entry = Entries[i];
            
            if (entry.TypeID == EntryType::Entity) {
                auto entity = scene->CreateEntity().lock();
                entities.insert(std::make_pair(entry.FileID, entity));
                
                entity->SetTag(entry.Properties.at("Tag"));
            }
        }
        
        /*-----– Second Pass -------*/
        
        for (size_t i = 0; i < Entries.size(); i++) {
            const BlueprintEntry& entry = Entries[i];
            
            switch (entry.TypeID) {
                case EntryType::Entity:
                {
                    auto entity = entities[entry.FileID];
                    
                    uint64_t parentFileID = nlohmann::json::parse(entry.Properties.at("Parent")).get<uint64_t>();
                    if (parentFileID == 0)
                        rootEntities.push_back(entity);
                    
                    std::vector<uint64_t> children = nlohmann::json::parse(entry.Properties.at("Children")).get<std::vector<uint64_t>>();
                    for (size_t j = 0; j < children.size(); j++) {
                        uint64_t childFileID = children[j];
                        entities[childFileID]->SetParent(entity);
                    }
                    break;
                }
                case EntryType::TransformData:
                {
                    uint64_t entityFileID = nlohmann::json::parse(entry.Properties.at("Entity")).get<uint64_t>();
                    Vector2 position = nlohmann::json::parse(entry.Properties.at("Position")).get<Vector2>();
                    Vector2 scale = nlohmann::json::parse(entry.Properties.at("Scale")).get<Vector2>();
                    
                    auto transform = entities[entityFileID]->GetTransformData();
                    transform->SetLocalPosition(position);
                    transform->SetLocalScale(scale);
                    
                    break;
                }
                case EntryType::ShapeData:
                {
                    uint64_t entityFileID = nlohmann::json::parse(entry.Properties.at("Entity")).get<uint64_t>();
                    Vector2 pivot = nlohmann::json::parse(entry.Properties.at("Pivot")).get<Vector2>();
                    Vector2 size = nlohmann::json::parse(entry.Properties.at("Size")).get<Vector2>();
                    
                    auto shapeData = entities[entityFileID]->AddShapeData();
                    shapeData->Set(pivot, size);

                    break;
                }
                case EntryType::RenderData:
                {
                    uint64_t entityFileID = nlohmann::json::parse(entry.Properties.at("Entity")).get<uint64_t>();
                    Color tint = nlohmann::json::parse(entry.Properties.at("Tint")).get<Color>();
                    Rect sourceRect = nlohmann::json::parse(entry.Properties.at("SourceRect")).get<Rect>();
                    //Texture tex = Json::DeserializeRaw<Vector2>(entry.Properties["Sprite"]);
                    
                    auto renderData = entities[entityFileID]->AddRenderData();
                    renderData->Tint = tint;
                    renderData->SourceRect = sourceRect;
                    //renderData->Sprite = tex;
                    
                    break;
                }
                case EntryType::PhysicsData:
                {
                    uint64_t entityFileID = nlohmann::json::parse(entry.Properties.at("Entity")).get<uint64_t>();
                    bool isStatic = nlohmann::json::parse(entry.Properties.at("IsStatic")).get<bool>();
                    Vector2 velocity = nlohmann::json::parse(entry.Properties.at("Velocity")).get<Vector2>();
                    
                    auto physicsData = entities[entityFileID]->AddPhysicsData(isStatic).lock();
                    physicsData->Velocity = velocity;
                    
                    break;
                }
                    
                default:
                    break;
            }
        }
        
        return rootEntities;
    }
}
