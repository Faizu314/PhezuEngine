#pragma once

#include "Core/Types/Buffer.hpp"
#include "Core/Types/VertexTypes.hpp"
#include "Graphics/Core/GraphicsTypes.hpp"
#include "Graphics/Data/Mesh.hpp"
#include "Asset/Types/MeshAsset.hpp"
#include "Asset/Types/ShaderAsset.hpp"

namespace Phezu {

	class IGraphicsAPI;
	class IShader;

	class MeshBuilder {
	public:
		static Mesh CreateMesh(const MeshAsset* meshAsset, IGraphicsAPI* api);
		static IShader* CreateShader(const ShaderAsset* shaderAsset, IGraphicsAPI* api);
	};

}