// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "Assets/Core/Asset.hpp"
#include "Graphics/Core/Graphics.hpp"

namespace Phezu {

	class ResourceRegistry {
	public:
		ResourceRegistry() : m_ResourceID(1) {}
	public:
		void DestroyAndClearRecords();
		ResourceHandle AddRecord(AssetHandle assetHandle, ResourceType type, void* resourcePtr);
		ResourceHandle AddRecord(ResourceType type, void* resourcePtr);
		void RemoveRecord(void* resourcePtr);
		bool Exists(AssetHandle assetHandle);
		void* GetResource(ResourceHandle resourceHandle);
		void* GetResource(AssetHandle assetHandle);
		ResourceHandle GetResourceHandle(void* resourcePtr);
		ResourceHandle GetResourceHandle(AssetHandle assetHandle);
		bool IsResourceHandleValid(ResourceHandle handle, ResourceType type);
	private:
		std::unordered_map<void*, ResourceMeta> m_PtrToMeta;
		std::unordered_map<ResourceHandle, void*> m_HandleToPtr;
		std::unordered_map<AssetHandle, ResourceMeta> m_AssetToMeta;
		std::unordered_map<ResourceHandle, AssetHandle> m_HandleToAsset;
	private:
		uint64_t m_ResourceID;
	};
}