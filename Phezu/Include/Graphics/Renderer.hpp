#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>

#include "Core/Types/Color.hpp"
#include "Core/Types/GUID.hpp"
#include "Graphics/Core/Descriptors/VertexLayout.hpp"

namespace Phezu {
    
    class IWindow;
    class Entity;
    class IGraphicsAPI;
    class ITexture;
    class IShader;
    class CameraData;
    class Mesh;
    class ResourceManager;
    class IFrameBuffer;

    struct RendererContext {
        RendererContext() = default;

        IWindow* Window = nullptr;
        IGraphicsAPI* Api = nullptr;
        ResourceManager* Asset = nullptr;
    };
  
    class Renderer {
    public:
        Renderer();
        ~Renderer();
    public:
        void Init(RendererContext ctx, RenderTarget renderTarget);
        void Destroy();
    public:
        void DrawScene(const std::vector<Entity*>& renderableEntities, size_t count, CameraData* camera);
    private:
        void OnWindowResized(int width, int height);
        void DrawEntity(Entity* entity, CameraData* camera);
    private:
        RendererContext m_Ctx;
    private:
        int m_WindowSubId;
        ITexture* m_IntermediateTex;
        IFrameBuffer* m_IntermediateTarget;
        IFrameBuffer* m_RenderTarget;
    private:
        const Mesh* m_QuadMesh;
        IShader* m_BlitShader;
    private:
        Mat3x3 m_ViewTransform;
        Mat3x3 m_ScreenTransform;
    };
}
