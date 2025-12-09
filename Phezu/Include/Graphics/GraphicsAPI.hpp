#pragma once

#include "Maths/Math.hpp"
#include "Core/Types/Color.hpp"
#include "Core/Platform.hpp"

namespace Phezu {

	class IGraphicsAPI {
	public:
		virtual void SetViewport(int x, int y, int width, int height) = 0;
	public:
		virtual void ClearFrame(Color color) = 0;
		virtual void RenderBox(Vector2 downLeftScreen, Vector2 upRightScreen, Color tint) = 0;
	};
}