#pragma once

#include <vector>
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
        Entity(Scene* scene);
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
        PhysicsData* GetPhysicsData() const { return m_PhysicsData; }
        size_t GetScriptComponentCount() const { return m_ScriptComponents.size(); }
        ScriptComponent* GetScriptComponent(size_t index);
        bool HasScriptComponent(const std::string& classFullname);
    public:
        ShapeData* AddShapeData();
        RenderData* AddRenderData(Color tint = Color::White);
        PhysicsData* AddPhysicsData(bool isStatic);
        ScriptComponent* AddScriptComponent(const std::string& classFullname);
    public:
        Scene* GetSceneContext() const { return m_Scene; }
        TransformData* GetParent() const;
        void SetParent(Entity* parent);
        void RemoveParent();
        size_t GetChildCount();
        Entity* GetChild(size_t childIndex);
    private:
        void OnDestroyed();
        void OnChildDestroyed();
        void AddChild(Entity* child);
        void RecalculateSubtreeTransformations();
    private:
        Scene* m_Scene;
        Entity* m_Parent;
        std::vector<Entity*> m_Children;
    private:
        TransformData m_TransformData;
        ShapeData* m_ShapeData;
        RenderData* m_RenderData;
        PhysicsData* m_PhysicsData;
    private:
        std::vector<ScriptComponent> m_ScriptComponents;
    private:
        static uint64_t s_EntitiesCount;
        uint64_t m_EntityID;
        bool m_IsActive;
        std::string m_Tag;
        
        friend void SetParentInternal(Entity* _this, Entity* child);
        friend class Scene;
        friend class Physics;
        friend void Destroy(Entity* entity);
    };
}
