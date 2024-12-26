#include "scene/components/BehaviourComponentPrefab.hpp"

namespace Phezu {
    
    BehaviourComponentPrefabBase::BehaviourComponentPrefabBase(std::unique_ptr<size_t[]> pathToPrefabEntity, size_t pathSize, uint8_t componentID)
    : m_PathToEntityInBlueprint(std::move(pathToPrefabEntity)), m_PathSize(pathSize), m_ComponentPrefabID(componentID) { }
    
    uint8_t BehaviourComponentPrefabBase::GetComponentID() const { return m_ComponentPrefabID; }

}
