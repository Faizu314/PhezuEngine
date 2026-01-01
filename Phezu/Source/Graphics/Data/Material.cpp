#include "Core/Platform.hpp"
#include "Graphics/Core/Resources/Shader.hpp"
#include "Graphics/Core/Resources/Texture.hpp"
#include "Graphics/Data/Material.hpp"

namespace Phezu {

	void Material::Init(IShader* shader, std::unordered_map<std::string, ITexture*> textures) {
		m_Shader = shader;
		m_Textures = textures;
	}

	void Material::Bind() {
		m_Shader->Bind();
		
		int texUnit = 0;

		for (auto& texProperty : m_Textures) {
			const std::string& texName = texProperty.first;
			m_Shader->SetInt(texName, texUnit);
			texProperty.second->Bind(texUnit);
		}

		for (auto& property : m_Properties) {
			ApplyProperty(property.first, property.second);
		}
	}

	MaterialProperty Material::GetProperty(const std::string& propertyName) {
		if (m_Properties.find(propertyName) != m_Properties.end()) {
			return m_Properties.at(propertyName);
		}

		return MaterialProperty();
	}

	void Material::SetProperty(const std::string& propertyName, const MaterialProperty& property) {
		if (m_Properties.find(propertyName) != m_Properties.end()) {
			auto prop = m_Properties.at(propertyName);

			if (prop.Type == property.Type && prop.Value == property.Value)
				return;
		}

		ApplyProperty(propertyName, property);

		m_Properties[propertyName] = property;
	}

	void Material::SetMat3(const std::string& propertyName, Mat3x3 matrix) {
		m_Shader->SetMat3(propertyName, matrix);
	}

	void Material::ApplyProperty(const std::string& propertyName, const MaterialProperty& property) {
		switch (property.Type) {
			case MaterialPropertyType::Color:
				m_Shader->SetColor(propertyName, std::get<Color>(property.Value));
				break;
			case MaterialPropertyType::Int:
				m_Shader->SetInt(propertyName, std::get<int>(property.Value));
				break;
			default: {
				Log("Should assert here: material property apply case not implemented\n");
				return;
			}
		}
	}

}