#pragma once

#include <vector>
#include <memory>
#include <typeindex>
#include <string>

#include "Renderer.hpp"
#include "scene/components/TransformData.hpp"
#include "scene/components/BehaviourComponent.hpp"

namespace Phezu {
    
    class ShapeData;
    class RenderData;
    class PhysicsData;
    class Scene;
    
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
        const TransformData& GetTransformData() const { return m_TransformData; }
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
    public:
        template<typename T>
        std::weak_ptr<T> GetComponent() {
            if (!std::is_base_of<BehaviourComponent, T>::value) {
                //TODO: copy and paste the logging class
                return std::weak_ptr<T>();
            }
            
            for (int i = 0; i < m_BehaviourComponents.size(); i++) {
                auto* componentPtr = m_BehaviourComponents[i].get();
                if (dynamic_cast<T*>(m_BehaviourComponents[i].get())) {
                    return std::static_pointer_cast<T>(m_BehaviourComponents[i]);
                }
            }
            
            return std::weak_ptr<T>();
        }
        template<typename T>
        std::weak_ptr<T> GetComponent(uint8_t componentPrefabID) {
            if (!std::is_base_of<BehaviourComponent, T>::value) {
                //TODO: copy and paste the logging class
                return std::weak_ptr<T>();
            }
            
            for (int i = 0; i < m_BehaviourComponents.size(); i++) {
                auto* componentPtr = m_BehaviourComponents[i].get();
                if (dynamic_cast<T*>(m_BehaviourComponents[i].get()) && componentPtr->GetComponentPrefabID() == componentPrefabID) {
                    return std::static_pointer_cast<T>(m_BehaviourComponents[i]);
                }
            }
            
            return std::weak_ptr<T>();
        }
        template<typename T>
        std::vector<std::weak_ptr<T>> GetComponents() {
            if (!std::is_base_of<T, BehaviourComponent>::value) {
                //TODO: copy and paste the logging class
                return std::vector<std::weak_ptr<T>>();
            }
            
            std::vector<std::weak_ptr<T>> comps;
            comps.reserve(m_BehaviourComponents.size());
            
            for (int i = 0; i < m_BehaviourComponents.size(); i++) {
                auto* componentPtr = m_BehaviourComponents[i].get();
                if (dynamic_cast<T*>(m_BehaviourComponents[i].get())) {
                    comps.push_back(m_BehaviourComponents[i]);
                }
            }
            
            return comps;
        }
        template<typename T>
        std::weak_ptr<T> AddComponent(uint8_t componentPrefabID = -1) {
            static_assert(std::is_base_of<BehaviourComponent, T>::value, "Component T is not of type BehaviourComponent");
            
            std::shared_ptr<T> component = std::make_shared<T>(this, componentPrefabID);
            
            m_BehaviourComponents.push_back(std::static_pointer_cast<BehaviourComponent>(component));
            
            return component;
        }
        template<typename T>
        void RemoveComponent() {
            if (!std::is_base_of<BehaviourComponent, T>::value) {
                //TODO: copy and paste the logging class
                return;
            }
            
            for (int i = 0; i < m_BehaviourComponents.size(); i++) {
                if (dynamic_cast<T*>(m_BehaviourComponents[i].get())) {
                    m_BehaviourComponents.erase(m_BehaviourComponents.begin() + i);
                    return;
                }
            }
        }
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
        std::vector<std::shared_ptr<BehaviourComponent>> m_BehaviourComponents;
    private:
        static uint64_t s_EntitiesCount;
        uint64_t m_EntityID;
        bool m_IsActive;
        std::string m_Tag;
        
        friend void SetParentInternal(std::weak_ptr<Entity> _this, std::weak_ptr<Entity> child);
        friend class Scene;
        friend void Destroy(Entity* entity);
    };
}
