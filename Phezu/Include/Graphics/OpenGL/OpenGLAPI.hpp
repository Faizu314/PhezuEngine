#pragma once

#include "Graphics/Core/GraphicsAPI.hpp"

namespace Phezu {

	class OpenGLAPI : public IGraphicsAPI {
	public:
		void SetViewport(int x, int y, int width, int height) override;
	public:
		void ClearFrame(Color color) override;
		void RenderTriangles(unsigned int indicesCount) override;
	public:
		IVertexBuffer* CreateVertexBuffer(const void* vertices, size_t bufferSize, BufferType bufferType) override;
		IIndexBuffer* CreateIndexBuffer(const unsigned int* indices, size_t bufferSize, BufferType bufferType) override;
		IVertexArray* CreateVertexArray() override;
	public:
		IShader* CreateShader(const std::string& vert, const std::string& frag) override;
	};
}