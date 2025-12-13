#pragma once

#include "Graphics/GraphicsAPI.hpp"

namespace Phezu {

	class IShader;

	class OpenGLAPI : public IGraphicsAPI {
	public:
		void SetViewport(int x, int y, int width, int height) override;
	public:
		void ClearFrame(Color color) override;
		void RenderBox(Vector2 downLeftScreenNormalized, Vector2 upRightScreenNormalized, Color tint) override;
		IShader* CreateShader(const std::string& vert, const std::string& frag) override;
	};
}