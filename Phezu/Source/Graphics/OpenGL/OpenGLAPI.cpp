#pragma once

#include "glad/glad.h"

#include "Graphics/OpenGL/OpenGLAPI.hpp"
#include "Core/Platform.hpp"

namespace Phezu {

	void OpenGLAPI::RenderBox(Vector2 downLeftScreen, Vector2 upRightScreen, Color tint) {
		
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
		float r = color.r / 255.0f;
		float g = color.g / 255.0f;
		float b = color.b / 255.0f;
		float a = color.a / 255.0f;

		glClearColor(r, g, b, a);
	}


}