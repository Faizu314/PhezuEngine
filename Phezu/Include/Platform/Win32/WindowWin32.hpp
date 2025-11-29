#pragma once

#include "Platform/Win32/PlatformWin32.hpp"

#include <Windows.h>
#include <string>

namespace Phezu {
    
    class WindowWin32 {
    public:
        WindowWin32() = default;
    public:
        int Init(const WindowArgs& args, const char winClassName[], HINSTANCE hInstance);
        void Destroy();
    public:
        void Update();
    public:
        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }
        int GetRenderScale() const { return m_RenderScale; }
    public:
        int OnWindowMove();
        int OnWindowResize(UINT flag, int width, int height);
        int OnWindowClose();
    private:
        int m_Width = 0;
        int m_Height = 0;
        int m_RenderScale = 0;
        HWND m_WindowPtr = nullptr;
    };
}
