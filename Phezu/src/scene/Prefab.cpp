#include "scene/Prefab.hpp"

namespace Phezu {
    
    Prefab::Prefab() {
        m_Guid = s_PrefabCount;
        s_PrefabCount++;
    }
    
    std::vector<std::shared_ptr<Entity>> Prefab::Instantiate(std::shared_ptr<Scene> scene) const {
        return m_Blueprint.Instantiate(scene);
    }
}
