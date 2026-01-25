// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "Core/Platform.hpp"

#ifdef _WIN32
	#ifdef PZ_DEBUG
		#define PZ_ASSERT(X, ...) if (!(##X)) { Phezu::Log(__VA_ARGS__); __debugbreak(); }
	#endif
	#ifdef PZ_RELEASE
		#define PZ_ASSERT(X, ...)
	#endif
#endif