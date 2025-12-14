#include "Graphics/OpenGL/Resources/GLVertexArray.hpp"
#include "Core/Platform.hpp"

namespace Phezu {

	GLint GetVertexAttributeCount(VertexAttributeCount countEnum) {
		switch (countEnum) {
			case VertexAttributeCount::One:
				return 1;
			case VertexAttributeCount::Two:
				return 2;
			case VertexAttributeCount::Three:
				return 3;
			case VertexAttributeCount::Four:
				return 4;

			default: {
				Log("Should assert here\n");
				return 1;
			}
		}
	}

	GLuint GetVertexAttributeSize(const VertexAttribute& attribute) {
		int count = GetVertexAttributeCount(attribute.AttributeCount);

		switch (attribute.AttributeType) {
			case VertexAttributeType::Float:
				return sizeof(float) * count;
				
			case VertexAttributeType::Int:
				return sizeof(int) * count;
			case VertexAttributeType::UInt:
				return sizeof(unsigned int) * count;

			case VertexAttributeType::Byte:
			case VertexAttributeType::UByte:
				return count;

			default: {
				Log("Should assert here\n");
				return 1;
			}
		}
	}

	void GLVertexArray::Init() {
		glGenVertexArrays(1, &m_Array);
	}

	void GLVertexArray::Bind() {
		glBindVertexArray(m_Array);
	}

	void GLVertexArray::Destroy() {
		if (m_Array != 0)
			glDeleteVertexArrays(1, &m_Array);
		m_Array = 0;
	}

	void GLVertexArray::LinkVertexBuffer(IVertexBuffer* vertexBuffer, const VertexLayout& layout) {
		Bind();
		vertexBuffer->Bind();

		ApplyVertexLayout(layout);
	}

	void GLVertexArray::LinkIndexBuffer(IIndexBuffer* indexBuffer) {
		Bind();
		indexBuffer->Bind();
	}

	void GLVertexArray::ApplyVertexLayout(const VertexLayout& layout) {
		GLsizei stride = 0;
		GLuint offset = 0;

		for (int i = 0; i < layout.GetAttributesCount(); i++) {
			stride += GetVertexAttributeSize(layout.GetAttributeAt(i));
		}

		for (int i = 0; i < layout.GetAttributesCount(); i++) {
			VertexAttribute attrib = layout.GetAttributeAt(i);
			GLint count = GetVertexAttributeCount(attrib.AttributeCount);
			GLboolean normalized = attrib.Normalized ? GL_TRUE : GL_FALSE;

			switch (attrib.AttributeType) {
				case VertexAttributeType::Float:
					glVertexAttribPointer(i, count, GL_FLOAT, GL_FALSE, stride, (void*)(offset));
					break;

				case VertexAttributeType::Int:
					glVertexAttribIPointer(i, count, GL_INT, stride, (void*)(offset));
					break;

				case VertexAttributeType::UInt:
					glVertexAttribIPointer(i, count, GL_UNSIGNED_INT, stride, (void*)(offset));
					break;

				case VertexAttributeType::Byte:
					glVertexAttribPointer(i, count, GL_BYTE, normalized, stride, (void*)(offset));
					break;

				case VertexAttributeType::UByte:
					glVertexAttribPointer(i, count, GL_UNSIGNED_BYTE, normalized, stride, (void*)(offset));
					break;

				default: {
					Log("Should Assert Here\n");
				}
			}

			glEnableVertexAttribArray(i);

			offset += GetVertexAttributeSize(attrib);
		}
	}
}
