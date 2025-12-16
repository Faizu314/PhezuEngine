#pragma once

#include "Windows.h"

#include "Core/Platform.hpp"

namespace Phezu {

	class IWindow;
	class IGraphicsAPI;

	class PlatformWin32 : public IPlatform {
	public:
		PlatformWin32();
	public:
		void Init(const WindowArgs& args) override;
		void Destroy() override;
	public:
		void PollEvents() override;
		const InputData& GetInput() override;
		void Update() override;
		IWindow* GetWindow() override;
		IGraphicsAPI* GetGraphicsApi() override;
		void Log(const char* msg, va_list args) override;
	private:
		void CreateGraphicsContext();
	private:
		HINSTANCE m_hInstance;
		HDC m_WindowHandle;
	};

	void PrintLastWinError(const char* msg);
}