// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "Graphics/Core/GraphicsAPI.hpp"

namespace Phezu {

	class OpenGLAPI : public IGraphicsAPI {
	public:
		void SetViewport(int x, int y, int width, int height) override;
		void ClearFrame(Color color) override;
		void SetRenderTarget(IFrameBuffer* fbo) override;
		void RenderTriangles(unsigned int indicesCount) override;
	public:
		IVertexBuffer* CreateVertexBuffer(const void* vertices, size_t bufferSize, BufferType bufferType) override;
		IIndexBuffer* CreateIndexBuffer(const unsigned int* indices, size_t bufferSize, BufferType bufferType) override;
		IVertexArray* CreateVertexArray() override;
		IShader* CreateShader(const std::string& vert, const std::string& frag, const std::unordered_map<VertexSemantic, unsigned int>& vertInput) override;
		ITexture* CreateTexture(unsigned char* data, unsigned int width, unsigned int height, SamplerDesc settings) override;
		IFrameBuffer* CreateFrameBuffer() override;
	};
}