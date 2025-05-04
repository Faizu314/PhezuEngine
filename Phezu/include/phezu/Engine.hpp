#pragma once

#include "Input.hpp"
#include "Physics.hpp"
#include "scene/SceneManager.hpp"
#include "AssetManagement/AssetManager.hpp"
#include "scripting/ScriptEngine.hpp"

#include <filesystem>

namespace Phezu {
    
    class Scene;
    class Prefab;
    class Window;
    class Renderer;
    
    class Engine {
    public:
        int Init(std::filesystem::path exePath, std::filesystem::path projectPath, const std::string name, int width, int height, int renderScale = 1);
        void Run();
    public:
        std::weak_ptr<const Prefab> GetPrefab(GUID guid);
    public:
        SceneManager& GetSceneManager() { return m_SceneManager; }
        AssetManager& GetAssetManager() { return m_AssetManager; }
        ScriptEngine& GetScriptEngine() { return m_ScriptEngine; }
        const InputData& GetInput() const { return Input::GetInput(); }
        long long unsigned int GetFrameCount() const { return m_FrameCount; }
        std::filesystem::path GetExePath() const { return m_ExePath; }
        std::filesystem::path GetProjectPath() const { return m_ProjectPath; }
    private:
        Engine();
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;
    private:
        void Destroy();
    private:
        Window* m_Window;
        Renderer* m_Renderer;
        Physics m_Physics;
    private:
        SceneManager m_SceneManager;
        AssetManager m_AssetManager;
        ScriptEngine m_ScriptEngine;
    private:
        bool m_HasInited;
        bool m_IsRunning;
        long long unsigned int m_FrameCount;
        std::filesystem::path m_ExePath;
        std::filesystem::path m_ProjectPath;
    private:
        static Engine* s_Instance;
        
        friend Engine* GetEngine();
        friend Engine& CreateEngine();
    };
}
