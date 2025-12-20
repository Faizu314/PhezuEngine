#pragma once

#include <string>
#include <vector>

#include "Core/Types/Color.hpp"
#include "Core/Types/VertexTypes.hpp"

namespace Phezu {

	class IShader {
	public:
		virtual void Init(const std::string& vert, const std::string& frag) = 0;
		virtual void Bind() = 0;
		virtual void Destroy() = 0;
	public:
		virtual std::vector<VertexSemantic> GetRequiredSemantics() const = 0;
		virtual unsigned int GetSemanticLocation(VertexSemantic semantic) const = 0;
	public:
		virtual void SetColor(const std::string& uniformName, Color color) = 0;
		virtual void SetMat3(const std::string& uniformName, Mat3x3 mat) = 0;
	};
}