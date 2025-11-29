#pragma once

#include "Core/Platform.hpp"

namespace Phezu {

	class PlatformWin32 : public IPlatform {
	public:
		PlatformWin32();
	public:
		int Init(const WindowArgs& args) override;
		void Destroy() override;
	public:
		void PollEvents() override;
		const InputData& GetInput() override;
		void Update() override;
	public:
		void Log(const char* msg, va_list args) override;
	};

	void PrintLastWinError(const char* msg);
}