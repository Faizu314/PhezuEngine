#include "Core/Types/Buffer.hpp"
#include "Graphics/Core/GraphicsAPI.hpp"
#include "Graphics/Core/Resources/Shader.hpp"
#include "Graphics/Core/Resources/Texture.hpp"
#include "Graphics/Data/ResourceManager.hpp"
#include "Graphics/Data/Mesh.hpp"
#include "Graphics/Data/Material.hpp"
#include "Graphics/Core/Descriptors/VertexLayout.hpp"

namespace Phezu {

    void ResourceManager::Init(AssetManager* assetManager, IGraphicsAPI* api) {
        m_AssetManager = assetManager;
        m_Api = api;
    }

    void ResourceManager::Destroy() {
        for (auto& mesh : m_Meshes) {
            mesh.second->Destroy();
        }
        for (auto& mat : m_Materials) {
            mat.second->Destroy();
        }
        for (auto& shader : m_Shaders) {
            shader.second->Destroy();
        }
        for (auto& texture : m_Textures) {
            texture.second->Destroy();
        }
    }

    const Mesh* ResourceManager::GetMesh(AssetHandle<MeshAsset> meshHandle) {
        if (m_Meshes.find(meshHandle.GetGuid()) != m_Meshes.end())
            return m_Meshes.at(meshHandle.GetGuid());

        const MeshAsset* meshAsset = m_AssetManager->GetAsset(meshHandle);
        Mesh* mesh = CreateMesh(meshAsset);
        m_Meshes.insert(std::make_pair(meshHandle.GetGuid(), mesh));

        return mesh;
    }

    Material* ResourceManager::GetMaterial(AssetHandle<MaterialAsset> materialHandle) {
        if (m_Materials.find(materialHandle.GetGuid()) != m_Materials.end())
            return m_Materials.at(materialHandle.GetGuid());

        const MaterialAsset* materialAsset = m_AssetManager->GetAsset(materialHandle);
        Material* material = CreateMaterial(materialAsset);
        m_Materials.insert(std::make_pair(materialHandle.GetGuid(), material));

        return material;
    }

    ITexture* ResourceManager::GetTexture(AssetHandle<TextureAsset> textureHandle) {
        if (m_Materials.find(textureHandle.GetGuid()) != m_Materials.end())
            return m_Textures.at(textureHandle.GetGuid());

        const TextureAsset* textureAsset = m_AssetManager->GetAsset(textureHandle);
        ITexture* texture = CreateTexture(textureAsset);
        m_Textures.insert(std::make_pair(textureHandle.GetGuid(), texture));

        return texture;
    }

    IShader* ResourceManager::GetShader(AssetHandle<ShaderAsset> shaderHandle) {
        if (m_Shaders.find(shaderHandle.GetGuid()) != m_Shaders.end())
            return m_Shaders.at(shaderHandle.GetGuid());

        const ShaderAsset* shaderAsset = m_AssetManager->GetAsset(shaderHandle);
        IShader* shader = CreateShader(shaderAsset);
        m_Shaders.insert(std::make_pair(shaderHandle.GetGuid(), shader));

        return shader;
    }

	Mesh* ResourceManager::CreateMesh(const MeshAsset* meshAsset)
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

    Material* ResourceManager::CreateMaterial(const MaterialAsset* materialAsset) {
        GUID shaderGuid = materialAsset->ShaderRef;

        IShader* shader = GetShader({ shaderGuid });

        std::unordered_map<std::string, ITexture*> textures;

        for (auto& kvp : materialAsset->Textures) {
            const std::string& texName = kvp.first;
            GUID texGuid = kvp.second;
            ITexture* texture = GetTexture({ texGuid });
            textures.insert(std::make_pair(texName, texture));
        }

        Material* mat = new Material();

        mat->Init(shader, textures);

        return mat;
    }

    IShader* ResourceManager::CreateShader(const ShaderAsset* shaderAsset) {
        IShader* shader = m_Api->CreateShader(shaderAsset->VertexSource, shaderAsset->FragmentSource, shaderAsset->Semantics);

        return shader;
    }

    ITexture* ResourceManager::CreateTexture(const TextureAsset* textureAsset) {
        AssetHandle<ImageAsset> imageHandle = { textureAsset->ImageRef };
        const ImageAsset* imageAsset = m_AssetManager->GetAsset(imageHandle);

        SamplerDesc desc = {
            textureAsset->WrapMode,
            textureAsset->FilteringMode
        };

        ITexture* texture = m_Api->CreateTexture(imageAsset->Data, imageAsset->Width, imageAsset->Height, desc);

        return texture;
    }
}
