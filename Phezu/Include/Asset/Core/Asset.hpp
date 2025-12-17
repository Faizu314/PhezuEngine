#pragma once

#include <string>

#include "Core/Types/GUID.hpp"

namespace Phezu {
	
	template<typename T>
	struct AssetHandle {
	public:
		AssetHandle(GUID guid) : Guid(guid) {}
		GUID GetGuid() { return Guid; }
	private:
		GUID Guid;
	};

	class IAsset {
	public:
		virtual void Deserialize(const std::string& data) = 0;
	};

}