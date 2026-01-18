#pragma once

#include "Windows.h"
#include <chrono>
#include <cstdarg>

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
		void Update() override;
		void Wait(float delay) override;
		double GetTime() override;
	public:
		IWindow* GetWindow() override;
		IGraphicsAPI* GetGraphicsApi() override;
	public:
		void Log(const char* msg, va_list args) override;
		const InputData& GetInput() override;
	private:
		void CreateGraphicsContext();
	private:
		HINSTANCE m_hInstance;
		HDC m_WindowHandle;
		std::chrono::steady_clock::time_point m_StartTime;
	};

	void PrintLastWinError(const char* msg);
}