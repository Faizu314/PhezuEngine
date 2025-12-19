#include "Graphics/OpenGL/Resources/GLIndexBuffer.hpp"

namespace Phezu {

	void GLIndexBuffer::Init(const unsigned int* indices, size_t bufferSize, BufferType bufferType) {
		glGenBuffers(1, &m_Buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);

		GLenum usage = GL_STATIC_DRAW;

		if (bufferType == BufferType::Static)
			usage = GL_STATIC_DRAW;
		else if (bufferType == BufferType::Dynamic)
			usage = GL_DYNAMIC_DRAW;

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, indices, usage);

		m_IndicesCount = bufferSize / sizeof(unsigned int);
	}

	void GLIndexBuffer::Bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffer);
	}

	void GLIndexBuffer::Destroy() {
		if (m_Buffer != 0)
			glDeleteBuffers(1, &m_Buffer);
		m_Buffer = 0;
	}
}