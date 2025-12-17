#pragma once

#include "Core/Platform.hpp"

namespace Phezu {

	enum class VertexSemantic : uint8_t {
		Position,
		Normal,
		Color,
		TexCoord0,
		TexCoord1,
		TexCoord2,
		TexCoord3,
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
