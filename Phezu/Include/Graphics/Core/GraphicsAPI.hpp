#pragma once

#include <unordered_map>
#include <string>

#include "Core/Platform.hpp"
#include "Core/Types/Color.hpp"
#include "Core/Types/Types.hpp"
#include "Graphics/Core/GraphicsTypes.hpp"

namespace Phezu {

	class IShader;
	class IVertexBuffer;
	class IIndexBuffer;
	class IVertexArray;
	class ITexture;
	class IFrameBuffer;

	class IGraphicsAPI {
	public:
		virtual void SetViewport(int x, int y, int width, int height) = 0;
		virtual void ClearFrame(Color color) = 0;
		virtual void SetRenderTarget(IFrameBuffer* fbo) = 0;
		virtual void RenderTriangles(unsigned int indicesCount) = 0;
	public:
		virtual IVertexBuffer* CreateVertexBuffer(const void* vertices, size_t bufferSize, BufferType bufferType) = 0;
		virtual IIndexBuffer* CreateIndexBuffer(const unsigned int* indices, size_t bufferSize, BufferType bufferType) = 0;
		virtual IVertexArray* CreateVertexArray() = 0;
		virtual IShader* CreateShader(const std::string& vert, const std::string& frag, const std::unordered_map<VertexSemantic, unsigned int>& vertInput) = 0;
		virtual ITexture* CreateTexture(unsigned char* data, unsigned int width, unsigned int height, SamplerDesc settings) = 0;
		virtual IFrameBuffer* CreateFrameBuffer() = 0;
	};
}