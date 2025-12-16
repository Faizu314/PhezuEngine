#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "glad/glad.h"
#include "glad/glad_wgl.h"

#include "Platform/Win32/PlatformWin32.hpp"
#include "Platform/Win32/WindowWin32.hpp"
#include "Platform/Win32/InputWin32.hpp"
#include "Platform/Win32/LoggerWin32.hpp"

#include "Graphics/Core/GraphicsAPI.hpp"
#include "Graphics/OpenGL/OpenGLAPI.hpp"

namespace Phezu {
	
	static WindowWin32* s_Window = nullptr;
	static InputWin32* s_Input = nullptr;
	static LoggerWin32* s_Logger = nullptr;
	static IGraphicsAPI* s_GraphicsApi = nullptr;

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


	PlatformWin32::PlatformWin32() : m_hInstance(nullptr), m_WindowHandle(nullptr) {
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

		m_WindowHandle = GetDC(s_Window->GetWindowPtr());

		s_Input->Init();

		CreateGraphicsContext();

		return 0;
	}

	void PlatformWin32::CreateGraphicsContext() {
		PIXELFORMATDESCRIPTOR desiredPixelFormat = {};
		desiredPixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		desiredPixelFormat.nVersion = 1;
		desiredPixelFormat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		desiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
		desiredPixelFormat.cColorBits = 32;
		desiredPixelFormat.cAlphaBits = 8;
		desiredPixelFormat.iLayerType = PFD_MAIN_PLANE;

		int pixelFormatIndex = ChoosePixelFormat(m_WindowHandle, &desiredPixelFormat);

		if (pixelFormatIndex == 0) {
			PrintLastWinError("Unable to choose pixel format");
		}

		PIXELFORMATDESCRIPTOR pixelFormat;

		if (DescribePixelFormat(m_WindowHandle, pixelFormatIndex, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormat) == 0) {
			PrintLastWinError("Failed to describe pixel format");
		}

		if (SetPixelFormat(m_WindowHandle, pixelFormatIndex, &pixelFormat) != TRUE) {
			PrintLastWinError("Unable to set pixel format");
		}

		HGLRC tempContext = wglCreateContext(m_WindowHandle);

		if (!tempContext) {
			PrintLastWinError("Unable to create wgl context");
		}

		if (wglMakeCurrent(m_WindowHandle, tempContext) != TRUE) {
			PrintLastWinError("Unable to make context current");
		}

		gladLoadWGL(m_WindowHandle);

		int attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 5,
			WGL_CONTEXT_FLAGS_ARB,
			WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0
		};

		HGLRC glContext = wglCreateContextAttribsARB(m_WindowHandle, 0, attribs);

		if (!glContext) {
			PrintLastWinError("Unable to create wgl context");
		}

		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(tempContext);

		if (!wglMakeCurrent(m_WindowHandle, glContext)) {
			PrintLastWinError("Unable to make context current");
		}

		if (!gladLoadGL()) {
			Phezu::Log("Unable to load glad\n");
		}

		s_GraphicsApi = new OpenGLAPI();
	}

	void PlatformWin32::PollEvents() {
		MSG msg;

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	IWindow* PlatformWin32::GetWindow() {
		return s_Window;
	}

	const InputData& PlatformWin32::GetInput() {
		return s_Input->GetInput();
	}

	IGraphicsAPI* PlatformWin32::GetGraphicsApi() {
		return s_GraphicsApi;
	}


	void PlatformWin32::Update() {
		s_Window->Update();
		SwapBuffers(m_WindowHandle);
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