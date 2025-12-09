#include "Graphics/Renderer/Renderer.hpp"
#include "Graphics/GraphicsAPI.hpp"
#include "Core/Platform.hpp"
#include "Core/Window.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components/TransformData.hpp"
#include "Scene/Components/ShapeData.hpp"
#include "Scene/Components/RenderData.hpp"
#include "Scene/Components/CameraData.hpp"
#include "Maths/Math.hpp"

#include "glm/glm.hpp"

namespace Phezu {
        
    Renderer::Renderer()
    : m_Api(nullptr), m_Window(nullptr), m_WindowSubId(0) {}
    
    Renderer::~Renderer() {}
    
    void Renderer::Init(IWindow* window, IGraphicsAPI* graphicsApi) {
        m_Api = graphicsApi;
        m_Window = window;

        m_WindowSubId = m_Window->RegisterWindowResizeCallback(
            [this](int width, int height) { m_Api->SetViewport(0, 0, width, height); }
        );
    }

    void Renderer::Destroy() {
        m_Window->UnregisterWindowResizeCallback(m_WindowSubId);
    }
   
    
    void Renderer::ClearFrame(const Color& bg) {
        m_Api->ClearFrame(bg);
    }
    
    void Renderer::DrawEntities(const std::vector<Entity*>& renderableEntities, size_t count, CameraData* camera) {
        int index = 0;
        for (auto& entity : renderableEntities) {
            if (index >= count)
                break;
            DrawEntity(entity, camera);
            index++;
        }
    }
    
    void Renderer::DrawEntity(Entity* entity, CameraData* camera) {
        TransformData* cameraTransform = dynamic_cast<TransformData*>(camera->GetEntity()->GetDataComponent(ComponentType::Transform));
        TransformData* transformData = dynamic_cast<TransformData*>(entity->GetDataComponent(ComponentType::Transform));
        ShapeData* shapeData = dynamic_cast<ShapeData*>(entity->GetDataComponent(ComponentType::Shape));
        RenderData* renderData = dynamic_cast<RenderData*>(entity->GetDataComponent(ComponentType::Render));
        
        if (shapeData == nullptr || renderData == nullptr)
            return;

        int screenWidth = m_Window->GetWidth();
        int screenHeight = m_Window->GetHeight();
        
        Vector2 camPosition = cameraTransform->GetWorldPosition();
        float aspectRatio = static_cast<float>(screenWidth) / screenHeight;
        float vSize = camera->Size * 2.0f;
        float hSize = vSize * aspectRatio;
        
        Vector2 upRightLocal = shapeData->GetVertexPosition(ShapeData::VertexType::UpRight);
        Vector2 downLeftLocal = shapeData->GetVertexPosition(ShapeData::VertexType::DownLeft);
        
        Vector2 upRightWorld = transformData->LocalToWorldPoint(upRightLocal);
        Vector2 downLeftWorld = transformData->LocalToWorldPoint(downLeftLocal);
        
        Vector2 upRightView = upRightWorld - camPosition;
        Vector2 downLeftView = downLeftWorld - camPosition;

        Vector2 upRightScreen = Vector2(upRightView.X() * screenWidth / hSize, upRightView.Y() * screenHeight / vSize);
        Vector2 downLeftScreen = Vector2(downLeftView.X() * screenWidth / hSize, downLeftView.Y() * screenHeight / vSize);

        m_Api->RenderBox(downLeftScreen, upRightScreen, renderData->Tint);
    }
    
    void Renderer::RenderFrame() {
    }
}
