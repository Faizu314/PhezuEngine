#include "Platform/Win32/PlatformWin32.hpp"
#include "Platform/Win32/WindowWin32.hpp"
#include "Platform/Win32/InputWin32.hpp"
#include "Platform/Win32/LoggerWin32.hpp"

#include <Windows.h>
#include "glad/khrplatform.h"
#include "glad/glad_wgl.h"

namespace Phezu {
	
	static WindowWin32* s_Window;
	static InputWin32* s_Input;
	static LoggerWin32* s_Logger;

	LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
			case WM_CREATE:
			{
				return DefWindowProc(hwnd, msg, wParam, lParam);
			}
			case WM_SIZE:
			{
				int width = LOWORD(lParam);
				int height = HIWORD(lParam);

				return s_Window->OnWindowResize(msg, width, height);
			}
			case WM_MOVE:
			{
				return s_Window->OnWindowMove();
			}
			case WM_CLOSE:
			{
				return s_Window->OnWindowClose();
			}
			default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}

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


	PlatformWin32::PlatformWin32() : m_hInstance(nullptr) {
		s_Window = new WindowWin32();
		s_Input = new InputWin32();
		s_Logger = new LoggerWin32();
	}

	int PlatformWin32::Init(const WindowArgs& args) {
		s_Logger->Init();

		m_hInstance = GetModuleHandle(NULL);

		const char CLASS_NAME[] = "MyWindowClass";

		WNDCLASS wc = { };

		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = m_hInstance;
		wc.lpszClassName = CLASS_NAME;

		ATOM atom = RegisterClass(&wc);

		if (!atom) {
			PrintLastWinError("Unable to register class");
			return -1;
		}

		int error = s_Window->Init(args, CLASS_NAME, m_hInstance);

		if (error != 0)
			return error;

		s_Input->Init();

		CreateGraphicsContext();

		return 0;
	}

	void PlatformWin32::CreateGraphicsContext() {
		HDC hdc = s_Window->GetDeviceContext();

		PIXELFORMATDESCRIPTOR pfd = {};
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int pixelFormat = ChoosePixelFormat(hdc, &pfd);

		SetPixelFormat(hdc, pixelFormat, &pfd);

		HGLRC tempContext = wglCreateContext(hdc);
		wglMakeCurrent(hdc, tempContext);

		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
			(PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

		int attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 5,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		HGLRC glContext = wglCreateContextAttribsARB(hdc, 0, attribs);

		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(tempContext);
		wglMakeCurrent(hdc, glContext);
	}

	void PlatformWin32::PollEvents() {
		MSG msg;

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	const InputData& PlatformWin32::GetInput() {
		return s_Input->GetInput();
	}

	void PlatformWin32::Update() {
		s_Window->Update();
	}

	void* PlatformWin32::GetOpenGLFunctionLoader() {
		return wglGetProcAddress;
	}

	IWindow* PlatformWin32::GetWindow() {
		return s_Window;
	}

	void PlatformWin32::Log(const char* msg, va_list args) {
		s_Logger->Log(msg, args);
	}

	void PlatformWin32::Destroy() {
		s_Window->Destroy();
		s_Input->Destroy();
		//m_Logger->Destroy();
	}
}