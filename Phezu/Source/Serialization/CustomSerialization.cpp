#include "Core/Platform.hpp"
#include "Serialization/CustomSerialization.hpp"
#include "Maths/Objects/Vector2.hpp"
#include "Maths/Objects/Vector3.hpp"
#include "Core/Types/Color.hpp"

namespace Phezu {

    void from_json(const nlohmann::json& j, GUID& guid) {
        guid.Value = j.get<uint64_t>();
    }

    void to_json(nlohmann::json& j, const GUID& guid) {
        j = guid.Value;
    }

    void from_json(const nlohmann::json& j, EntryRef& v) {
        v.Guid = j.value("Guid", 0);
        v.InstanceID = j.value("InstanceID", 0);
        v.FileID = j.value("FileID", 0);
    }
    
    void to_json(nlohmann::json& j, const EntryRef& v) {
        j = nlohmann::json{{"Guid", v.Guid}, {"FileID", v.InstanceID}, {"EntityFileID", v.FileID}};
    }
    
    
    
    void from_json(const nlohmann::json& j, PrefabOverrides& obj) {
        obj.RemovedEntities = j.value("RemovedEntities", std::unordered_set<uint64_t>{});
        obj.RemovedComponents = j.value("RemovedComponents", std::unordered_set<uint64_t>{});
        
        obj.EntryOverrides.clear();
        if (j.contains("EntryOverrides") && j["EntryOverrides"].is_object()) {
            for (const auto& [key, value] : j["EntryOverrides"].items()) {
                uint64_t id = std::stoull(key);
                obj.EntryOverrides[id] = value.get<EntryOverrides>();
            }
        }
    }
    
    void to_json(nlohmann::json& j, const PrefabOverrides& obj) {
        j = nlohmann::json{
            {"RemovedEntities", obj.RemovedEntities},
            {"RemovedComponents", obj.RemovedComponents},
            {"EntryOverrides", obj.EntryOverrides}
        };
    }
    
    
    
    void from_json(const nlohmann::json& j, ScriptField& obj) {
        obj.Type = static_cast<ScriptFieldType>(j.value("Type", 0));
        obj.Value = j["Value"];
    }
    
    void to_json(nlohmann::json& j, const ScriptField& obj) {
        j = nlohmann::json{
            {"Type", static_cast<int>(obj.Type)},
            {"Value", obj.Value}
        };
    }
    
    
    
    void from_json(const nlohmann::json& j, Vector2& v) {
        v.Set(j.value("x", 0.0f), j.value("y", 0.0f));
    }

    void to_json(nlohmann::json& j, const Vector2& v) {
        j = nlohmann::json{{"x", v.X()}, {"y", v.Y()}};
    }



    void from_json(const nlohmann::json& j, Vector3& v) {
        v.Set(j.value("x", 0.0f), j.value("y", 0.0f), j.value("z", 0.0f));
    }

    void to_json(nlohmann::json& j, const Vector3& v) {
        j = nlohmann::json{ {"x", v.X()}, {"y", v.Y()}, {"z", v.Z()} };
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

    void from_json(const nlohmann::json& j, MaterialProperty& p) {
        std::string type = j["Type"].get<std::string>();

        if (type == "Float") {
            p.Type = MaterialPropertyType::Float;
            p.Value = j["Value"].get<float>();
        }
        else if (type == "Float2") {
            p.Type = MaterialPropertyType::Float2;
            p.Value = j["Value"].get<Vector2>();
        }
        else if (type == "Float3") {
            p.Type = MaterialPropertyType::Float3;
            p.Value = j["Value"].get<Vector3>();
        }
        else if (type == "Color") {
            p.Type = MaterialPropertyType::Color;
            p.Value = j["Value"].get<Color>();
        }
        else if (type == "Int") {
            p.Type = MaterialPropertyType::Int;
            p.Value = j["Value"].get<int>();
        }
        else if (type == "Bool") {
            p.Type = MaterialPropertyType::Bool;
            p.Value = j["Value"].get<bool>();
        }
    }

    void to_json(nlohmann::json& j, const MaterialProperty& p) {
        switch (p.Type) {
            case MaterialPropertyType::Float:
                j["Type"] = "Float";
                j["Value"] = std::get<float>(p.Value);
                break;
            case MaterialPropertyType::Float2:
                j["Type"] = "Float2";
                j["Value"] = std::get<Vector2>(p.Value);
                break;
            case MaterialPropertyType::Float3:
                j["Type"] = "Float3";
                j["Value"] = std::get<Vector3>(p.Value);
                break;
            case MaterialPropertyType::Color:
                j["Type"] = "Color";
                j["Value"] = std::get<Color>(p.Value);
                break;
            case MaterialPropertyType::Int:
                j["Type"] = "Int";
                j["Value"] = std::get<int>(p.Value);
                break;
            case MaterialPropertyType::Bool:
                j["Type"] = "Bool";
                j["Value"] = std::get<bool>(p.Value);
                break;
            default: {
                Log("Should assert here\n");
            }
        }
    }
    
}
