#include "Core/Input.hpp"

#ifdef _WIN32
#include "Platform/Win32/InputWin32.hpp"
#endif

namespace Phezu {

	IInput* CreateInput() {
#ifdef _WIN32
		return new InputWin32();
#elif __APPLE__
		return nullptr;
#endif
	}
}