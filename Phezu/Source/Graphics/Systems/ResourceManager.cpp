#include "Core/Types/Buffer.hpp"
#include "Graphics/Core/GraphicsAPI.hpp"
#include "Graphics/Core/Resources/Shader.hpp"
#include "Graphics/Core/Resources/Texture.hpp"
#include "Graphics/Systems/ResourceManager.hpp"
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
            delete mesh.second;
        }
        m_Meshes.clear();

        for (auto& mat : m_Materials) {
            delete mat.second;
        }
        m_Materials.clear();

        for (auto& shader : m_Shaders) {
            shader.second->Destroy();
            delete shader.second;
        }
        m_Shaders.clear();

        for (auto& texture : m_Textures) {
            texture.second->Destroy();
            delete texture.second;
        }
        m_Textures.clear();
    }

    const Mesh* ResourceManager::GetMesh(AssetHandle meshHandle) {
        if (m_Meshes.find(meshHandle) != m_Meshes.end())
            return m_Meshes.at(meshHandle);

        auto meshAsset = m_AssetManager->GetAsset<MeshAsset>(meshHandle);
        Mesh* mesh = CreateMesh(meshAsset);
        m_Meshes.insert(std::make_pair(meshHandle, mesh));

        return mesh;
    }

    Material* ResourceManager::GetMaterial(AssetHandle materialHandle) {
        if (m_Materials.find(materialHandle) != m_Materials.end())
            return m_Materials.at(materialHandle);

        auto materialAsset = m_AssetManager->GetAsset<MaterialAsset>(materialHandle);
        Material* material = CreateMaterial(materialAsset);
        m_Materials.insert(std::make_pair(materialHandle, material));

        return material;
    }

    ITexture* ResourceManager::GetTexture(AssetHandle textureHandle) {
        if (m_Materials.find(textureHandle) != m_Materials.end())
            return m_Textures.at(textureHandle);

        auto textureAsset = m_AssetManager->GetAsset<TextureAsset>(textureHandle);
        ITexture* texture = CreateTexture(textureAsset);
        m_Textures.insert(std::make_pair(textureHandle, texture));

        return texture;
    }

    IShader* ResourceManager::GetShader(AssetHandle shaderHandle) {
        if (m_Shaders.find(shaderHandle) != m_Shaders.end())
            return m_Shaders.at(shaderHandle);

        auto shaderAsset = m_AssetManager->GetAsset<ShaderAsset>(shaderHandle);
        IShader* shader = CreateShader(shaderAsset);
        m_Shaders.insert(std::make_pair(shaderHandle, shader));

        return shader;
    }


    bool ResourceManager::IsUserMaterial(uint64_t materialID) {
        Material* mat = m_MaterialRegistry.GetResource(materialID);
        
        return m_UserMaterials.find(mat) != m_UserMaterials.end();
    }

    uint64_t ResourceManager::CreateUserMaterial(uint64_t sourceMaterialID) {
        Material* sourceMat = m_MaterialRegistry.GetResource(sourceMaterialID);
        Material* sourceCopy = sourceMat->Copy();

        m_UserMaterials.insert(sourceCopy);

        return m_MaterialRegistry.AddRecord(sourceCopy);
    }

    void ResourceManager::DestroyUserMaterial(uint64_t materialID) {
        Material* mat = m_MaterialRegistry.GetResource(materialID);

        if (m_UserMaterials.find(mat) == m_UserMaterials.end()) {
            Log("Assert here, trying to delete a user material that does not exist\n");
            return;
        }

        m_UserMaterials.erase(mat);
        m_MaterialRegistry.RemoveRecord(mat);
        delete mat;
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

        m_MaterialRegistry.AddRecord(mat);

        return mat;
    }

    IShader* ResourceManager::CreateShader(const ShaderAsset* shaderAsset) {
        IShader* shader = m_Api->CreateShader(shaderAsset->VertexSource, shaderAsset->FragmentSource, shaderAsset->Semantics);

        return shader;
    }

    ITexture* ResourceManager::CreateTexture(const TextureAsset* textureAsset) {
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
