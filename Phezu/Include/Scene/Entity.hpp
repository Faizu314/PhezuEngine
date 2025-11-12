#pragma once

#include <vector>
#include <typeindex>
#include <string>
#include <unordered_map>

#include "Renderer.hpp"
#include "Scene/Components/TransformData.hpp"
#include "Scene/Components/ScriptComponent.hpp"

namespace Phezu {
    
    class Scene;
    class Physics;
    
    class Entity {
    public:
        Entity() = delete;
        Entity(Scene* scene);
        ~Entity();
    public:
        bool IsActive;
        std::string Tag;
    public:
        uint64_t GetEntityID() const { return m_EntityID; }
        bool IsDirty() { return m_TransformData.GetIsDirty(); }
    public:
        DataComponent* AddDataComponent(ComponentType componentType);
        DataComponent* GetDataComponent(ComponentType componentType);
        TransformData* GetTransformData() { return &m_TransformData; }
        void RemoveDataComponent(ComponentType componentType);
        bool HasDataComponent(ComponentType componentType) const;
        ScriptComponent* AddScriptComponent(const std::string& classFullname);
        ScriptComponent* GetScriptComponent(size_t index);
        void RemoveScriptComponent(const std::string& classFullname);
        size_t GetScriptComponentCount() const { return m_ScriptComponents.size(); }
        bool HasScriptComponent(const std::string& classFullname) const;
    public:
        Scene* GetSceneContext() const { return m_Scene; }
        TransformData* GetParent() const;
        void SetParent(Entity* parent);
        void RemoveParent();
        size_t GetChildCount() { return m_Children.size(); }
        Entity* GetChild(size_t childIndex);
        void Destroy();
    private:
        void OnDestroyed();
        void OnChildRemoved(const Entity* removedChild);
        void AddChild(Entity* child);
        void RecalculateSubtreeTransforms();
    private:
        Scene* m_Scene;
        uint64_t m_EntityID;
        Entity* m_Parent;
        std::vector<Entity*> m_Children;
    private:
        TransformData m_TransformData;
        std::unordered_map<ComponentType, DataComponent*> m_DataComponents;
        std::vector<ScriptComponent*> m_ScriptComponents;
    private:
        static uint64_t s_EntitiesCount;
        
        friend class Scene;
        friend class Physics;
    };
}
