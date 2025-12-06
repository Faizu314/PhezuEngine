#pragma once

#include <cstdint>
#include <vector>

#include "glm/glm.hpp"
#include "Core/Types/Color.hpp"

namespace Phezu {
    
    class IPlatform;
    class IWindow;
    class Entity;
    class Vector2;
    class CameraData;
    class TransformData;
    class IGraphicsAPI;
  
    class Renderer {
    public:
        Renderer();
        ~Renderer();
    public:
        void Init(IPlatform* platform);
        void Destroy();
    public:
        void ClearFrame(const Color& bg = Color::Black);
        void DrawEntities(const std::vector<Entity*>& renderableEntities, size_t count, const Color& bg = Color::Black);
        void RenderFrame();
        void SetActiveCamera(CameraData* camera);
    private:
        void DrawEntity(Entity* entity);
    private:
        IGraphicsAPI* m_Api;
    private:
        IWindow* m_Window;
        CameraData* m_Camera;
        TransformData* m_CameraTransform;
    };
}
