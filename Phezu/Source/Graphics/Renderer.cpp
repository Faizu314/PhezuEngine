#include "Graphics/Renderer.hpp"
#include "Graphics/Core/Resources/Shader.hpp" 

#include "Graphics/Core/GraphicsAPI.hpp"

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
    : m_Api(nullptr), m_Window(nullptr), m_DefaultShader(nullptr), m_WindowSubId(0) {}
    
    Renderer::~Renderer() {}
    
    void Renderer::Init(IWindow* window, IGraphicsAPI* graphicsApi) {
        m_Api = graphicsApi;
        m_Window = window;

        m_WindowSubId = m_Window->RegisterWindowResizeCallback(
            [this](int width, int height) { m_Api->SetViewport(0, 0, width, height); }
        );

        std::string vert = 
            "#version 460 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";

        std::string frag =
            "#version 460 core\n"
            "out vec4 FragColor;\n"
            "uniform vec4 tint;"
            "void main()\n"
            "{\n"
            "   FragColor = tint;\n"
            "}\0";

        m_DefaultShader = m_Api->CreateShader(vert, frag);
        m_DefaultShader->Bind();
    }

    void Renderer::Destroy() {
        m_Window->UnregisterWindowResizeCallback(m_WindowSubId);
    }
   
    
    void Renderer::ClearFrame() {
        m_Api->ClearFrame(Color(0, 0, 0, 0));
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

        Vector2 upRightNormalized = Vector2(upRightScreen.X() / (screenWidth / 2.0f), upRightScreen.Y() / (screenWidth / 2.0f));
        Vector2 downLeftNormalized = Vector2(downLeftScreen.X() / (screenWidth / 2.0f), downLeftScreen.Y() / (screenWidth / 2.0f));

        m_DefaultShader->SetVec4("tint", renderData->Tint);

        m_Api->RenderBox(downLeftNormalized, upRightNormalized, renderData->Tint);
    }
    
    void Renderer::RenderFrame() {
    }
}
