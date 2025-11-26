#include "Core/Window.hpp"

#ifdef _WIN32
#include "Platform/Win32/WindowWin32.hpp"
#undef CreateWindow
#endif

namespace Phezu {
		IWindow* CreateWindow() {
#ifdef _WIN32
		return new WindowWin32();
#elif __APPLE__
		return nullptr;
#endif
	}
}