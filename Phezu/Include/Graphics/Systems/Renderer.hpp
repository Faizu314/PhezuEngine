// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <vector>

#include "Graphics/Core/Descriptors/VertexLayout.hpp"
#include "Maths/Objects/Mat3x3.hpp"

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
        IWindow* Window = nullptr;
        IGraphicsAPI* Api = nullptr;
        ResourceManager* Asset = nullptr;
    };
  
    class Renderer {
    public:
        Renderer();
    public:
        Renderer(const Renderer&) = delete;
        Renderer(const Renderer&&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(const Renderer&&) = delete;
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
        int m_WindowSubId = -1;
        int m_ScreenWidth = 0;
        int m_ScreenHeight = 0;
    private:
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
