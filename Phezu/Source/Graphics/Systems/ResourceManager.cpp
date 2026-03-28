#include "Core/Types/Buffer.hpp"
#include "Graphics/Core/GraphicsAPI.hpp"
#include "Graphics/Core/Resources/Shader.hpp"
#include "Graphics/Core/Resources/Texture.hpp"
#include "Graphics/Systems/ResourceManager.hpp"
#include "Graphics/Data/Mesh.hpp"
#include "Graphics/Data/Material.hpp"
#include "Graphics/Core/Descriptors/VertexLayout.hpp"
#include "Core/Utils/EnumUtils.hpp"

namespace Phezu {

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


    ResourceHandle ResourceManager::CreateMaterial(ResourceHandle sourceMaterialID) {
        Material* sourceMat = static_cast<Material*>(m_Resources.GetResource(sourceMaterialID));
        Material* sourceCopy = sourceMat->Copy();

        m_Resources.AddRecord(ResourceType::Material, sourceCopy);

        return m_Resources.GetResourceHandle(sourceCopy);
    }

    void ResourceManager::DestroyResource(ResourceHandle materialID) {
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