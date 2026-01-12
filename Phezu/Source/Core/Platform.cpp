#include "Core/Platform.hpp"

#ifdef _WIN32
#include "Platform/Win32/PlatformWin32.hpp"
#endif

namespace Phezu {

	static IPlatform* m_Platform = nullptr;

	IPlatform* CreatePlatform() {
#ifdef _WIN32
		if (m_Platform != nullptr)
			return m_Platform;
		m_Platform = new PlatformWin32();
#elif __APPLE__
		m_Platform nullptr;
#endif

		return m_Platform;
	}

	void Log(const char* msg, ...) {
		va_list args;

		va_start(args, msg);

		m_Platform->Log(msg, args);

		va_end(args);
	}
}