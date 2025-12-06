#pragma once

#include <cstdint>
#include <vector>

#include "glm/glm.hpp"
#include "Core/Types/Color.hpp"

namespace Phezu {
    
    class Engine;
    class Entity;
    class Vector2;
    class CameraData;
    class TransformData;
  
    class Renderer {
    public:
        Renderer() = delete;
        Renderer(Engine* engine);
        ~Renderer();
    public:
        void ClearFrame(const Color& bg = Color::Black);
        void DrawEntities(const std::vector<Entity*>& renderableEntities, size_t count, const Color& bg = Color::Black);
        void RenderFrame();
        void SetActiveCamera(CameraData* camera);
    private:
        void DrawEntity(Entity* entity);
    private:
        Engine* m_Engine;
        CameraData* m_Camera;
        TransformData* m_CameraTransform;
        float m_ScreenHeight;
        float m_ScreenWidth;
    };
}
