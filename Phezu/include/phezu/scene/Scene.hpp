#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include "maths/Math.hpp"
#include "scene/Blueprint.hpp"

namespace Phezu {
    
    class Engine;
    class Entity;
    class Prefab;
    
    class Scene : public std::enable_shared_from_this<Scene> {
    public:
        Scene() = delete;
        Scene(Engine* engine);
        Scene(Engine* engine, const std::string& name);
    public:
        std::weak_ptr<Entity> CreateEntity();
        std::weak_ptr<Entity> CreateEntity(GUID prefabGuid);
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
    public:
        std::string Serialize() const;
        void Deserialize(const std::string& data);
    private:
        void DestroyEntityInternal(uint64_t entityID);
    private:
        Engine* const m_Engine;
        GUID m_Guid;
        Blueprint m_SceneEntities;
        std::string m_Name;
        bool m_IsLoaded;
        std::unordered_map<uint64_t, std::shared_ptr<Entity>> m_RuntimeEntities;
        std::vector<uint64_t> m_EntitiesToDestroy;
    };
}
