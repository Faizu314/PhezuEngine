#include "Core/Types/Buffer.hpp"
#include "Graphics/Core/GraphicsAPI.hpp"
#include "Graphics/Core/Resources/Shader.hpp"
#include "Graphics/Core/Resources/Texture.hpp"
#include "Graphics/Systems/ResourceManager.hpp"
#include "Graphics/Data/Mesh.hpp"
#include "Graphics/Data/Material.hpp"
#include "Graphics/Core/Descriptors/VertexLayout.hpp"

namespace Phezu {

    void ResourceRegistry::DestroyAndClearRecords() {
        for (auto kvp : m_PtrToHandle)
            delete kvp.first;

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




    void ResourceManager::Init(AssetManager* assetManager, IGraphicsAPI* api) {
        m_AssetManager = assetManager;
        m_Api = api;
    }

    void ResourceManager::Destroy() {
        m_Resources.DestroyAndClearRecords();
    }

    Mesh* ResourceManager::GetMesh(AssetHandle meshHandle) {
        if (m_Resources.Exists(meshHandle))
            return static_cast<Mesh*>(m_Resources.GetResource(meshHandle));

        auto meshAsset = m_AssetManager->GetAsset<MeshAsset>(meshHandle);
        Mesh* mesh = CreateMeshFromAsset(meshAsset);
        m_Resources.AddRecord(meshHandle, ResourceType::Mesh, mesh);

        return mesh;
    }

    Material* ResourceManager::GetMaterial(AssetHandle materialHandle) {
        if (m_Resources.Exists(materialHandle))
            return static_cast<Material*>(m_Resources.GetResource(materialHandle));

        auto materialAsset = m_AssetManager->GetAsset<MaterialAsset>(materialHandle);
        Material* material = CreateMaterialFromAsset(materialAsset);
        m_Resources.AddRecord(materialHandle, ResourceType::Material, material);

        return material;
    }

    ITexture* ResourceManager::GetTexture(AssetHandle textureHandle) {
        if (m_Resources.Exists(textureHandle))
            return static_cast<ITexture*>(m_Resources.GetResource(textureHandle));

        auto textureAsset = m_AssetManager->GetAsset<TextureAsset>(textureHandle);
        ITexture* texture = CreateTextureFromAsset(textureAsset);
        m_Resources.AddRecord(textureHandle, ResourceType::Texture, texture);

        return texture;
    }

    IShader* ResourceManager::GetShader(AssetHandle shaderHandle) {
        if (m_Resources.Exists(shaderHandle))
            return static_cast<IShader*>(m_Resources.GetResource(shaderHandle));

        auto shaderAsset = m_AssetManager->GetAsset<ShaderAsset>(shaderHandle);
        IShader* shader = CreateShaderFromAsset(shaderAsset);
        m_Resources.AddRecord(shaderHandle, ResourceType::Shader, shader);

        return shader;
    }


    uint64_t ResourceManager::CreateMaterial(uint64_t sourceMaterialID) {
        Material* sourceMat = static_cast<Material*>(m_Resources.GetResource(sourceMaterialID));
        Material* sourceCopy = sourceMat->Copy();

        m_Resources.AddRecord(ResourceType::Material, sourceCopy);

        return m_Resources.GetResourceID(sourceCopy);
    }

    void ResourceManager::DestroyMaterial(uint64_t materialID) {
        Material* mat = static_cast<Material*>(m_Resources.GetResource(materialID));
        m_Resources.RemoveRecord(mat);
        delete mat;
    }

	Mesh* ResourceManager::CreateMeshFromAsset(const MeshAsset* meshAsset)
	{
        Buffer vertexBuffer;
        VertexLayout layout;

        size_t vertexCount = meshAsset->VertexCount;
        size_t vertexBufferSize = meshAsset->Stride * vertexCount;
        size_t indexBufferSize = sizeof(unsigned int) * meshAsset->IndexBuffer.size();

        vertexBuffer.Create(vertexBufferSize);
        size_t elementOffset = 0;

        for (const auto& buffer : meshAsset->VertexBuffers) {
            layout.Push({ buffer.Semantic, buffer.Type, buffer.Count, buffer.Normalized });

            size_t count = GetVertexAttributeCount(buffer.Count);
            size_t size = GetVertexAttributeSize(buffer.Type) * count;

            std::byte* castedBuffer = new std::byte[size * vertexCount];

            switch (buffer.Type) {
                case VertexAttributeType::Float: {
                    float* floatBuffer = reinterpret_cast<float*>(castedBuffer);

                    for (size_t i = 0; i < buffer.Data.size(); i++)
                        floatBuffer[i] = buffer.Data[i];

                    break;
                }
                case VertexAttributeType::Int: {
                    int* intBuffer = reinterpret_cast<int*>(castedBuffer);

                    for (size_t i = 0; i < buffer.Data.size(); i++)
                        intBuffer[i] = static_cast<int>(buffer.Data[i]);

                    break;
                }
                case VertexAttributeType::UInt: {
                    unsigned int* uIntBuffer = reinterpret_cast<unsigned int*>(castedBuffer);

                    for (size_t i = 0; i < buffer.Data.size(); i++)
                        uIntBuffer[i] = static_cast<unsigned int>(buffer.Data[i]);

                    break;
                }
                case VertexAttributeType::Byte: {
                    int8_t* byteBuffer = reinterpret_cast<int8_t*>(castedBuffer);

                    for (size_t i = 0; i < buffer.Data.size(); i++)
                        byteBuffer[i] = static_cast<int8_t>(buffer.Data[i]);

                    break;
                }
                case VertexAttributeType::UByte: {
                    uint8_t* uByteBuffer = reinterpret_cast<uint8_t*>(castedBuffer);

                    for (size_t i = 0; i < buffer.Data.size(); i++)
                        uByteBuffer[i] = static_cast<uint8_t>(buffer.Data[i]);

                    break;
                }
            }

            for (size_t i = 0; i < meshAsset->VertexCount; i++)
                vertexBuffer.Write(&castedBuffer[i * size], size, i * meshAsset->Stride + elementOffset);

            delete[] castedBuffer;

            elementOffset += size;
        }

        layout.ComputeLayout();

        Mesh* mesh = new Mesh();

        mesh->Init(m_Api);

        mesh->AttachVertexBuffer(vertexBuffer.GetData(), vertexBuffer.GetSize(), BufferType::Static, layout);
        mesh->AttachIndexBuffer(meshAsset->IndexBuffer.data(), indexBufferSize, BufferType::Static);

        mesh->UploadMeshData();

        vertexBuffer.Destroy();

		return mesh;
	}

    Material* ResourceManager::CreateMaterialFromAsset(const MaterialAsset* materialAsset) {
        AssetHandle shaderHandle = materialAsset->ShaderRef;

        IShader* shader = GetShader(shaderHandle);

        std::unordered_map<std::string, ITexture*> textures;

        for (auto& kvp : materialAsset->Textures) {
            const std::string& texName = kvp.first;
            AssetHandle texHandle = kvp.second;
            ITexture* texture = GetTexture(texHandle);
            textures.insert(std::make_pair(texName, texture));
        }

        Material* mat = new Material();

        mat->Init(shader, textures);
        mat->Bind();

        for (auto& kvp : materialAsset->Properties) {
            const std::string& propName = kvp.first;
            const MaterialProperty& property = kvp.second;
            mat->SetProperty(propName, property);
        }

        return mat;
    }

    IShader* ResourceManager::CreateShaderFromAsset(const ShaderAsset* shaderAsset) {
        IShader* shader = m_Api->CreateShader(shaderAsset->VertexSource, shaderAsset->FragmentSource, shaderAsset->Semantics);

        return shader;
    }

    ITexture* ResourceManager::CreateTextureFromAsset(const TextureAsset* textureAsset) {
        AssetHandle imageHandle = textureAsset->ImageRef;
        auto imageAsset = m_AssetManager->GetAsset<ImageAsset>(imageHandle);

        SamplerDesc desc = {
            textureAsset->WrapMode,
            textureAsset->FilteringMode
        };

        ITexture* texture = m_Api->CreateTexture(imageAsset->Data, imageAsset->Width, imageAsset->Height, desc);

        return texture;
    }
}