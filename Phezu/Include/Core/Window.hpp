#pragma once

#include <string>

namespace Phezu {
	
	struct WindowArgs {
		std::string Name;
		int Width;
		int Height;
		int RenderScale;
	};

	class IWindow {
	public:
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual int GetRenderScale() const = 0;
	};

	IWindow* CreateWindow(const WindowArgs& args);
}