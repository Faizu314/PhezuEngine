// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "Core/Platform.hpp"

#if defined(PZ_DEBUG)
	#if defined(PZ_PLATFORM_WINDOWS)
		#define PZ_ASSERT(X, ...) if (!(##X)) { Phezu::Log(__VA_ARGS__); __debugbreak(); }
	#elif defined(PZ_PLATFORM_WEB)
		#define PZ_ASSERT(X, ...) if (!(X)) { Phezu::Log(__VA_ARGS__); __builtin_trap(); }
	#endif
#endif

#if defined(PZ_RELEASE)
	#define PZ_ASSERT(X, ...)
#endif