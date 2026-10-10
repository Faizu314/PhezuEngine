#include "Platform/Web/WindowWeb.hpp"
#include "Platform/Web/PlatformWeb.hpp"

#include "emscripten/html5.h"

namespace Phezu {

    void WindowWeb::Init(const WindowArgs& args) {
        m_Width = args.Width;
        m_Height = args.Height;
        m_RenderScale = args.RenderScale;

        emscripten_set_canvas_element_size("#canvas", m_Width, m_Height);
    }

    void WindowWeb::Update() {

    }

    int WindowWeb::OnWindowMove() {
        return 0;
    }

    int WindowWeb::OnWindowResize(int width, int height) {
        return 0;
    }

    int WindowWeb::RegisterWindowResizeCallback(const ResizeCallback& callback) {
        int id = m_CurrSubscriberId;
        m_CurrSubscriberId++;

        m_Subscribers.insert(std::pair(id, callback));

        return id;
    }

    void WindowWeb::UnregisterWindowResizeCallback(int subscriberId) {
        if (m_Subscribers.find(subscriberId) == m_Subscribers.end()) {
            return;
        }

        m_Subscribers.erase(subscriberId);
    }

    int WindowWeb::OnWindowClose() {
        m_WindowPtr = nullptr;

        return 0;
    }

    void WindowWeb::Destroy() {
        m_WindowPtr = nullptr;
        m_Width = m_Height = m_RenderScale = 0;
    }
}
