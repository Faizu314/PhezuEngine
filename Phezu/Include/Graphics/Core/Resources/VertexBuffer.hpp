#pragma once

#include "Graphics/Core/GraphicsTypes.hpp"

namespace Phezu {

	class IVertexBuffer {
	public:
		virtual void Init(const float* vertices, size_t bufferSize, BufferType bufferType) = 0;
		virtual void Bind() = 0;
		virtual void Destroy() = 0;
	};
}
