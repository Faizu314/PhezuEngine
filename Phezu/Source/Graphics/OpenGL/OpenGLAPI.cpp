#include "glad/glad.h"

#include "Graphics/OpenGL/OpenGLAPI.hpp"
#include "Graphics/OpenGL/Resources/GLVertexBuffer.hpp"
#include "Graphics/OpenGL/Resources/GLIndexBuffer.hpp"
#include "Graphics/OpenGL/Resources/GLVertexArray.hpp"
#include "Graphics/OpenGL/Resources/GLShader.hpp"
#include "Graphics/OpenGL/Resources/GLTexture.hpp"
#include "Core/Platform.hpp"

namespace Phezu {

	void OpenGLAPI::RenderTriangles(unsigned int indicesCount) {
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indicesCount), GL_UNSIGNED_INT, 0);
	}

	void OpenGLAPI::SetViewport(int x, int y, int width, int height) {
		glViewport(
			static_cast<GLint>(x),
			static_cast<GLint>(y),
			static_cast<GLint>(width),
			static_cast<GLint>(height)
		);
	}

	void OpenGLAPI::ClearFrame(Color color) {
		GLfloat r = color.r / 255.0f;
		GLfloat g = color.g / 255.0f;
		GLfloat b = color.b / 255.0f;
		GLfloat a = color.a / 255.0f;

		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	IShader* OpenGLAPI::CreateShader(const std::string& vert, const std::string& frag, const std::unordered_map<VertexSemantic, unsigned int>& vertInput) {
		IShader* shader = new GLShader();
		shader->Init(vert, frag, vertInput);
		return shader;
	}

	ITexture* OpenGLAPI::CreateTexture(unsigned char* data, unsigned int width, unsigned int height, SamplerDesc settings) {
		ITexture* texture = new GLTexture();
		texture->Init(data, width, height, settings);
		return texture;
	}

	IVertexBuffer* OpenGLAPI::CreateVertexBuffer(const void* vertices, size_t bufferSize, BufferType bufferType) {
		IVertexBuffer* buffer = new GLVertexBuffer();
		buffer->Init(vertices, bufferSize, bufferType);
		return buffer;
	}

	IIndexBuffer* OpenGLAPI::CreateIndexBuffer(const unsigned int* indices, size_t bufferSize, BufferType bufferType) {
		IIndexBuffer* buffer = new GLIndexBuffer();
		buffer->Init(indices, bufferSize, bufferType);
		return buffer;
	}

	IVertexArray* OpenGLAPI::CreateVertexArray() {
		IVertexArray* vertexArray = new GLVertexArray();
		vertexArray->Init();
		return vertexArray;
	}
}