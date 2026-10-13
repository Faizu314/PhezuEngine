#include "Core/Defs/Assert.hpp"
#include "Core/Platform.hpp"
#include "Graphics/OpenGL/Resources/GLShader.hpp"
#include "Maths/Objects/Mat3x3.hpp"

namespace Phezu {
	
	void GLShader::Init(const std::string& vert, const std::string& frag, const std::unordered_map<VertexSemantic, unsigned int>& vertInput) {
		int success;
		unsigned int vertexShader = CompileShaderProgram(vert, ShaderProgramType::Vertex, success);

		PZ_ASSERT(success, "Invalid shader asset\n");

		unsigned int fragmentShader = CompileShaderProgram(frag, ShaderProgramType::Fragment, success);

		PZ_ASSERT(success, "Invalid shader asset\n");

		m_ShaderProgram = CreateShaderProgram(vertexShader, fragmentShader, success);

		PZ_ASSERT(success, "Unable to link shader programs\n");

		m_Semantics = vertInput;
	}

	void GLShader::Reload(const std::string& vert, const std::string& frag, const std::unordered_map<VertexSemantic, unsigned int>& vertInput) {
		int success;
		unsigned int vertexShader = CompileShaderProgram(vert, ShaderProgramType::Vertex, success);

		if (!success) {
			glDeleteShader(vertexShader);
			return;
		}

		unsigned int fragmentShader = CompileShaderProgram(frag, ShaderProgramType::Fragment, success);

		if (!success) {
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			return;
		}

		unsigned int newShaderProgram = CreateShaderProgram(vertexShader, fragmentShader, success);

		if (success == GL_FALSE) {
			glDeleteProgram(newShaderProgram);
			return;
		}

		glDeleteProgram(m_ShaderProgram);
		m_ShaderProgram = newShaderProgram;

		m_UniformLocations.clear();
		m_Semantics = vertInput;
	}

	void GLShader::Bind() {
		glUseProgram(m_ShaderProgram);
	}

	void GLShader::Destroy() {
		glDeleteProgram(m_ShaderProgram);
		m_ShaderProgram = 0;
	}

	std::vector<VertexSemantic> GLShader::GetRequiredSemantics() const {
		std::vector<VertexSemantic> semantics;

		for (auto& kvp : m_Semantics) {
			semantics.push_back(kvp.first);
		}

		return semantics;
	}

	unsigned int GLShader::GetSemanticLocation(VertexSemantic semantic) const {
		PZ_ASSERT(m_Semantics.find(semantic) != m_Semantics.end(), "Semantic does not exist in shader.\n");

		return m_Semantics.at(semantic);
	}

	unsigned int GLShader::CreateShaderProgram(unsigned int vert, unsigned int frag, int& success) {
		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vert);
		glAttachShader(shaderProgram, frag);
		glLinkProgram(shaderProgram);

		char infoLog[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			Log("OpenGL Shader Linking Error: %s\n", infoLog);
		}

		glDetachShader(shaderProgram, vert);
		glDetachShader(shaderProgram, frag);
		glDeleteShader(vert);
		glDeleteShader(frag);

		return shaderProgram;
	}

	unsigned int GLShader::CompileShaderProgram(const std::string& src, ShaderProgramType type, int& success) {
		const char* shaderSource = src.c_str();

		unsigned int shaderProgram;
		shaderProgram = glCreateShader(type == ShaderProgramType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
		glShaderSource(shaderProgram, 1, &shaderSource, NULL);
		glCompileShader(shaderProgram);

		char infoLog[512];
		glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
			Log("OpenGL ShaderProgramType value (%i) Compilation Error: %s\n", static_cast<int>(type), infoLog);
			return shaderProgram;
		}

		return shaderProgram;
	}

	GLint GLShader::GetUniformLocation(const std::string& uniformName) {
		GLint location;

		if (m_UniformLocations.find(uniformName) == m_UniformLocations.end()) {
			location = glGetUniformLocation(m_ShaderProgram, uniformName.c_str());
			m_UniformLocations.insert(std::pair(uniformName, location));
		}
		else {
			location = m_UniformLocations[uniformName];
		}

		return location;
	}

	void GLShader::SetInt(const std::string& uniformName, int value) {
		GLint location = GetUniformLocation(uniformName);

		glUniform1i(location, static_cast<GLint>(value));
	}

	void GLShader::SetFloat(const std::string& uniformName, float value) {
		GLint location = GetUniformLocation(uniformName);

		glUniform1f(location, static_cast<GLfloat>(value));
	}

	void GLShader::SetVec2(const std::string& uniformName, Vector2 value) {
		GLint location = GetUniformLocation(uniformName);

		glUniform2f(location, static_cast<GLfloat>(value.X()), static_cast<GLfloat>(value.Y()));
	}

	void GLShader::SetColor(const std::string& uniformName, Color color) {
		GLint location = GetUniformLocation(uniformName);

		glUniform4f(location, color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	}

	void GLShader::SetMat3(const std::string& uniformName, Mat3x3 mat) {
		GLint location = GetUniformLocation(uniformName);

		glUniformMatrix3fv(location, 1, GL_FALSE, mat.GetPtr());
	}

}