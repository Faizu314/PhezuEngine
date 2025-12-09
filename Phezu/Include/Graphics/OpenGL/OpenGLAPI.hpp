#pragma once

#include "Graphics/GraphicsAPI.hpp"

namespace Phezu {

	class OpenGLAPI : public IGraphicsAPI {
	public:
		void SetViewport(int x, int y, int width, int height) override;
	public:
		void ClearFrame(Color color) override;
		void RenderBox(Vector2 downLeftScreen, Vector2 upRightScreen, Color tint) override;
	};
}