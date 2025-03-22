#include "serialization/CustomSerialization.hpp"

namespace Phezu {
    
    void from_json(const nlohmann::json& j, EntryRef& v) {
        v.Guid = j.value("Guid", 0);
        v.FileID = j.value("FileID", 0);
    }
    
    void to_json(nlohmann::json& j, const EntryRef& v) {
        j = nlohmann::json{{"Guid", v.Guid}, {"FileID", v.FileID}};
    }
    
    
    
    void from_json(const nlohmann::json& j, EntryOverrides& obj) {
        j.at("PropertyOverrides").get_to(obj.PropertyOverrides);
    }
    
    void to_json(nlohmann::json& j, const EntryOverrides& obj) {
        j = nlohmann::json{{"PropertyOverrides", obj.PropertyOverrides}};
    }
    
    
    
    void from_json(const nlohmann::json& j, PrefabOverrides& obj) {
        j.at("RemovedEntities").get_to(obj.RemovedEntities);
        j.at("RemovedComponents").get_to(obj.RemovedComponents);
        j.at("EntryOverrides").get_to(obj.EntryOverrides);
    }
    
    void to_json(nlohmann::json& j, const PrefabOverrides& obj) {
        j = nlohmann::json{
            {"RemovedEntities", obj.RemovedEntities},
            {"RemovedComponents", obj.RemovedComponents},
            {"EntryOverrides", obj.EntryOverrides}
        };
    }
    
    
    
    void from_json(const nlohmann::json& j, Vector2& v) {
        v.Set(j.value("x", 0.0f), j.value("y", 0.0f));
    }

    void to_json(nlohmann::json& j, const Vector2& v) {
        j = nlohmann::json{{"x", v.X()}, {"y", v.Y()}};
    }

    
    
    void from_json(const nlohmann::json& j, Color& c) {
        c.r = j.value("r", 0);
        c.g = j.value("g", 0);
        c.b = j.value("b", 0);
        c.a = j.value("a", 255);
    }

    void to_json(nlohmann::json& j, const Color& c) {
        j = nlohmann::json{{"r", c.r}, {"g", c.g}, {"b", c.b}, {"a", c.a}};
    }
    
    
    
    void from_json(const nlohmann::json& j, Rect& r) {
        r.x = j.value("X", 0);
        r.y = j.value("Y", 0);
        r.w = j.value("Width", 0);
        r.h = j.value("Height", 0);
    }

    void to_json(nlohmann::json& j, const Rect& r) {
        j = nlohmann::json{{"X", r.x}, {"Y", r.y}, {"Width", r.w}, {"Height", r.h}};
    }
}
