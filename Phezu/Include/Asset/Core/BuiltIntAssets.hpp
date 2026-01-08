#pragma once

#include "Asset/Types/ImageAsset.hpp"
#include "Asset/Types/TextureAsset.hpp"
#include "Asset/Types/ShaderAsset.hpp"
#include "Asset/Types/MeshAsset.hpp"

namespace Phezu {

	struct BuiltInAssets {

		static MeshAsset* CreateQuadMesh() {
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

			MeshAsset* meshAsset = new MeshAsset(static_cast<int>(BuiltInAssetType::QuadMesh));
			meshAsset->VertexBuffers.push_back(positionBuffer);
			meshAsset->VertexBuffers.push_back(uvsBuffer);
			meshAsset->IndexBuffer = { 0, 2, 1, 0, 3, 2 };
			meshAsset->Stride = GetVertexAttributeSize(VertexAttributeType::Float) * 4;
			meshAsset->VertexCount = 4;

			return meshAsset;
		}

		static ImageAsset* CreateWhiteImage() {
			Color* color = new Color(255, 255, 255, 255);

			ImageAsset* imageAsset = new ImageAsset(static_cast<int>(BuiltInAssetType::WhiteImage));

			imageAsset->Data = reinterpret_cast<unsigned char*>(color);
			imageAsset->Width = 1;
			imageAsset->Height = 1;

			return imageAsset;
		}

		static TextureAsset* CreateDefaultTexture(AssetHandle imageRef) {
			TextureAsset* textureAsset = new TextureAsset(static_cast<int>(BuiltInAssetType::DefaultTexture));

			textureAsset->ImageRef = imageRef;
			textureAsset->FilteringMode = TextureFilteringMode::Point;
			textureAsset->WrapMode = TextureWrapMode::Repeat;

			return textureAsset;
		}

		static ShaderAsset* CreateSpriteShader() {
			ShaderAsset* shaderAsset = new ShaderAsset(static_cast<int>(BuiltInAssetType::SpriteShader));

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

		static ShaderAsset* CreateBlitShader() {
			ShaderAsset* shaderAsset = new ShaderAsset(static_cast<int>(BuiltInAssetType::BlitShader));

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
					gl_Position = vec4(pos.xy, 0.0, 1.0);
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

		static ShaderAsset* CreateSolidShader() {
			ShaderAsset* shaderAsset = new ShaderAsset(static_cast<int>(BuiltInAssetType::SolidShader));

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
	};

}