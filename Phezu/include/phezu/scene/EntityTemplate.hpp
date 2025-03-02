#pragma once

#include <vector>
#include <string>
#include <memory>
#include "maths/Math.hpp"
#include "EntityBlueprint.hpp"

namespace Phezu {
    
    class Scene;
    class Entity;
    class BehaviourComponent;
    
    class EntityTemplate {
    private:
        struct ConstructionToken {};
        static std::shared_ptr<EntityTemplate> MakeShared(std::weak_ptr<Scene> scene, const EntityBlueprint& blueprint);
        static std::unique_ptr<EntityTemplate> MakeUnique(std::weak_ptr<Scene> scene, const EntityBlueprint& blueprint);
    public:
        EntityTemplate(ConstructionToken token, std::weak_ptr<Scene> scene, const EntityBlueprint& blueprint);
    private:
        const std::weak_ptr<Scene> m_Scene;
        std::weak_ptr<Entity> m_RuntimeRootEntity;
        //EntityBlueprint m_RootEntity;
        
        friend class Scene;
    };
}
