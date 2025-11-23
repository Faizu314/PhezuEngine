#pragma once

#include "Core/Window.hpp"
#include <string>

namespace Phezu {
    
    class WindowWin32 : public IWindow {
    public:
        WindowWin32(const WindowArgs& args);
        ~WindowWin32();
    public:
        int GetWidth() const override { return m_Width; }
        int GetHeight() const override { return m_Height; }
        int GetRenderScale() const override { return m_RenderScale; }
    private:
        const int m_Width;
        const int m_Height;
        const int m_RenderScale;
    };
}
