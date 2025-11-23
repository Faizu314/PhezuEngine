#pragma once

#include <cstdint>
#include <vector>

#include "glm/glm.hpp"

namespace Phezu {
    
    class Engine;
    class IWindow;
    class Entity;
    class Vector2;
    class CameraData;
    class TransformData;
    
    class Texture {
    private:
        Texture() = delete;
        
        friend class Renderer;
    };
    
    struct Rect {
        Rect(int x = 0, int y = 0, int w = 1, int h = 1);
        
        int x, y, w, h;
    };
    
    class Color {
    public:
        Color(uint8_t red = 255, uint8_t green = 255, uint8_t blue = 255, uint8_t alpha = 255);
        uint8_t r, g, b, a;
    public:
        static const Color White;
        static const Color Green;
        static const Color Red;
        static const Color Black;
        
        friend class Renderer;
    };
    
    class Renderer {
    public:
        Renderer() = delete;
        Renderer(Engine* engine, const IWindow& window);
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
