#pragma once

#include <string>
#include <unordered_map>

#include "Core/Types/Types.hpp"
#include "Maths/Objects/Mat3x3.hpp"

namespace Phezu {

	class IGraphicsAPI;
	class IShader;
	class ITexture;

	class Material {
	public:
		Material() = default;
	public:
		Material(const Material&) = delete;
		Material& operator=(const Material&) = delete;
		Material(Material&& other) = delete;
		Material& operator=(Material&& other) = delete;
	public:
		void Init(IShader* shader, std::unordered_map<std::string, ITexture*> textures);
		void Bind();
		void Destroy();
	public:
		void SetProperty(const std::string& propertyName, const MaterialProperty& property);
		void SetMat3(const std::string& matName, Mat3x3 matrix);
		IShader* GetShader() { return m_Shader; }
	private:
		void ApplyProperty(const std::string& propertyName, const MaterialProperty& property);
	private:
		IGraphicsAPI* m_Api = nullptr;
	private:
		IShader* m_Shader = nullptr;
		std::unordered_map<std::string, ITexture*> m_Textures;
		std::unordered_map<std::string, MaterialProperty> m_Properties;
	};

}