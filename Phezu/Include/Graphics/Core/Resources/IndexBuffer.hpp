#pragma once

namespace Phezu {
	
	class IIndexBuffer {
	public:
		virtual void Init(const unsigned int* indices, size_t bufferSize) = 0;
		virtual void Bind() = 0;
	};
}
