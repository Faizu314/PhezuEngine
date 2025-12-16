#pragma once

#include <filesystem>

#include "Core/Platform.hpp"
#include "Physics.hpp"
#include "Scene/SceneManager.hpp"
#include "Asset/Core/AssetManager.hpp"
#include "Graphics/Renderer.hpp"
#include "Scripting/ScriptEngine.hpp"

namespace Phezu {
    
    class Scene;
    class PrefabAsset;

    struct Paths {
        std::filesystem::path ExePath;
        std::filesystem::path AssetsPath;
        std::filesystem::path ScriptCoreDllPath;
        std::filesystem::path MonoCoreLibsPath;
    };

    struct EngineArgs {
        Paths AllPaths;
        WindowArgs WindowArgs;
    };

    
    class Engine {
    public:
        int Init(EngineArgs& args);
        void Run();
    public:
        SceneManager& GetSceneManager() { return m_SceneManager; }
        AssetManager& GetAssetManager() { return m_AssetManager; }
        ScriptEngine& GetScriptEngine() { return m_ScriptEngine; }
        const InputData& GetInput() const { return m_Platform->GetInput(); }
        long long unsigned int GetFrameCount() const { return m_FrameCount; }
        std::filesystem::path GetExePath() const { return m_ExePath; }
        std::filesystem::path GetAssetsPath() const { return m_AssetsPath; }
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
        IPlatform* m_Platform;
    private:
        Physics m_Physics;
    private:
        Renderer m_Renderer;
        SceneManager m_SceneManager;
        AssetManager m_AssetManager;
        ScriptEngine m_ScriptEngine;
    private:
        bool m_HasInited;
        bool m_IsRunning;
        long long unsigned int m_FrameCount;
        std::filesystem::path m_ExePath;
        std::filesystem::path m_AssetsPath;
        std::filesystem::path m_ScriptCoreDllPath;
        std::filesystem::path m_MonoCoreLibsPath;
    private:
        static Engine* s_Instance;
        
        friend Engine* GetEngine();
        friend Engine& CreateEngine();
    };
}
