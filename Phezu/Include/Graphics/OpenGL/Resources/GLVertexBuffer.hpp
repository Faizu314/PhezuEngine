// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "glad/glad.h"

#include "Graphics/Core/Resources/VertexBuffer.hpp"

namespace Phezu {

	class GLVertexBuffer : public IVertexBuffer {
	public:
		void Init(const void* vertices, size_t bufferSize, BufferType bufferType) override;
		void Bind() override;
		void Destroy() override;
	private:
		GLuint m_Buffer = 0;
	};
}
