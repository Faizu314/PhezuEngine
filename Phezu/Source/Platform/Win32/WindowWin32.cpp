#include "Platform/Win32/WindowWin32.hpp"
#include "Core/Logger.hpp"

#include "string.h"

namespace Phezu {

    static WindowWin32* m_This = nullptr;

    void PrintLastWinError(const char* msg)
    {
        DWORD error = GetLastError();

        LPVOID buffer;
        DWORD size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&buffer,
            0,
            nullptr
        );

        if (size && buffer)
        {
            Log("%s: (%lu) %s\n", msg, error, buffer);
            LocalFree(buffer);
        }
    }

    LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
            case WM_SIZE:
            {
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);

                return m_This->OnWindowResize(msg, width, height);
            }
            case WM_MOVE:
            {
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);

                return m_This->OnWindowMove(msg, width, height);
            }
            case WM_CLOSE:
            {
                return m_This->OnWindowClose();
            }
            default:
                return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }

    int WindowWin32::Init(const WindowArgs& args) {
        m_This = this;
        m_Width = args.Width;
        m_Height = args.Height;
        m_RenderScale = args.RenderScale;

        HINSTANCE hInstance = GetModuleHandle(NULL);

        const char CLASS_NAME[] = "MyWindowClass";

        WNDCLASS wc = { };

        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszClassName = CLASS_NAME;

        ATOM atom = RegisterClass(&wc);

        if (!atom) {
            PrintLastWinError("Unable to register class");
            return -1;
        }

        m_WindowPtr = CreateWindowEx(
            0,
            CLASS_NAME,
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
            PrintLastWinError("Unable to open window");
            return -1;
        }

        ShowWindow(m_WindowPtr, SW_SHOWNORMAL);

        return 0;
    }

    void WindowWin32::Update() {
        MSG msg;

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    int WindowWin32::OnWindowMove(UINT flag, int width, int height) {
        return 0;
    }

    int WindowWin32::OnWindowResize(UINT flag, int width, int height) {
        return 0;
    }

    int WindowWin32::OnWindowClose() {
        DestroyWindow(m_WindowPtr);

        return 0;
    }

    void WindowWin32::Destroy() {
        DestroyWindow(m_WindowPtr);
        m_WindowPtr = nullptr;
        m_Width = m_Height = m_RenderScale = 0;
    }
}
