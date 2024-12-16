#pragma once

#include <vector>
#include <string>
#include <memory>
#include "maths/Math.hpp"

namespace Phezu {
    
    class Scene;
    class Entity;
    class PrefabEntity;
    class BehaviourComponent;
    
    class EntityTemplate {
    private:
        struct ConstructionToken {};
        static std::shared_ptr<EntityTemplate> MakeShared(std::weak_ptr<Scene> scene, uint64_t prefabID = 0);
        static std::unique_ptr<EntityTemplate> MakeUnique(std::weak_ptr<Scene> scene, uint64_t prefabID = 0);
    public:
        //if hierarchy entity is not from a prefab than use a default prefab template
        //EntityPrefabOverride OverridePrefab();
        bool OverrideTag;
        std::string TagOverride;
        bool OverridePosition;
        Vector2 PositionOverride;
        EntityTemplate(ConstructionToken token, std::weak_ptr<Scene> scene, uint64_t prefabID = 0);
    public:
        uint64_t GetPrefabID() const;
    private:
        const std::weak_ptr<Scene> m_Scene;
        const uint64_t m_PrefabID;
        std::weak_ptr<Entity> m_RuntimeRootEntity;
        
        friend class Scene;
    };
}
