#pragma once

#include "Input.hpp"
#include "Physics.hpp"
#include "scene/SceneManager.hpp"
#include "AssetManagement/AssetManager.hpp"
#include <filesystem>

namespace Phezu {
    
    class Scene;
    class Prefab;
    class Window;
    class Renderer;
    
    class Engine {
    public:
        int Init(std::filesystem::path exePath, const std::string name, int width, int height, int renderScale = 1);
        void Run();
        std::weak_ptr<Scene> GetMasterScene();
        void Destroy();
        std::weak_ptr<Prefab> CreatePrefab();
        std::weak_ptr<const Prefab> GetPrefab(GUID guid);
    public:
        void LoadScene(const std::string& sceneName);
        SceneManager& GetSceneManager();
        AssetManager& GetAssetManager();
        std::weak_ptr<Entity> CreateEntity();
        std::weak_ptr<Entity> CreateEntity(GUID prefabGuid);
        long long unsigned int GetFrameCount() const { return m_FrameCount; }
        const InputData& GetInput();
        std::filesystem::path GetExePath() { return m_ExePath; }
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
        AssetManager m_AssetManager;
    private:
        bool m_HasInited;
        bool m_IsRunning;
        long long unsigned int m_FrameCount;
        std::filesystem::path m_ExePath;
    private:
        static Engine* s_Instance;
        
        friend Engine* GetEngine();
        friend Engine& CreateEngine();
        template<typename T>
        friend void SubscribeToOnSceneLoaded(T* subscriber, void (T::*handler)(void));
    };
}
