#pragma once

#include "glad/glad.h"

#include "Graphics/OpenGL/OpenGLAPI.hpp"
#include "Graphics/OpenGL/GLShader.hpp"
#include "Core/Platform.hpp"

namespace Phezu {

	void OpenGLAPI::RenderBox(Vector2 dl, Vector2 ur, Color tint) {
		float vertices[] = {
			 dl.X(), dl.Y(), 0.0f,
			 dl.X(), ur.Y(), 0.0f,
			 ur.X(), ur.Y(), 0.0f,
			 ur.X(), dl.Y(), 0.0f
		};

		unsigned int indices[] = {
			0, 2, 1,
			0, 3, 2
		};

		unsigned int vao;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned int vbo;
		
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		unsigned int ibo;

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
		glDeleteVertexArrays(1, &vao);
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
		glClear(GL_COLOR_BUFFER_BIT);
	}

	IShader* OpenGLAPI::CreateShader(const std::string& vert, const std::string& frag) {
		IShader* shader = new GLShader();
		shader->Init(vert, frag);
		return shader;
	}

}