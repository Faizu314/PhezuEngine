#include "Graphics/Systems/ResourceRegistry.hpp"
#include "Core/Defs/Assert.hpp"
#include "Core/Platform.hpp"
#include "Graphics/Core/Resources/Shader.hpp"
#include "Graphics/Core/Resources/Texture.hpp"
#include "Graphics/Data/Mesh.hpp"

namespace Phezu {

    void ResourceRegistry::DestroyAndClearRecords() {
        for (auto kvp : m_PtrToHandle) {
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

        m_PtrToHandle.clear();
        m_HandleToPtr.clear();
        m_AssetToResource.clear();
        m_ResourceToAsset.clear();
    }

    uint64_t ResourceRegistry::AddRecord(AssetHandle assetHandle, ResourceType type, void* resourcePtr) {
        uint64_t resourceId = AddRecord(type, resourcePtr);
        ResourceHandle resourceHandle(resourceId, type);

        m_ResourceToAsset.insert(std::make_pair(resourceHandle, assetHandle));
        m_AssetToResource.insert(std::make_pair(assetHandle, resourceHandle));

        return resourceId;
    }

    uint64_t ResourceRegistry::AddRecord(ResourceType type, void* resourcePtr) {
        while (m_HandleToPtr.find(ResourceHandle(m_ResourceID)) != m_HandleToPtr.end() || m_ResourceID == 0)
            m_ResourceID++;

        ResourceHandle handle(m_ResourceID, type);
        m_HandleToPtr.insert(std::pair(handle, resourcePtr));
        m_PtrToHandle.insert(std::pair(resourcePtr, handle));

        return m_ResourceID;
    }

    void ResourceRegistry::RemoveRecord(void* resourcePtr) {
        if (m_PtrToHandle.find(resourcePtr) == m_PtrToHandle.end()) {
            //TODO: Log Warning
            Log("Trying to remove a record that does not exist\n");
            return;
        }

        ResourceHandle resourceHandle = m_PtrToHandle.at(resourcePtr);
        m_PtrToHandle.erase(resourcePtr);
        m_HandleToPtr.erase(resourceHandle);

        if (m_ResourceToAsset.find(resourceHandle) == m_ResourceToAsset.end())
            return;

        AssetHandle assetHandle = m_ResourceToAsset.at(resourceHandle);
        m_ResourceToAsset.erase(resourceHandle);
        m_AssetToResource.erase(assetHandle);
    }

    bool ResourceRegistry::Exists(AssetHandle assetHandle) {
        return m_AssetToResource.find(assetHandle) != m_AssetToResource.end();
    }

    void* ResourceRegistry::GetResource(uint64_t resourceID) {
        PZ_ASSERT(m_HandleToPtr.find(ResourceHandle(resourceID)) != m_HandleToPtr.end(), "Resource does not exist.\n");

        return m_HandleToPtr.at(resourceID);
    }

    void* ResourceRegistry::GetResource(AssetHandle assetHandle) {
        PZ_ASSERT(m_AssetToResource.find(ResourceHandle(assetHandle)) != m_AssetToResource.end(), "Resource does not exist.\n");

        return m_HandleToPtr.at(m_AssetToResource.at(assetHandle));
    }

    uint64_t ResourceRegistry::GetResourceID(void* resourcePtr) {
        PZ_ASSERT(m_PtrToHandle.find(resourcePtr) != m_PtrToHandle.end(), "Resource does not exist.\n");

        return m_PtrToHandle.at(resourcePtr).GetID();
    }

    uint64_t ResourceRegistry::GetResourceID(AssetHandle assetHandle) {
        PZ_ASSERT(m_AssetToResource.find(ResourceHandle(assetHandle)) != m_AssetToResource.end(), "Resource does not exist.\n");

        return m_AssetToResource.at(assetHandle).GetID();
    }

}