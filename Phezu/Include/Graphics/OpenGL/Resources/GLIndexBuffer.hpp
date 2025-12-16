#pragma once

#include "glad/glad.h"

#include "Graphics/Core/GraphicsTypes.hpp"
#include "Graphics/Core/Resources/IndexBuffer.hpp"

namespace Phezu {

	class GLIndexBuffer : public IIndexBuffer {
	public:
		void Init(const unsigned int* indices, size_t bufferSize, BufferType bufferType) override;
		void Bind() override;
		void Destroy() override;
	private:
		GLuint m_Buffer = 0;
	};
}
