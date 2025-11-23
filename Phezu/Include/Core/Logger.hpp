#pragma once

namespace Phezu {


	class ILogger {
	public:
		virtual void Init() = 0;
		virtual void Log(const char* msg, ...) = 0;
	};

	ILogger* CreateLogger();
	void Log(const char* msg, ...);
}