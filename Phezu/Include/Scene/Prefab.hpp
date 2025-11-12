#pragma once

#include <vector>
#include "Scene/Blueprint.hpp"
#include "GUID.hpp"

namespace Phezu {
    
    class Prefab {
    public:
        Prefab(Engine* engine, GUID guid);
    public:
        GUID GetGuid() { return m_Guid; }
    public:
        std::string Serialize() const;
        void Deserialize(const std::string& data);
    public:
        const Blueprint& GetBlueprint() const { return m_Blueprint; }
        Entity* Instantiate(Scene* scene) const;
    public: //EDITOR ONLY
        void UpdateEntry(uint64_t fileID, const BlueprintEntry& newEntry);
        void RemoveEntry(uint64_t fileID);
        void AddEntry(const Blueprint& newEntry);
        
    private:
        GUID m_Guid;
        Blueprint m_Blueprint;
    private:
        Engine* m_Engine;
    };
    
}
