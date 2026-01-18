#pragma once

#include "Assets/Core/Asset.hpp"

namespace Phezu {

	class MeshAsset;
	class ImageAsset;
	class TextureAsset;
	class ShaderAsset;
	class MaterialAsset;

	struct BuiltInAssets {

		static MeshAsset* CreateQuadMesh();

		static ImageAsset* CreateWhiteImage();

		static TextureAsset* CreateDefaultTexture();

		static ShaderAsset* CreateSpriteShader();

		static ShaderAsset* CreateBlitShader();

		static ShaderAsset* CreateSolidShader();

		static MaterialAsset* CreateSpriteMaterial();
	};

}