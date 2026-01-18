#include "Platform/Win32/WindowWin32.hpp"
#include "Platform/Win32/PlatformWin32.hpp"

namespace Phezu {

    void WindowWin32::Init(const WindowArgs& args, const char winClassName[], HINSTANCE hInstance) {
        m_Width = args.Width;
        m_Height = args.Height;
        m_RenderScale = args.RenderScale;

        m_WindowPtr = CreateWindowEx(
            0,
            winClassName,
            "Phezu Runtime",
            WS_OVERLAPPEDWINDOW,

            CW_USEDEFAULT, CW_USEDEFAULT, m_Width, m_Height,

            NULL,
            NULL,
            hInstance,
            NULL
        );

        if (m_WindowPtr == nullptr)
        {
            Log("Close program here\n");
            PrintLastWinError("Unable to open window");
            return;
        }

        ShowWindow(m_WindowPtr, SW_SHOWNORMAL);
    }

    void WindowWin32::Update() {

    }

    int WindowWin32::OnWindowMove() {
        return 0;
    }

    int WindowWin32::OnWindowResize(UINT flag, int width, int height) {
        m_Width = width;
        m_Height = height;

        for (auto sub : m_Subscribers) {
            sub.second(m_Width, m_Height);
        }

        return 0;
    }

    int WindowWin32::RegisterWindowResizeCallback(const ResizeCallback& callback) {
        int id = m_CurrSubscriberId;
        m_CurrSubscriberId++;

        m_Subscribers.insert(std::pair(id, callback));

        return id;
    }

    void WindowWin32::UnregisterWindowResizeCallback(int subscriberId) {
        if (m_Subscribers.find(subscriberId) == m_Subscribers.end()) {
            return;
        }

        m_Subscribers.erase(subscriberId);
    }

    int WindowWin32::OnWindowClose() {
        DestroyWindow(m_WindowPtr);
        m_WindowPtr = nullptr;

        return 0;
    }

    void WindowWin32::Destroy() {
        if (m_WindowPtr != nullptr)
            DestroyWindow(m_WindowPtr);
        m_WindowPtr = nullptr;
        m_Width = m_Height = m_RenderScale = 0;
    }
}
