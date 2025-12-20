#pragma once

#include <unordered_map>
#include <string>

#include "glad/glad.h"

#include "Graphics/Core/Resources/Shader.hpp"

namespace Phezu {

	class GLShader : public IShader {
	public:
		void Init(const std::string& vert, const std::string& frag) override;
		void Bind() override;
		void Destroy() override;
	public:
		void SetSemantics(const std::unordered_map<VertexSemantic, unsigned int>& semantics);
		std::vector<VertexSemantic> GetRequiredSemantics() const override;
		unsigned int GetSemanticLocation(VertexSemantic semantic) const override;
	public:
		void SetColor(const std::string& uniformName, Color color) override;
		void SetMat3(const std::string& uniformName, Mat3x3 mat) override;
	private:
		GLuint m_ShaderProgram = 0;
		std::unordered_map<VertexSemantic, unsigned int> m_Semantics;
		std::unordered_map<std::string, GLuint> m_UniformLocations;
	};
}
