#pragma once

#include <cstdarg>

namespace Phezu {


	class ILogger {
	public:
		virtual void Init() = 0;
		virtual void Log(const char* msg, va_list args) = 0;
	};

	ILogger* CreateLogger();
	void Log(const char* msg, ...);
}