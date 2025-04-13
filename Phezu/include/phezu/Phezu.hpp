#pragma once

#include "Engine.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include "Physics.hpp"
#include "maths/Random.hpp"
#include "maths/Math.hpp"
#include "scene/Prefab.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneManager.hpp"
#include "scene/Blueprint.hpp"
#include "scene/BlueprintEntry.hpp"
#include "scene/Entity.hpp"
#include "scene/components/DataComponent.hpp"
#include "scene/components/TransformData.hpp"
#include "scene/components/ShapeData.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/PhysicsData.hpp"
#include "scene/components/ScriptComponent.hpp"

namespace Phezu {
    
    Engine& CreateEngine();
    std::weak_ptr<Entity> CreateEntity();
    std::weak_ptr<Entity> CreateEntity(GUID prefabGuid);
    void LoadScene(const std::string& sceneName);
    void Destroy(Entity* entity);
    const InputData& GetInput();
    long long unsigned int GetFrameCount();
    
    template<typename T>
    void SubscribeToOnSceneLoaded(T* subscriber, void (T::*handler)(void)) {
        if (Engine::s_Instance == nullptr) {
            //TODO: Logging
            return;
        }
        
        Engine::s_Instance->m_SceneManager.SubscribeToOnSceneLoaded(subscriber, handler);
    }
    
    void UnsubscribeToOnSceneLoaded(void* subscriber);
}
