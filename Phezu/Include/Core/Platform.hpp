// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <cstdarg>
#include <string>

#include "Maths/Objects/Vector2.hpp"

namespace Phezu {

	struct WindowArgs {
		std::string Name;
		int Width;
		int Height;
		int RenderScale;
	};

	struct InputData {
		bool W, A, S, D;
		bool Up, Right, Down, Left;
		bool Space, F, G, H;
		Vector2 MousePos;
		bool LeftMouse, RightMouse, MiddleMouse;
	};

	class IWindow;
	class IGraphicsAPI;

	class IPlatform {
	public:
		virtual void Init(const WindowArgs& args) = 0;
		virtual void Destroy() = 0;
	public:
		virtual void PollEvents() = 0;
		virtual void Update() = 0;
		virtual void Wait(float delay) = 0;
		virtual double GetTime() = 0;
	public:
		virtual IWindow* GetWindow() = 0;
		virtual IGraphicsAPI* GetGraphicsApi() = 0;
	public:
		virtual void Log(const char* msg, va_list args) = 0;
		virtual const InputData& GetInput() = 0;
	};

	void Log(const char* msg, ...);
	IPlatform* CreatePlatform();
}