#include "Platform/Win32/WindowWin32.hpp"

namespace Phezu {

    WindowWin32::WindowWin32(const WindowArgs& args) : m_Width(args.Width), m_Height(args.Height), m_RenderScale(args.RenderScale) {
        
    }
    
    WindowWin32::~WindowWin32() {
        
    }
}
