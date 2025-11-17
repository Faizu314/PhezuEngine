#include "Window.hpp"
#include "Logger.hpp"

namespace Phezu {

    Window::Window(const std::string& name, int width, int height, int renderScale) : m_Width(width), m_Height(height), m_RenderScale(renderScale) {
        int windowsFlag = 0;

    }
    
    Window::~Window() {
        
    }
    
    Window::operator SDL_Window*() const {
        return m_WindowPtr;
    }
}
