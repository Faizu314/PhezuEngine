// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "glad/glad.h"
#include "Graphics/Core/Resources/Shader.hpp"
#include "Maths/Objects/Vector2.hpp"

namespace Phezu {

	class GLShader : public IShader {
	public:
		void Init(const std::string& vert, const std::string& frag, const std::unordered_map<VertexSemantic, unsigned int>& vertInput) override;
		void Bind() override;
		void Destroy() override;
	public:
		std::vector<VertexSemantic> GetRequiredSemantics() const override;
		unsigned int GetSemanticLocation(VertexSemantic semantic) const override;
	public:
		void SetInt(const std::string& uniformName, int color) override;
		void SetVec2(const std::string& uniformName, Vector2 value) override;
		void SetColor(const std::string& uniformName, Color color) override;
		void SetMat3(const std::string& uniformName, Mat3x3 mat) override;
	private:
		GLint GetUniformLocation(const std::string& uniformName);
	private:
		GLuint m_ShaderProgram = 0;
		std::unordered_map<VertexSemantic, unsigned int> m_Semantics;
		std::unordered_map<std::string, GLuint> m_UniformLocations;
	};
}
