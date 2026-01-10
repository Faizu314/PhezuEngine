#include "Asset/Core/BuiltInAssets.hpp"
#include "Asset/Types/ImageAsset.hpp"
#include "Asset/Types/TextureAsset.hpp"
#include "Asset/Types/ShaderAsset.hpp"
#include "Asset/Types/MeshAsset.hpp"
#include "Asset/Types/MaterialAsset.hpp"

namespace Phezu {

	MeshAsset* BuiltInAssets::CreateQuadMesh() {
		std::vector<float> positions = {
			-0.5f, -0.5f,
			-0.5f,  0.5f,
				0.5f,  0.5f,
				0.5f, -0.5f
		};

		SerializedBuffer positionBuffer = {
			VertexSemantic::Position,
			VertexAttributeType::Float,
			VertexAttributeCount::Two,
			false,
			positions
		};

		std::vector<float> uvs = {
			0, 0,
			0, 1,
			1, 1,
			1, 0
		};

		SerializedBuffer uvsBuffer = {
			VertexSemantic::TexCoord0,
			VertexAttributeType::Float,
			VertexAttributeCount::Two,
			false,
			uvs
		};

		MeshAsset* meshAsset = new MeshAsset();
		meshAsset->VertexBuffers.push_back(positionBuffer);
		meshAsset->VertexBuffers.push_back(uvsBuffer);
		meshAsset->IndexBuffer = { 0, 2, 1, 0, 3, 2 };
		meshAsset->Stride = GetVertexAttributeSize(VertexAttributeType::Float) * 4;
		meshAsset->VertexCount = 4;

		return meshAsset;
	}

	ImageAsset* BuiltInAssets::CreateWhiteImage() {
		Color* color = new Color(255, 255, 255, 255);

		ImageAsset* imageAsset = new ImageAsset();

		imageAsset->Data = reinterpret_cast<unsigned char*>(color);
		imageAsset->Width = 1;
		imageAsset->Height = 1;

		return imageAsset;
	}

	TextureAsset* BuiltInAssets::CreateDefaultTexture() {
		TextureAsset* textureAsset = new TextureAsset();

		textureAsset->ImageRef = AssetHandle(static_cast<int>(BuiltInAssetType::WhiteImage), AssetSource::Engine);
		textureAsset->FilteringMode = TextureFilteringMode::Point;
		textureAsset->WrapMode = TextureWrapMode::Repeat;

		return textureAsset;
	}

	ShaderAsset* BuiltInAssets::CreateSpriteShader() {
		ShaderAsset* shaderAsset = new ShaderAsset();

		shaderAsset->Semantics = {
			{ VertexSemantic::Position, 0 },
			{ VertexSemantic::TexCoord0, 1 }
		};
		shaderAsset->VertexSource = R"(
			#version 460 core

			layout (location = 0) in vec2 pos;
			layout (location = 1) in vec2 texCoord;
	
			uniform mat3 objectToWorld;
			uniform mat3 worldToView;
			uniform mat3 viewToScreen;

			out vec2 uv;

			void main() {
				vec3 worldPos = objectToWorld * vec3(pos.xy, 1.0);
				vec3 viewPos = worldToView* vec3(worldPos.xy, 1.0);
				vec3 screenPos = viewToScreen * vec3(viewPos.xy, 1.0);

				gl_Position = vec4(screenPos.xy, 0.0, 1.0);
				uv = texCoord;
			}
		)";

		shaderAsset->FragmentSource = R"(
			#version 460 core

			in vec2 uv;

			out vec4 FragColor;

			uniform sampler2D mainTex;
			uniform vec4 tint;

			void main() {
				FragColor = texture(mainTex, uv) * tint;
			}
		)";

		return shaderAsset;
	}

	ShaderAsset* BuiltInAssets::CreateBlitShader() {
		ShaderAsset* shaderAsset = new ShaderAsset();

		shaderAsset->Semantics = {
			{ VertexSemantic::Position, 0 },
			{ VertexSemantic::TexCoord0, 1 }
		};
		shaderAsset->VertexSource = R"(
			#version 460 core

			layout (location = 0) in vec2 pos;
			layout (location = 1) in vec2 texCoord;

			out vec2 uv;

			void main() {
				gl_Position = vec4(pos.x * 2.0, pos.y * 2.0, 0.0, 1.0);
				uv = texCoord;
			}
		)";

		shaderAsset->FragmentSource = R"(
			#version 460 core

			in vec2 uv;

			out vec4 FragColor;

			uniform sampler2D mainTex;

			void main() {
				FragColor = texture(mainTex, uv);
			}
		)";

		return shaderAsset;
	}

	ShaderAsset* BuiltInAssets::CreateSolidShader() {
		ShaderAsset* shaderAsset = new ShaderAsset();

		shaderAsset->Semantics = {
			{ VertexSemantic::Position, 0 }
		};
		shaderAsset->VertexSource = R"(
			#version 460 core

			layout (location = 0) in vec2 pos;

			void main() {
				gl_Position = vec4(pos.xy, 0.0, 1.0);
			}
		)";

		shaderAsset->FragmentSource = R"(
			#version 460 core

			out vec4 FragColor;

			uniform vec4 tint;

			void main() {
				FragColor = tint;
			}
		)";

		return shaderAsset;
	}

	MaterialAsset* BuiltInAssets::CreateSpriteMaterial() {
		MaterialAsset* materialAsset = new MaterialAsset();

		materialAsset->ShaderRef = AssetHandle(static_cast<int>(BuiltInAssetType::SpriteShader), AssetSource::Engine);
		materialAsset->Textures["mainTex"] = AssetHandle(static_cast<int>(BuiltInAssetType::DefaultTexture), AssetSource::Engine);
		materialAsset->Properties["tint"] = { MaterialPropertyType::Color, Color::White };

		return materialAsset;
	}
}