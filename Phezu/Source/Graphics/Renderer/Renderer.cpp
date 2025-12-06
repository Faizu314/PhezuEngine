#include "Graphics/Renderer/Renderer.hpp"
#include "Core/Platform.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components/TransformData.hpp"
#include "Scene/Components/ShapeData.hpp"
#include "Scene/Components/RenderData.hpp"
#include "Scene/Components/CameraData.hpp"
#include "Maths/Math.hpp"

#include "glm/glm.hpp"

namespace Phezu {
        
    Renderer::Renderer(Engine* engine)
    : m_Engine(engine), m_Camera(nullptr)
    {
        //m_ScreenWidth = window.GetWidth();
        //m_ScreenHeight = window.GetHeight();
    }
    
    Renderer::~Renderer() {}
    
    void Renderer::SetActiveCamera(CameraData* camera) {
        m_Camera = camera;
        m_CameraTransform = dynamic_cast<TransformData*>(camera->GetEntity()->GetDataComponent(ComponentType::Transform));
    }
   
    
    void Renderer::ClearFrame(const Color& bg) {

    }
    
    void Renderer::DrawEntities(const std::vector<Entity*>& renderableEntities, size_t count, const Color& bg) {
        int index = 0;
        for (auto& entity : renderableEntities) {
            if (index >= count)
                break;
            DrawEntity(entity);
            index++;
        }
    }
    
    void Renderer::DrawEntity(Entity* entity) {
        TransformData* transformData = dynamic_cast<TransformData*>(entity->GetDataComponent(ComponentType::Transform));
        ShapeData* shapeData = dynamic_cast<ShapeData*>(entity->GetDataComponent(ComponentType::Shape));
        RenderData* renderData = dynamic_cast<RenderData*>(entity->GetDataComponent(ComponentType::Render));
        
        if (shapeData == nullptr || renderData == nullptr)
            return;
        
        Vector2 camPosition = m_CameraTransform->GetWorldPosition();
        float aspectRatio = static_cast<float>(m_ScreenWidth) / m_ScreenHeight;
        float vSize = m_Camera->Size * 2.0;
        float hSize = vSize * aspectRatio;
        
        Vector2 upRightLocal = shapeData->GetVertexPosition(ShapeData::VertexType::UpRight);
        Vector2 downLeftLocal = shapeData->GetVertexPosition(ShapeData::VertexType::DownLeft);
        
        Vector2 upRightWorld = transformData->LocalToWorldPoint(upRightLocal);
        Vector2 downLeftWorld = transformData->LocalToWorldPoint(downLeftLocal);
        
        Vector2 upRightView = upRightWorld - camPosition;
        Vector2 downLeftView = downLeftWorld - camPosition;

        Vector2 upRightScreen = Vector2(upRightView.X() * m_ScreenWidth / hSize, upRightView.Y() * m_ScreenHeight / vSize);
        Vector2 downLeftScreen = Vector2(downLeftView.X() * m_ScreenWidth / hSize, downLeftView.Y() * m_ScreenHeight / vSize);
    }
    
    void Renderer::RenderFrame() {
    }
}
