#include "Graphics/Systems/ResourceRegistry.hpp"
#include "Core/Defs/Assert.hpp"
#include "Core/Platform.hpp"
#include "Graphics/Core/Resources/Shader.hpp"
#include "Graphics/Core/Resources/Texture.hpp"
#include "Graphics/Data/Mesh.hpp"

namespace Phezu {

    void ResourceRegistry::DestroyAndClearRecords() {
        for (auto kvp : m_PtrToMeta) {
            switch (kvp.second.GetType()) {
                case ResourceType::Shader:
                {
                    static_cast<IShader*>(kvp.first)->Destroy();
                    break;
                }
                case ResourceType::Mesh: 
                {
                    static_cast<Mesh*>(kvp.first)->Destroy();
                    break;
                }
                case ResourceType::Texture:
                {
                    static_cast<ITexture*>(kvp.first)->Destroy();
                    break;
                }
                default:
                {
                    PZ_ASSERT(kvp.second.GetType() != ResourceType::None, "ResourceType should not be none\n");
                }
            }

            delete kvp.first;
        }

        m_PtrToMeta.clear();
        m_HandleToPtr.clear();
        m_AssetToMeta.clear();
        m_HandleToAsset.clear();
    }

    ResourceHandle ResourceRegistry::AddRecord(AssetHandle assetHandle, ResourceType type, void* resourcePtr) {
        ResourceHandle resourceHandle = AddRecord(type, resourcePtr);
        ResourceMeta resourceMeta(resourceHandle, type);

        m_HandleToAsset.insert(std::make_pair(resourceHandle, assetHandle));
        m_AssetToMeta.insert(std::make_pair(assetHandle, resourceMeta));

        return resourceHandle;
    }

    ResourceHandle ResourceRegistry::AddRecord(ResourceType type, void* resourcePtr) {
        while (m_HandleToPtr.find(m_ResourceID) != m_HandleToPtr.end() || m_ResourceID == INVALID_GUID)
            m_ResourceID++;

        ResourceMeta meta(m_ResourceID, type);
        m_HandleToPtr.insert(std::pair(m_ResourceID, resourcePtr));
        m_PtrToMeta.insert(std::pair(resourcePtr, meta));

        return m_ResourceID;
    }

    void ResourceRegistry::RemoveRecord(void* resourcePtr) {
        if (m_PtrToMeta.find(resourcePtr) == m_PtrToMeta.end()) {
            //TODO: Log Warning
            Log("Trying to remove a record that does not exist\n");
            return;
        }

        ResourceMeta resourceMeta = m_PtrToMeta.at(resourcePtr);
        ResourceHandle resourceHandle = resourceMeta.GetHandle();

        m_PtrToMeta.erase(resourcePtr);
        m_HandleToPtr.erase(resourceHandle);

        if (m_HandleToAsset.find(resourceHandle) == m_HandleToAsset.end())
            return;

        AssetHandle assetHandle = m_HandleToAsset.at(resourceHandle);
        m_HandleToAsset.erase(resourceHandle);
        m_AssetToMeta.erase(assetHandle);
    }

    bool ResourceRegistry::Exists(AssetHandle assetHandle) {
        return m_AssetToMeta.find(assetHandle) != m_AssetToMeta.end();
    }

    void* ResourceRegistry::GetResource(ResourceHandle resourceHandle) {
        PZ_ASSERT(m_HandleToPtr.find(ResourceMeta(resourceHandle)) != m_HandleToPtr.end(), "Resource does not exist.\n");

        return m_HandleToPtr.at(resourceHandle);
    }

    void* ResourceRegistry::GetResource(AssetHandle assetHandle) {
        PZ_ASSERT(m_AssetToMeta.find(ResourceMeta(assetHandle)) != m_AssetToMeta.end(), "Resource does not exist.\n");

        return m_HandleToPtr.at(m_AssetToMeta.at(assetHandle).GetHandle());
    }

    ResourceHandle ResourceRegistry::GetResourceHandle(void* resourcePtr) {
        PZ_ASSERT(m_PtrToMeta.find(resourcePtr) != m_PtrToMeta.end(), "Resource does not exist.\n");

        return m_PtrToMeta.at(resourcePtr).GetHandle();
    }

    ResourceHandle ResourceRegistry::GetResourceHandle(AssetHandle assetHandle) {
        PZ_ASSERT(m_AssetToMeta.find(ResourceMeta(assetHandle)) != m_AssetToMeta.end(), "Resource does not exist.\n");

        return m_AssetToMeta.at(assetHandle).GetHandle();
    }

}