#pragma once

#include "Core/Types/Buffer.hpp"
#include "Core/Types/VertexTypes.hpp"
#include "Graphics/Core/GraphicsTypes.hpp"
#include "Graphics/Data/Mesh.hpp"
#include "Asset/Types/MeshAsset.hpp"

namespace Phezu {

	class IGraphicsAPI;

	class MeshBuilder {
	public:
		static Mesh CreateMesh(const MeshAsset* meshAsset, IGraphicsAPI* api);
	};

}