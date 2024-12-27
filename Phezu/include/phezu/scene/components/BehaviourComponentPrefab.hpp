#pragma once

#include <cstdint>
#include <memory>
#include <typeindex>

#include "scene/Scene.hpp"
#include "scene/Entity.hpp"

namespace Phezu {
    
    class BehaviourComponent;
    
    class BehaviourComponentPrefabBase {
    public:
        BehaviourComponentPrefabBase() = delete;
        BehaviourComponentPrefabBase(std::unique_ptr<size_t[]> pathToEntityInBlueprint, size_t pathSize, uint8_t componentPrefabID);
    public:
        virtual std::shared_ptr<BehaviourComponentPrefabBase> Clone() = 0;
    public:
        uint8_t GetComponentID() const;
        virtual std::weak_ptr<BehaviourComponent> CreateComponent(std::weak_ptr<Entity> entity) const = 0;
        virtual void InitRuntimeComponentInternal(std::weak_ptr<Scene> scene, std::shared_ptr<BehaviourComponent> component) const = 0;
        virtual std::weak_ptr<BehaviourComponent> GetRuntimeComponent(std::weak_ptr<Scene> scene, std::shared_ptr<Entity> entity) const = 0;
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
            
            return std::make_shared<BehaviourComponentPrefab<T>>(path, m_PathSize, m_ComponentPrefabID);
        }
        std::weak_ptr<BehaviourComponent> CreateComponent(std::weak_ptr<Entity> entity) const override final {
            auto entityL = entity.lock();
            return entityL->AddComponent<T>(m_ComponentPrefabID);
        }
        void InitRuntimeComponentInternal(std::weak_ptr<Scene> scene, std::shared_ptr<BehaviourComponent> component) const override final {
            InitRuntimeComponent(scene, std::dynamic_pointer_cast<T>(component));
        }
        std::weak_ptr<BehaviourComponent> GetRuntimeComponent(std::weak_ptr<Scene> scene, std::shared_ptr<Entity> entity) const override final {
            return entity->GetComponent<T>(m_ComponentPrefabID);
        }
        virtual void InitRuntimeComponent(std::weak_ptr<Scene> scene, std::shared_ptr<T> component) const {};
    };
}
