#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>

#include "Core/Types/Color.hpp"
#include "Core/Types/GUID.hpp"
#include "Graphics/Core/Descriptors/VertexLayout.hpp"
#include "Graphics/Data/Mesh.hpp"

namespace Phezu {
    
    class IWindow;
    class Entity;
    class IGraphicsAPI;
    class CameraData;
    class IShader;
    class AssetManager;

    struct RendererContext {
        RendererContext() = default;

        IWindow* Window = nullptr;
        IGraphicsAPI* Api = nullptr;
        AssetManager* Asset = nullptr;
    };
  
    class Renderer {
    public:
        Renderer();
        ~Renderer();
    public:
        void Init(RendererContext ctx);
        void Destroy();
    public:
        void ClearFrame();
        void DrawEntities(const std::vector<Entity*>& renderableEntities, size_t count, CameraData* camera);
    private:
        void DrawEntity(Entity* entity, CameraData* camera);
    private:
        RendererContext m_Ctx;
        int m_WindowSubId;
        // MUST HAVE A TARGET FRAME BUFFER
    private:
        std::unordered_map<GUID, IShader*> m_Shaders;
        std::unordered_map<GUID, Mesh> m_Meshes;
    private:
        Mat3x3 m_ViewTransform;
        Mat3x3 m_ScreenTransform;
    };
}
