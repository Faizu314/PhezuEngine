#pragma once

#include <string>

#include "Core/Types/GUID.hpp"

namespace Phezu {
	
	template<typename T>
	struct AssetHandle {
	public:
		AssetHandle(GUID guid) : m_Guid(guid) {}
		GUID GetGuid() { return m_Guid; }
	private:
		GUID m_Guid;
	};

	class IAsset {
	public:
		virtual void Deserialize(const std::string& data) = 0;
	};

}