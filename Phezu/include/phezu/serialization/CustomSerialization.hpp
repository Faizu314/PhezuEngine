#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "maths/Math.hpp"
#include "Renderer.hpp"
#include "nlohmann/json.hpp"

namespace Phezu {
    
    struct EntryRef {
        uint64_t Guid;
        uint64_t InstanceID;
        uint64_t FileID;
    };
    using EntryOverrides = std::unordered_map<std::string, nlohmann::json>;
    struct PrefabOverrides {
        std::unordered_set<uint64_t> RemovedEntities; //A file will only save the entities it removed
        std::unordered_set<uint64_t> RemovedComponents; //A file will only save the components it removed
        std::unordered_map<uint64_t, EntryOverrides> EntryOverrides;
    };
    
    void from_json(const nlohmann::json& j, EntryRef& v);
    
    void to_json(nlohmann::json& j, const EntryRef& v);
    
    
    
    void from_json(const nlohmann::json& j, PrefabOverrides& obj);
    
    void to_json(nlohmann::json& j, const PrefabOverrides& obj);
    
    
    
    void from_json(const nlohmann::json& j, Vector2& v);

    void to_json(nlohmann::json& j, const Vector2& v);

    
    
    void from_json(const nlohmann::json& j, Color& c);

    void to_json(nlohmann::json& j, const Color& c);
    
    
    
    void from_json(const nlohmann::json& j, Rect& r);

    void to_json(nlohmann::json& j, const Rect& r);
}
