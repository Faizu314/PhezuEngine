#pragma once

#include <vector>
#include "scene/Blueprint.hpp"

namespace Phezu {
    
    class Prefab {
    public:
        Prefab();
    public:
        uint64_t GetGuid() { return m_Guid; }
    public:
        void SaveToDisk();
        void LoadFromDisk();
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
        static uint64_t s_PrefabCount;
        uint64_t m_Guid;
        Blueprint m_Blueprint;
    };
    
}
