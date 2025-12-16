#pragma once

#include "glad/glad.h"

#include "Graphics/Core/Resources/VertexBuffer.hpp"

namespace Phezu {

	class GLVertexBuffer : public IVertexBuffer {
	public:
		void Init(const float* vertices, size_t bufferSize, BufferType bufferType) override;
		void Bind() override;
		void Destroy() override;
	private:
		GLuint m_Buffer = 0;
	};
}
