#pragma once

#include <vector>
#include <memory>
#include <typeindex>
#include <string>

#include "Renderer.hpp"
#include "scene/components/TransformData.hpp"
#include "scene/components/ScriptComponent.hpp"

namespace Phezu {
    
    class ShapeData;
    class RenderData;
    class PhysicsData;
    class Scene;
    class Physics;
    
    class Entity {
    public:
        Entity() = delete;
        Entity(std::weak_ptr<Scene> scene);
        ~Entity();
    public:
        uint64_t GetEntityID() const;
        void SetActive(bool isActive);
        bool GetActive() const;
        bool IsDirty();
        const std::string& GetTag() { return m_Tag; }
        void SetTag(const std::string& tag) { m_Tag = tag; }
    public:
        TransformData* GetTransformData();
        ShapeData* GetShapeData() const { return m_ShapeData; }
        RenderData* GetRenderData() const { return m_RenderData; }
        std::weak_ptr<PhysicsData> GetPhysicsData() const { return m_PhysicsData; }
    public:
        ShapeData* AddShapeData();
        RenderData* AddRenderData(Color tint = Color::White);
        std::weak_ptr<PhysicsData> AddPhysicsData(bool isStatic);
    public:
        TransformData* GetParent() const;
        void SetParent(std::weak_ptr<Entity> parent);
        void RemoveParent();
        size_t GetChildCount();
        std::weak_ptr<Entity> GetChild(size_t childIndex);
    private:
        void OnDestroyed();
        void OnChildDestroyed();
        void AddChild(std::weak_ptr<Entity> child);
        void RecalculateSubtreeTransformations();
    private:
        const std::weak_ptr<Scene> m_Scene;
        Entity* m_Parent;
        std::vector<std::weak_ptr<Entity>> m_Children;
    private:
        TransformData m_TransformData;
        ShapeData* m_ShapeData;
        RenderData* m_RenderData;
        std::shared_ptr<PhysicsData> m_PhysicsData;
    private:
        std::vector<std::shared_ptr<ScriptComponent>> m_BehaviourComponents;
    private:
        static uint64_t s_EntitiesCount;
        uint64_t m_EntityID;
        bool m_IsActive;
        std::string m_Tag;
        
        friend void SetParentInternal(std::weak_ptr<Entity> _this, std::weak_ptr<Entity> child);
        friend class Scene;
        friend class Physics;
        friend void Destroy(Entity* entity);
    };
}
