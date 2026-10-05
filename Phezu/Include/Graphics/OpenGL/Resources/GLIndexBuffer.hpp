// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#if defined(PZ_PLATFORM_WINDOWS)
	#include "glad/glad.h"
#elif defined(PZ_PLATFORM_WEB)
	#include <GLES3/gl3.h>
#endif

#include "Graphics/Core/Graphics.hpp"
#include "Graphics/Core/Resources/IndexBuffer.hpp"

namespace Phezu {

	class GLIndexBuffer : public IIndexBuffer {
	public:
		void Init(const unsigned int* indices, size_t bufferSize, BufferType bufferType) override;
		void Bind() override;
		void Destroy() override;
		unsigned int GetIndicesCount() override { return m_IndicesCount; }
	private:
		GLuint m_Buffer = 0;
		unsigned int m_IndicesCount = 0;
	};
}
