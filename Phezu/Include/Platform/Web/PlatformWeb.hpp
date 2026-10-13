// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "Core/Platform.hpp"

namespace Phezu {

	class IWindow;
	class IGraphicsAPI;

	class PlatformWeb : public IPlatform {
	public:
		PlatformWeb();
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
		double m_StartTime;
	};
}