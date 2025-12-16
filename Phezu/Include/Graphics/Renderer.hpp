#pragma once

#include <cstdint>
#include <vector>

#include "glm/glm.hpp"

#include "Core/Types/Color.hpp"
#include "Graphics/Core/Descriptors/VertexLayout.hpp"

namespace Phezu {
    
    class IWindow;
    class Entity;
    class IGraphicsAPI;
    class CameraData;
    class IShader;
    class IIndexBuffer;
  
    class Renderer {
    public:
        Renderer();
        ~Renderer();
    public:
        void Init(IWindow* window, IGraphicsAPI* graphicsApi);
        void Destroy();
    public:
        void ClearFrame();
        void DrawEntities(const std::vector<Entity*>& renderableEntities, size_t count, CameraData* camera);
    private:
        void DrawEntity(Entity* entity, CameraData* camera);
    private:
        IGraphicsAPI* m_Api;
    private:
        int m_WindowSubId;
        IWindow* m_Window;
        // MUST HAVE A TARGET FRAME BUFFER
    private:
        IShader* m_DefaultShader;
        IIndexBuffer* m_QuadIndices;
        VertexLayout m_QuadLayout;
    };
}
