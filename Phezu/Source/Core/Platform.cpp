#include "Core/Platform.hpp"

#if defined(PZ_PLATFORM_WINDOWS)
#include "Platform/Win32/PlatformWin32.hpp"
#endif

namespace Phezu {

	static IPlatform* m_Platform = nullptr;

	IPlatform* CreatePlatform() {
#if defined(PZ_PLATFORM_WINDOWS)
		if (m_Platform != nullptr)
			return m_Platform;
		m_Platform = new PlatformWin32();
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