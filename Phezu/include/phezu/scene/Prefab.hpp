#pragma once

#include <vector>
#include "scene/EntityBlueprint.hpp"

namespace Phezu {
    
    class Prefab {
    public:
        Prefab() {
            m_PrefabID = s_PrefabCount;
            s_PrefabCount++;
        }
    public:
        uint64_t GetPrefabID() { return m_PrefabID; }
        //EntityBlueprint RootEntity;
    private:
        static uint64_t s_PrefabCount;
        uint64_t m_PrefabID;
    };
    
}
