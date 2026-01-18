#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "nlohmann/json.hpp"
#include "Assets/Blueprint/BlueprintEntry.hpp"

namespace Phezu {
    
    class Scene;
    class Engine;
    
    class Blueprint {
    public:
        Blueprint() = default;
    public:
        Blueprint(const Blueprint& other) = delete;
        Blueprint(const Blueprint&& other) = delete;
        Blueprint& operator=(const Blueprint& other) = delete;
    public:
        std::vector<BlueprintEntry> EntityEntries;
        std::vector<BlueprintEntry> PrefabEntries;
        std::vector<BlueprintEntry> ComponentEntries;
        std::vector<BlueprintEntry> ScriptEntries;
    public:
        void Deserialize(const nlohmann::json& j);
    };
}
