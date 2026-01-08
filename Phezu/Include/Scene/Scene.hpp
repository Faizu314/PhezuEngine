#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "Maths/Math.hpp"
#include "Asset/Core/Asset.hpp"
#include "Asset/Types/PrefabAsset.hpp"

namespace Phezu {
    
    class Engine;
    class Entity;
    
    class Scene {
    public:
        Scene() = delete;
        Scene(Engine* engine, const std::string& name);
    public:
        Entity* CreateEntity();
        Entity* CreateEntity(AssetHandle prefabHandle);
        Entity* GetEntity(uint64_t entityID) const;
        void DestroyEntity(uint64_t entityID);
        void LogicUpdate(float deltaTime);
        void UpdateHierarchy();
        void GetPhysicsEntities(std::vector<Entity*>& staticEntities, std::vector<Entity*>& dynamicEntities, size_t& staticIndex, size_t& dynamicIndex) const;
        void GetRenderableEntities(std::vector<Entity*>& entities, size_t& count) const;
        void BeginUnload();
        void Unload();
    private:
        void DestroyEntityInternal(uint64_t entityID);
        void DestroyEntityInternal(Entity* entity);
    private:
        Engine* const m_Engine;
        std::string m_Name;
        std::unordered_map<uint64_t, Entity*> m_RuntimeEntities;
        std::vector<uint64_t> m_EntitiesToDestroy;
    };
}
