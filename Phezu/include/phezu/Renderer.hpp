#pragma once

#include <cstdint>
#include <vector>

#include "glm/glm.hpp"

struct SDL_Color;
struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;

namespace Phezu {
    
    class Engine;
    class Window;
    class Entity;
    class Vector2;
    
    class Texture {
    private:
        Texture() = delete;
        Texture(SDL_Texture* texture);
    private:
        SDL_Texture* m_Texture;
    public:
        operator SDL_Texture*() { return m_Texture; }
        
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
    private:
        void ConvertToSDLColor(SDL_Color& sdlColor) const;
        static Color FromSDLColor(const SDL_Color& sdlColor);
        
        friend class Renderer;
    };
    
    class Renderer {
    public:
        Renderer() = delete;
        Renderer(Engine* engine, const Window& window);
        ~Renderer();
    public:
        void ClearFrame(const Color& bg = Color::Black);
        void DrawEntities(const std::vector<Entity*>& renderableEntities, size_t count, const Color& bg = Color::Black);
        void RenderFrame();
    private:
        void DrawEntity(Entity* entity);
        Vector2 WorldToSdlPosition(const Vector2& worldPos) const;
    private:
        Engine* m_Engine;
        SDL_Renderer* m_RendererPtr;
        glm::mat3 m_WorldToSdl;
        SDL_Texture* m_DefaultTex;
        SDL_Texture* m_IntermediateTex;
    };
    
    Texture* LoadTexture(const Renderer& renderer);
}
