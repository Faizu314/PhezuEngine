#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include "maths/Math.hpp"

namespace Phezu {
    
    class Engine;
    class Entity;
    class Prefab;
    class EntityBlueprint;
    class EntityTemplate;
    
    class Scene : public std::enable_shared_from_this<Scene> {
    public:
        Scene() = delete;
        Scene(Engine* engine, const std::string& name);
    public:
        EntityBlueprint& CreateSceneEntity(uint64_t prefabID = 0);
        std::weak_ptr<Entity> CreateEntity();
        std::weak_ptr<Entity> CreateEntity(uint64_t prefabID);
        std::weak_ptr<Entity> GetEntity(uint64_t entityID) const;
        void DestroyEntity(uint64_t entityID);
        void Load();
        void LogicUpdate(float deltaTime);
        void UpdateHierarchy();
        void GetPhysicsEntities(std::vector<std::weak_ptr<Entity>>& entities, size_t& staticCount, size_t& dynamicCount) const;
        void GetRenderableEntities(std::vector<std::weak_ptr<Entity>>& entities, size_t& count) const;
        void BeginUnload();
        void Unload();
        long long unsigned int GetFrameCount() const;
    private:
        void BuildEntityFromBlueprint(std::shared_ptr<Entity> entity, const EntityBlueprint* blueprint);
        void InitEntityComponentsFromBlueprint(std::shared_ptr<Entity> entity, const EntityBlueprint* prefabEntity);
    private:
        void CallStartOnEntity(std::shared_ptr<Entity> entity);
        void DestroyEntityInternal(uint64_t entityID);
    private:
        Engine* const m_Engine;
        const std::string m_Name;
        bool m_IsLoaded;
        std::vector<std::unique_ptr<EntityTemplate>> m_SceneEntities;
        std::unordered_map<uint64_t, std::shared_ptr<Entity>> m_RuntimeEntities;
        std::vector<uint64_t> m_EntitiesToDestroy;
    };
}
