#include "Core/Window.hpp"

#ifdef _WIN32
#include "Platform/Win32/WindowWin32.hpp"
#endif

namespace Phezu {
		IWindow* CreateWindow(const WindowArgs& args) {
#ifdef _WIN32
		return new WindowWin32(args);
#elif __APPLE__
		return nullptr;
#endif
	}
}