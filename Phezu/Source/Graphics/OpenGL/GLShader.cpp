#include "Graphics/OpenGL/GLShader.hpp"
#include "Core/Platform.hpp"
#include "glad/glad.h"

namespace Phezu {
	
	void GLShader::Init(const std::string& vert, const std::string& frag) {
		const char* vertSource = vert.c_str();
		const char* fragSource = frag.c_str();

		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertSource, NULL);
		glCompileShader(vertexShader);

		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			Log("OpenGL Vertex Shader Compilation Error: %s", infoLog);
			return;
		}

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			Log("OpenGL Fragment Shader Compilation Error: %s", infoLog);
			return;
		}

		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			Log("OpenGL Shader Linking Error: %s", infoLog);
			return;
		}

		m_ShaderProgram = shaderProgram;

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void GLShader::Bind() {
		glUseProgram(m_ShaderProgram);
	}

	void GLShader::SetVec4(const std::string& uniformName, Color color) {
		GLint location;

		if (m_UniformLocations.find(uniformName) == m_UniformLocations.end()) {
			location = glGetUniformLocation(m_ShaderProgram, uniformName.c_str());
			m_UniformLocations.insert(std::pair(uniformName, location));
		}
		else {
			location = m_UniformLocations[uniformName];
		}

		glUniform4f(location, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	}

}