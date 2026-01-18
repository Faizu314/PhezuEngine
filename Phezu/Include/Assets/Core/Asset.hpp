#pragma once

#include <string>
#include <cstdint>

#include "Core/Types/GUID.hpp"

namespace Phezu {
	
	inline constexpr uint64_t INVALID_GUID = 0;

	enum class BuiltInAssetType {
		QuadMesh,
		WhiteImage,
		DefaultTexture,
		SpriteShader,
		BlitShader,
		SolidShader,
		SpriteMaterial
	};

	enum class AssetType {
		None,
		Scene,
		Prefab,
		Mesh,
		Shader,
		Image,
		Texture,
		Material,
		Config
	};

	enum class AssetSource {
		Engine = 0,
		Project
	};

	struct AssetHandle {
	public:
		AssetHandle() = default;
		AssetHandle(GUID guid, AssetSource source) : m_Guid(guid), m_Source(source) {}
	public:
		GUID GetGuid() const { return m_Guid; }
		AssetSource GetSource() const { return m_Source; }
	public:
		bool operator==(const AssetHandle& other) const { return m_Source == other.m_Source && m_Guid == other.m_Guid; }
	private:
		AssetSource m_Source = AssetSource::Engine;
		GUID m_Guid = INVALID_GUID;
	};

	class IAsset {
	public:
		virtual AssetType GetAssetType() = 0;
		virtual void Deserialize(const std::string& data) = 0;
	};

}

namespace std {
	template<>
	class hash<Phezu::AssetHandle> {
	public:
		size_t operator()(const Phezu::AssetHandle& h) const noexcept {
			return h.GetGuid().Value ^ uint64_t(h.GetSource()) << 63;
		}
	};
}