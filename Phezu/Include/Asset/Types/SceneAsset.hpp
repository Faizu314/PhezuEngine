#pragma once

#include "Asset/Blueprint/Blueprint.hpp"

namespace Phezu {

	class SceneAsset {
    public:
        SceneAsset() = default;
    public:
        GUID GetGuid() { return m_Guid; }
    public:
        void Deserialize(const std::string& data);
    public:
        const Blueprint& GetBlueprint() const { return m_Blueprint; }
        std::string GetName() const { return m_Name; }
	private:
		GUID m_Guid;
        std::string m_Name;
		Blueprint m_Blueprint;
	};

}