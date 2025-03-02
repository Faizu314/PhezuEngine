#include "scene/EntityTemplate.hpp"
#include "scene/Scene.hpp"

namespace Phezu {
    
    EntityTemplate::EntityTemplate(ConstructionToken token, std::weak_ptr<Scene> scene, const EntityBlueprint& blueprint)
    : m_Scene(scene) {}
    
    std::shared_ptr<EntityTemplate> EntityTemplate::MakeShared(std::weak_ptr<Scene> scene, const EntityBlueprint& blueprint) {
        ConstructionToken token;
        return std::make_shared<EntityTemplate>(token, scene, blueprint);
    }
    
    std::unique_ptr<EntityTemplate> EntityTemplate::MakeUnique(std::weak_ptr<Scene> scene, const EntityBlueprint& blueprint) {
        ConstructionToken token;
        return std::make_unique<EntityTemplate>(token, scene, blueprint);
    }
}
