#include "Core/Platform.hpp"

#if defined(PZ_PLATFORM_WINDOWS)
#include "Platform/Win32/PlatformWin32.hpp"
#elif defined(PZ_PLATFORM_WEB)
#include "Platform/Web/PlatformWeb.hpp"
#endif

namespace Phezu {

	static IPlatform* m_Platform = nullptr;

	IPlatform* CreatePlatform() {
		if (m_Platform != nullptr)
			return m_Platform;
#if defined(PZ_PLATFORM_WINDOWS)
		m_Platform = new PlatformWin32();
#elif defined(PZ_PLATFORM_WEB)
		m_Platform = new PlatformWeb();
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