// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#if defined(PZ_PLATFORM_WINDOWS)
	#include "glad/glad.h"
#elif defined(PZ_PLATFORM_WEB)
	#include <GLES3/gl3.h>
#endif

#include "Graphics/Core/Resources/Shader.hpp"
#include "Maths/Objects/Vector2.hpp"

namespace Phezu {

	class GLShader : public IShader {
	public:
		void Init(const std::string& vert, const std::string& frag, const std::unordered_map<VertexSemantic, unsigned int>& vertInput) override;
		void Reload(const std::string& vert, const std::string& frag, const std::unordered_map<VertexSemantic, unsigned int>& vertInput) override;
		void Bind() override;
		void Destroy() override;
	public:
		std::vector<VertexSemantic> GetRequiredSemantics() const override;
		unsigned int GetSemanticLocation(VertexSemantic semantic) const override;
	public:
		void SetInt(const std::string& uniformName, int color) override;
		void SetFloat(const std::string& uniformName, float value) override;
		void SetVec2(const std::string& uniformName, Vector2 value) override;
		void SetColor(const std::string& uniformName, Color color) override;
		void SetMat3(const std::string& uniformName, Mat3x3 mat) override;
	private:
		enum class ShaderProgramType {
			Vertex,
			Fragment
		};
	private:
		unsigned int CompileShaderProgram(const std::string& src, ShaderProgramType type, int& success);
		unsigned int CreateShaderProgram(unsigned int vert, unsigned int frag, int& success);
		GLint GetUniformLocation(const std::string& uniformName);
	private:
		GLuint m_ShaderProgram = 0;
		std::unordered_map<VertexSemantic, unsigned int> m_Semantics;
		std::unordered_map<std::string, GLuint> m_UniformLocations;
	};
}
