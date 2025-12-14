#pragma once

#include "Maths/Math.hpp"
#include "Core/Types/Color.hpp"
#include "Core/Platform.hpp"

#include "Graphics/Core/GraphicsTypes.hpp"

#include <string>

namespace Phezu {

	class IShader;
	class IVertexBuffer;
	class IIndexBuffer;
	class IVertexArray;

	class IGraphicsAPI {
	public:
		virtual void SetViewport(int x, int y, int width, int height) = 0;
	public:
		virtual void ClearFrame(Color color) = 0;
		virtual void RenderQuad(unsigned int indicesCount) = 0;
	public:
		virtual IVertexBuffer* CreateVertexBuffer(const float* vertices, size_t bufferSize, BufferType bufferType) = 0;
		virtual IIndexBuffer* CreateIndexBuffer(const unsigned int* indices, size_t bufferSize, BufferType bufferType) = 0;
		virtual IVertexArray* CreateVertexArray() = 0;
	public:
		virtual IShader* CreateShader(const std::string& vert, const std::string& frag) = 0;
	};
}