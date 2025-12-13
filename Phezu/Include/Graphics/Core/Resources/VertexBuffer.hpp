#pragma once

namespace Phezu {

	class IVertexBuffer {
	public:
		virtual void Init(const unsigned int* vertices, size_t bufferSize) = 0;
		virtual void Bind() = 0;
	};
}
