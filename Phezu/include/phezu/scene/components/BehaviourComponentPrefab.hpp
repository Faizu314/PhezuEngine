#pragma once

#include <cstdint>
#include <memory>
#include <typeindex>

#include "scene/Scene.hpp"
#include "scene/Entity.hpp"

namespace Phezu {
    
    class BehaviourComponent;
    class EntityTemplate;
    
    class BehaviourComponentPrefabBase {
    public:
        BehaviourComponentPrefabBase() = delete;
        BehaviourComponentPrefabBase(std::unique_ptr<size_t[]> pathToPrefabEntity, size_t pathSize, uint8_t componentID)
        : m_PathToEntityInBlueprint(std::move(pathToPrefabEntity)), m_PathSize(pathSize), m_ComponentPrefabID(componentID) { }
    public:
        BehaviourComponentPrefabBase& operator=(const BehaviourComponentPrefabBase& other) = delete;
    public:
        virtual std::shared_ptr<BehaviourComponentPrefabBase> Clone() = 0;
        virtual std::weak_ptr<BehaviourComponent> CreateComponent(std::weak_ptr<Entity> entity) const = 0;
        virtual void InitRuntimeComponentInternal(std::weak_ptr<Scene> scene, std::shared_ptr<BehaviourComponent> component) const = 0;
        virtual std::weak_ptr<BehaviourComponent> GetRuntimeComponent(std::shared_ptr<Entity> entity) const = 0;
        virtual std::weak_ptr<BehaviourComponent> GetRuntimeComponentFromRoot(std::shared_ptr<Entity> rootEntity) const = 0;
    public:
        uint8_t GetComponentID() const { return m_ComponentPrefabID; }
    protected:
        const uint8_t m_ComponentPrefabID;
        const size_t m_PathSize;
        std::unique_ptr<size_t[]> m_PathToEntityInBlueprint;
    };
    
    template <typename T>
    class BehaviourComponentPrefab : public BehaviourComponentPrefabBase {
        static_assert(std::is_base_of<BehaviourComponent, T>::value, "Component T is not of type BehaviourComponent");
    public:
        using BehaviourComponentPrefabBase::BehaviourComponentPrefabBase;
    public:
        std::shared_ptr<BehaviourComponentPrefabBase> Clone() override {
            std::unique_ptr<size_t[]> path = std::make_unique<size_t[]>(m_PathSize);
            for (size_t i = 0; i < m_PathSize; i++)
                path[i] = m_PathToEntityInBlueprint[i];
            
            return std::make_shared<BehaviourComponentPrefab<T>>(std::move(path), m_PathSize, m_ComponentPrefabID);
        }
        std::weak_ptr<BehaviourComponent> CreateComponent(std::weak_ptr<Entity> entity) const override final {
            auto entityL = entity.lock();
            return entityL->AddComponent<T>(m_ComponentPrefabID);
        }
        void InitRuntimeComponentInternal(std::weak_ptr<Scene> scene, std::shared_ptr<BehaviourComponent> component) const override final {
            InitRuntimeComponent(scene, std::dynamic_pointer_cast<T>(component));
        }
        std::weak_ptr<BehaviourComponent> GetRuntimeComponent(std::shared_ptr<Entity> entity) const override final {
            return entity->GetComponent<T>(m_ComponentPrefabID);
        }
        std::weak_ptr<BehaviourComponent> GetRuntimeComponentFromRoot(std::shared_ptr<Entity> rootEntity) const override final {
            for (int i = 0; i < m_PathSize; i++) {
                rootEntity = rootEntity->GetChild(m_PathToEntityInBlueprint[i]);
            }
            
            return rootEntity->GetComponent<T>(m_ComponentPrefabID);
        }
    protected:
        virtual void InitRuntimeComponent(std::weak_ptr<Scene> scene, std::shared_ptr<T> component) const {};
    };
}
