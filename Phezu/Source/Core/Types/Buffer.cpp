#pragma once

#include "Core/Types/Buffer.hpp"
#include "Core/Platform.hpp"

namespace Phezu {

	void Buffer::Create(size_t size) {
		m_Ptr = new std::byte[size];
		m_Size = size;
	}

	void Buffer::Write(const void* data, size_t writeSize, size_t startByteIndex) {
		if (startByteIndex + writeSize <= m_Size) {
			Log("Should assert here\n");
			return;
		}

		const std::byte* byteData = static_cast<const std::byte*>(data);

		for (size_t i = 0; i < writeSize; i++) {
			m_Ptr[startByteIndex + i] = byteData[i];
		}
	}

	void Buffer::Destroy() {
		if (m_Ptr != nullptr) {
			delete m_Ptr;
			m_Ptr = nullptr;
			m_Size = 0;
		}
	}
}
