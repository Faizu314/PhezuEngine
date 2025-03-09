#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "scene/BlueprintEntry.hpp"

namespace Phezu {
    
    class Entity;
    class ShapeData;
    class Scene;
    
    class Blueprint {
    public:
        Blueprint();
        Blueprint(std::vector<std::shared_ptr<Entity>> entities);
        ~Blueprint();
    public:
        Blueprint& operator=(const Blueprint& other) = delete;
    public:
        std::vector<BlueprintEntry> Entries;
    public:
        //How do we ensure a unique file gets created for a unique list of entities
    public:
        std::vector<std::shared_ptr<Entity>> Instantiate(std::shared_ptr<Scene> scene) const;
    };
}
