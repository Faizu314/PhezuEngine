#include "scene/Blueprint.hpp"
#include "scene/Scene.hpp"
#include "scene/Entity.hpp"
#include "scene/components/ShapeData.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/PhysicsData.hpp"
#include "serialization/Serializer.hpp"

namespace Phezu {
    
    Blueprint::Blueprint() {}
    
    Blueprint::~Blueprint() {}
    
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
                    
                    uint64_t parentFileID = Json::DeserializeRaw<uint64_t>(entry.Properties.at("Parent"));
                    if (parentFileID == 0)
                        rootEntities.push_back(entity);
                    
                    std::vector<uint64_t> children = Json::DeserializeArray<uint64_t>(entry.Properties.at("Children"));
                    for (size_t j = 0; j < children.size(); j++) {
                        uint64_t childFileID = children[j];
                        entities[childFileID]->SetParent(entity);
                    }
                    break;
                }
                case EntryType::TransformData:
                {
                    uint64_t entityFileID = Json::DeserializeRaw<uint64_t>(entry.Properties.at("Entity"));
                    Vector2 position = Json::DeserializeRaw<Vector2>(entry.Properties.at("Position"));
                    Vector2 scale = Json::DeserializeRaw<Vector2>(entry.Properties.at("Scale"));
                    
                    auto transform = entities[entityFileID]->GetTransformData();
                    transform->SetLocalPosition(position);
                    transform->SetLocalScale(scale);
                    
                    break;
                }
                case EntryType::ShapeData:
                {
                    uint64_t entityFileID = Json::DeserializeRaw<uint64_t>(entry.Properties.at("Entity"));
                    Vector2 pivot = Json::DeserializeRaw<Vector2>(entry.Properties.at("Pivot"));
                    Vector2 size = Json::DeserializeRaw<Vector2>(entry.Properties.at("Size"));
                    
                    auto shapeData = entities[entityFileID]->AddShapeData();
                    shapeData->Set(pivot, size);

                    break;
                }
                case EntryType::RenderData:
                {
                    uint64_t entityFileID = Json::DeserializeRaw<uint64_t>(entry.Properties.at("Entity"));
                    Color tint = Json::DeserializeRaw<Color>(entry.Properties.at("Tint"));
                    Rect sourceRect = Json::DeserializeRaw<Rect>(entry.Properties.at("SourceRect"));
                    //Texture tex = Json::DeserializeRaw<Vector2>(entry.Properties["Sprite"]);
                    
                    auto renderData = entities[entityFileID]->AddRenderData();
                    renderData->Tint = tint;
                    renderData->SourceRect = sourceRect;
                    //renderData->Sprite = tex;
                    
                    break;
                }
                case EntryType::PhysicsData:
                {
                    uint64_t entityFileID = Json::DeserializeRaw<uint64_t>(entry.Properties.at("Entity"));
                    bool isStatic = Json::DeserializeRaw<bool>(entry.Properties.at("IsStatic"));
                    Vector2 velocity = Json::DeserializeRaw<Vector2>(entry.Properties.at("Velocity"));
                    
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
