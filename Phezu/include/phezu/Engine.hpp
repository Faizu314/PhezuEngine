#pragma once

#include <unordered_map>
#include <typeindex>
#include <functional>

#include "Input.hpp"
#include "Physics.hpp"
#include "scene/SceneManager.hpp"

namespace Phezu {
    
    class Scene;
    class Prefab;
    class Window;
    class Renderer;
    
    class Engine {
    public:
        int Init(const std::string name, int width, int height, int renderScale = 1);
        void Run();
        std::weak_ptr<Scene> GetMasterScene();
        void Destroy();
        std::weak_ptr<Scene> CreateScene(const std::string& name);
        std::weak_ptr<Prefab> CreatePrefab();
        std::weak_ptr<const Prefab> GetPrefab(uint64_t prefabID);
    public:
        std::function<void(std::shared_ptr<Entity>)> GetComponentConstructor(std::type_index componentType);
        void LoadScene(const std::string& sceneName);
        SceneManager& GetSceneManager();
        std::weak_ptr<Entity> CreateEntity();
        std::weak_ptr<Entity> CreateEntity(uint64_t prefabID);
        long long unsigned int GetFrameCount() const { return m_FrameCount; }
        const InputData& GetInput();
    private:
        Engine();
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;
    private:
        Window* m_Window;
        Renderer* m_Renderer;
        Input m_Input;
        SceneManager m_SceneManager;
        Physics m_Physics;
    private:
        std::unordered_map<std::type_index, std::function<void(std::shared_ptr<Entity>)>> m_Constructors;
        std::unordered_map<uint64_t, std::shared_ptr<Prefab>> m_Prefabs;
        bool m_HasInited;
        bool m_IsRunning;
        long long unsigned int m_FrameCount;
    private:
        static Engine* s_Instance;
        
        friend Engine* GetEngine();
        friend Engine& CreateEngine();
        template<typename T>
        friend void SubscribeToOnSceneLoaded(T* subscriber, void (T::*handler)(void));
        template<typename T>
        friend void RegisterComponent(std::function<void(std::shared_ptr<Entity>)> constructor);
    };
}
