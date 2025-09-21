#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include "maths/Math.hpp"
#include "scene/Blueprint.hpp"

namespace Phezu {
    
    class Engine;
    class Entity;
    class Prefab;
    
    class Scene {
    public:
        Scene() = delete;
        Scene(Engine* engine);
        Scene(Engine* engine, const std::string& name);
    public:
        Entity* CreateEntity();
        Entity* CreateEntity(GUID prefabGuid);
        Entity* GetEntity(uint64_t entityID) const;
        void DestroyEntity(uint64_t entityID);
        void Load();
        void LogicUpdate(float deltaTime);
        void UpdateHierarchy();
        void GetPhysicsEntities(std::vector<Entity*>& staticEntities, std::vector<Entity*>& dynamicEntities, size_t& staticIndex, size_t& dynamicIndex) const;
        void GetRenderableEntities(std::vector<Entity*>& entities, size_t& count) const;
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
        std::unordered_map<uint64_t, Entity*> m_RuntimeEntities;
        std::vector<uint64_t> m_EntitiesToDestroy;
    };
}
