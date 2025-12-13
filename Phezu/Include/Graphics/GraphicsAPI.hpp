#pragma once

#include "Maths/Math.hpp"
#include "Core/Types/Color.hpp"
#include "Core/Platform.hpp"

#include <string>

namespace Phezu {

	class IShader;

	class IGraphicsAPI {
	public:
		virtual void SetViewport(int x, int y, int width, int height) = 0;
	public:
		virtual void ClearFrame(Color color) = 0;
		virtual void RenderBox(Vector2 downLeftScreen, Vector2 upRightScreen, Color tint) = 0;
	public:
		virtual IShader* CreateShader(const std::string& vert, const std::string& frag) = 0;
	};
}