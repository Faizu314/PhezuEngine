#pragma once

#include <vector>
#include "scene/Blueprint.hpp"
#include "GUID.hpp"

namespace Phezu {
    
    class Prefab {
    public:
        Prefab(GUID guid);
    public:
        GUID GetGuid() { return m_Guid; }
    public:
        std::string Serialize() const;
        void Deserialize(const std::string& data);
    public:
        std::vector<std::shared_ptr<Entity>> Instantiate(std::shared_ptr<Scene> scene) const;
        //Maybe a function like SetAsBlueprint(List<Entity>)
        
    public: //EDITOR ONLY
        //Functions that change prefab's blueprint go here
        //We can return a mapping between realtime instantiated entity ids and their fileids in prefab's blueprint
        void UpdateEntry(uint64_t fileID, const BlueprintEntry& newEntry);
        void RemoveEntry(uint64_t fileID);
        void AddEntry(const Blueprint& newEntry);
        
    private:
        GUID m_Guid;
        Blueprint m_Blueprint;
    };
    
}
