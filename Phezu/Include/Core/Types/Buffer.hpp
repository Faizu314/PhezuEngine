#pragma once

namespace Phezu {

	class Buffer {
	public:
		Buffer() = default;
	public:
		void Create(size_t size);
		void Write(const void* data, size_t writeSize, size_t startByteIndex);
		void Destroy();
	private:
		void* m_Ptr = nullptr;
	};
}
