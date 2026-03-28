// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#include "Graphics/Core/Graphics.hpp"

namespace Phezu {

	class ResourceRegistry {
	public:
		ResourceRegistry() : m_ResourceID(1) {}
	public:
		void DestroyAndClearRecords();
		uint64_t AddRecord(AssetHandle assetHandle, ResourceType type, void* resourcePtr);
		uint64_t AddRecord(ResourceType type, void* resourcePtr);
		void RemoveRecord(void* resourcePtr);
		bool Exists(AssetHandle assetHandle);
		void* GetResource(uint64_t resourceID);
		void* GetResource(AssetHandle assetHandle);
		uint64_t GetResourceID(void* resourcePtr);
		uint64_t GetResourceID(AssetHandle assetHandle);
	private:
		std::unordered_map<void*, ResourceHandle> m_PtrToHandle;
		std::unordered_map<ResourceHandle, void*> m_HandleToPtr;
		std::unordered_map<AssetHandle, ResourceHandle> m_AssetToResource;
		std::unordered_map<ResourceHandle, AssetHandle> m_ResourceToAsset;
	private:
		uint64_t m_ResourceID;
	};
}