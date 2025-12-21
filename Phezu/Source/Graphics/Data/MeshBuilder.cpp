#include "Core/Types/Buffer.hpp"
#include "Graphics/Core/GraphicsAPI.hpp"
#include "Graphics/Core/Resources/Shader.hpp"
#include "Graphics/Data/MeshBuilder.hpp"

namespace Phezu {

	Mesh MeshBuilder::CreateMesh(const MeshAsset* meshAsset, IGraphicsAPI* api)
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

        Mesh mesh;

        mesh.Init(api);

        mesh.AttachVertexBuffer(vertexBuffer.GetData(), vertexBuffer.GetSize(), BufferType::Static, layout);
        mesh.AttachIndexBuffer(meshAsset->IndexBuffer.data(), indexBufferSize, BufferType::Static);

        mesh.UploadMeshData();

        vertexBuffer.Destroy();

		return mesh;
	}

    IShader* MeshBuilder::CreateShader(const ShaderAsset* shaderAsset, IGraphicsAPI* api) {
        IShader* shader = api->CreateShader(shaderAsset->VertexSource, shaderAsset->FragmentSource, shaderAsset->Semantics);

        return shader;
    }
}
