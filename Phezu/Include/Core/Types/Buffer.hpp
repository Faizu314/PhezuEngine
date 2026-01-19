// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <cstddef>

namespace Phezu {

	class Buffer {
	public:
		Buffer() = default;
	public:
		void Create(size_t size);
		void Write(const void* data, size_t writeSize, size_t startByteIndex);
		void Destroy();
	public:
		const void* GetData() const { return m_Ptr; }
		size_t GetSize() const { return m_Size; }
	private:
		std::byte* m_Ptr = nullptr;
		size_t m_Size = 0;
	};
}
