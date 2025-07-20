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
    
    struct Resolution {
        int Width = 1280;
        int Height = 720;
        int RenderScale = 1;
    };

    struct Paths {
        std::filesystem::path ExePath;
        std::filesystem::path ProjectPath;
        std::filesystem::path ScriptCoreDllPath;
        std::filesystem::path MonoCoreLibsPath;
    };

    struct EngineConfig {
        std::string Name;
        Paths AllPaths;
        Resolution ResolutionSettings;
    };

    
    class Engine {
    public:
        int Init(EngineConfig& config);
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
        std::filesystem::path GetScriptCoreDllPath() const { return m_ScriptCoreDllPath; }
        std::filesystem::path GetMonoCoreLibsPath() const { return m_MonoCoreLibsPath; }
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
        std::filesystem::path m_ScriptCoreDllPath;
        std::filesystem::path m_MonoCoreLibsPath;
    private:
        static Engine* s_Instance;
        
        friend Engine* GetEngine();
        friend Engine& CreateEngine();
    };
}
