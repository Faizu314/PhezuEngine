#pragma once

#include "Core/Window.hpp"
#include "Platform/Win32/PlatformWin32.hpp"

#include <Windows.h>
#include <string>

namespace Phezu {
    
    class WindowWin32 : public IWindow {
    public:
        WindowWin32() = default;
    public:
        int Init(const WindowArgs& args, const char winClassName[], HINSTANCE hInstance);
        void Destroy();
    public:
        void Update();
    public:
        int GetWidth() const override { return m_Width; }
        int GetHeight() const override { return m_Height; }
        int GetRenderScale() const override { return m_RenderScale; }
    public:
        int OnWindowMove();
        int OnWindowResize(UINT flag, int width, int height);
        int OnWindowClose();
    public:
        HDC GetDeviceContext() { return m_Hdc; }
    private:
        int m_Width = 0;
        int m_Height = 0;
        int m_RenderScale = 0;
        HWND m_WindowPtr = nullptr;
        HDC m_Hdc = nullptr;
    };
}
