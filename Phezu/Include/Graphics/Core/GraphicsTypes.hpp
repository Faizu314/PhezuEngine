#pragma once

#include <stdint.h>

namespace Phezu {

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

	struct VertexAttribute {
		VertexAttribute(VertexAttributeType type, VertexAttributeCount count, bool normalized)
			: AttributeType(type), AttributeCount(count), Normalized(normalized) {}

		VertexAttributeType AttributeType;
		VertexAttributeCount AttributeCount;
		bool Normalized;
	};

	enum class BufferType {
		Static,
		Dynamic
	};
}