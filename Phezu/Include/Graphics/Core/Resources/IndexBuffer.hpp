#pragma once

namespace Phezu {
	
	class IIndexBuffer {
	public:
		virtual void Init(const unsigned int* indices, size_t bufferSize, BufferType bufferType) = 0;
		virtual void Bind() = 0;
		virtual void Destroy() = 0;
	};
}
