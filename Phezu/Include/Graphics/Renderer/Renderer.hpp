#pragma once

#include <cstdint>
#include <vector>

#include "glm/glm.hpp"
#include "Core/Types/Color.hpp"

namespace Phezu {
    
    class IPlatform;
    class IWindow;
    class Entity;
    class IGraphicsAPI;
    class CameraData;
  
    class Renderer {
    public:
        Renderer();
        ~Renderer();
    public:
        void Init(IPlatform* platform);
        void Destroy();
    public:
        void ClearFrame(const Color& bg = Color::Black);
        void DrawEntities(const std::vector<Entity*>& renderableEntities, size_t count, CameraData* camera);
        void RenderFrame();
    private:
        void DrawEntity(Entity* entity, CameraData* camera);
    private:
        IGraphicsAPI* m_Api;
    private:
        IWindow* m_Window;
        CameraData* m_Camera;
        // MUST HAVE A TARGET FRAME BUFFER
    };
}
