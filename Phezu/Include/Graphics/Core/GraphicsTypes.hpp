#pragma once

#include <stdint.h>

#include "Core/Types/Types.hpp"

namespace Phezu {

	struct ResourceHandle {
		uint64_t Ptr = 0;
	};

	struct VertexAttribute {
		VertexAttribute() = default;

		VertexAttribute(VertexSemantic semantic, VertexAttributeType type, VertexAttributeCount count, bool normalized = true)
			: Semantic(semantic), AttributeType(type), AttributeCount(count), Normalized(normalized) {}

		VertexSemantic Semantic;
		VertexAttributeType AttributeType;
		VertexAttributeCount AttributeCount;
		size_t Offset = 0;
		bool Normalized = true;
	};

	enum class BufferType {
		Static,
		Dynamic
	};

	class IFrameBuffer;

	struct RenderTarget {
		IFrameBuffer* Target;

		static RenderTarget Default() { return { nullptr }; }
		static RenderTarget Offscreen(IFrameBuffer* fbo) { return { fbo }; }
	};
}