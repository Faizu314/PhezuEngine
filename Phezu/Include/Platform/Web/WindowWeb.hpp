// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <unordered_map>

#include "Core/Window.hpp"
#include <Core/Platform.hpp>

namespace Phezu {
    
    class WindowWeb : public IWindow {
    public:
        WindowWeb() = default;
    public:
        void Init(const WindowArgs& args);
        void Destroy();
    public:
        void Update();
    public:
        int GetWidth() const override { return m_Width; }
        int GetHeight() const override { return m_Height; }
        int GetRenderScale() const override { return m_RenderScale; }
        int RegisterWindowResizeCallback(const ResizeCallback& callback) override;
        void UnregisterWindowResizeCallback(int subscriberId) override;
    public:
        int OnWindowMove();
        int OnWindowResize(int width, int height);
        int OnWindowClose();
    public:
        void* GetWindowPtr() { return m_WindowPtr; }
    private:
        int m_Width = 0;
        int m_Height = 0;
        int m_RenderScale = 0;
        void* m_WindowPtr = nullptr;
    private:
        int m_CurrSubscriberId = 0;
        std::unordered_map<int, ResizeCallback> m_Subscribers;
    };
}
