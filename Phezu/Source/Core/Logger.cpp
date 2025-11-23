#include "Core/Logger.hpp"

#ifdef _WIN32
#include "Platform/Win32/LoggerWin32.hpp"
#elif __APPLE__
#include "Platform/Mac/LoggerMac.hpp"
#endif

#include <iostream>
#include <cstdarg>

namespace Phezu {

	ILogger* s_Logger = nullptr;

	ILogger* CreateLogger() {
#ifdef _WIN32
		s_Logger = new LoggerWin32();
#elif __APPLE__
		s_Logger = new LoggerMac();
#endif

		return s_Logger;
	}

	void Log(const char* msg, ...) {
		va_list args;

		va_start(args, msg);

		s_Logger->Log(msg, args);

		va_end(args);
	}
}