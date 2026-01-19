// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <cstdint>

namespace Phezu {

	enum class VertexSemantic : uint8_t {
		Position,
		Normal,
		Color,
		TexCoord0,
		TexCoord1,
		TexCoord2,
		TexCoord3,
		Custom0,
		Custom1,
		Custom2,
		Custom3
	};

	enum class VertexAttributeType : uint8_t {
		Float,
		Int,
		UInt,
		Byte,
		UByte
	};

	enum class VertexAttributeCount : uint8_t {
		One = 1,
		Two = 2,
		Three = 3,
		Four = 4
	};
}