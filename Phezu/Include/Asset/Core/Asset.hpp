#pragma once

#include <string>

#include "Core/Types/GUID.hpp"

namespace Phezu {
	
	template<typename T>
	struct AssetHandle {
	public:
		AssetHandle() = default;
		AssetHandle(GUID guid) : m_Guid(guid) {}
		GUID GetGuid() { return m_Guid; }
	private:
		GUID m_Guid = 0;
	};

	class IAsset {
	public:
		IAsset() = delete;
		IAsset(GUID guid) : Guid(guid) {}
	public:
		virtual void Deserialize(const std::string& data) = 0;
	public:
		GUID Guid;
	};

}