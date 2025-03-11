#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "scene/BlueprintEntry.hpp"
#include "nlohmann/json.hpp"

namespace Phezu {
    
    class Entity;
    class ShapeData;
    class Scene;
    
    class Blueprint {
    public:
        Blueprint() = default;
        Blueprint(std::vector<std::shared_ptr<Entity>> entities);
    public:
        Blueprint& operator=(const Blueprint& other) = delete;
    public:
        std::vector<BlueprintEntry> Entries;
    public:
        void Serialize(nlohmann::json& j) const;
        void Deserialize(const nlohmann::json& j);
        //How do we ensure a unique file gets created for a unique list of entities
    public:
        std::vector<std::shared_ptr<Entity>> Instantiate(std::shared_ptr<Scene> scene) const;
    };
}
