#include "Graphics/OpenGL/Resources/GLVertexBuffer.hpp"

namespace Phezu {


	void GLVertexBuffer::Init(const void* vertices, size_t bufferSize, BufferType bufferType) {
		glGenBuffers(1, &m_Buffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);

		GLenum usage = GL_STATIC_DRAW;

		if (bufferType == BufferType::Static)
			usage = GL_STATIC_DRAW;
		else if (bufferType == BufferType::Dynamic)
			usage = GL_DYNAMIC_DRAW;

		glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices, usage);
	}

	void GLVertexBuffer::Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
	}

	void GLVertexBuffer::Destroy() {
		if (m_Buffer != 0)
			glDeleteBuffers(1, &m_Buffer);
		m_Buffer = 0;
	}
}
