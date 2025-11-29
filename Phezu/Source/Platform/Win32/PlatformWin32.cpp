#include "Platform/Win32/PlatformWin32.hpp"
#include "Platform/Win32/WindowWin32.hpp"
#include "Platform/Win32/InputWin32.hpp"
#include "Platform/Win32/LoggerWin32.hpp"

#include <Windows.h>

namespace Phezu {
	
	static WindowWin32* m_Window;
	static InputWin32* m_Input;
	static LoggerWin32* m_Logger;

	LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
			case WM_SIZE:
			{
				int width = LOWORD(lParam);
				int height = HIWORD(lParam);

				return m_Window->OnWindowResize(msg, width, height);
			}
			case WM_MOVE:
			{
				return m_Window->OnWindowMove();
			}
			case WM_CLOSE:
			{
				return m_Window->OnWindowClose();
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
		m_Window = new WindowWin32();
		m_Input = new InputWin32();
		m_Logger = new LoggerWin32();
	}

	int PlatformWin32::Init(const WindowArgs& args) {
		m_Logger->Init();

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

		int error = m_Window->Init(args, CLASS_NAME, m_hInstance);

		if (error != 0)
			return error;

		m_Input->Init();

		return 0;
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
		return m_Input->GetInput();
	}

	void PlatformWin32::Update() {
		m_Window->Update();
	}

	void PlatformWin32::Log(const char* msg, va_list args) {
		m_Logger->Log(msg, args);
	}

	void PlatformWin32::Destroy() {
		m_Window->Destroy();
		m_Input->Destroy();
		//m_Logger->Destroy();
	}
}