#include "Graphics/Renderer.hpp"
#include "Graphics/Core/Resources/VertexBuffer.hpp" 
#include "Graphics/Core/Resources/IndexBuffer.hpp" 
#include "Graphics/Core/Resources/VertexArray.hpp" 
#include "Graphics/Core/Resources/Shader.hpp"

#include "Graphics/OpenGL/Resources/GLShader.hpp"

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
    : m_Ctx(), m_DefaultShader(nullptr), m_QuadIndices(nullptr), m_QuadLayout(nullptr), m_WindowSubId(0) {}
    
    Renderer::~Renderer() {}
    
    void Renderer::Init(RendererContext ctx) {
        m_Ctx = ctx;

        m_WindowSubId = m_Ctx.Window->RegisterWindowResizeCallback(
            [this](int width, int height) { m_Ctx.Api->SetViewport(0, 0, width, height); }
        );

        std::string vert = 
            "#version 460 core\n"
            "layout (location = 3) in vec2 aPos;\n"
            "layout (location = 2) in vec4 color;\n"
            "out vec4 vertexColor;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
            "   vertexColor = color;\n"
            "}\0";

        std::string frag =
            "#version 460 core\n"
            "out vec4 FragColor;\n"
            "in vec4 vertexColor;\n"
            "uniform vec4 tint;"
            "void main()\n"
            "{\n"
            "   FragColor = tint * vertexColor;\n"
            "}\0";

        m_DefaultShader = m_Ctx.Api->CreateShader(vert, frag);
        ((GLShader*)m_DefaultShader)->SetSemantics({ std::make_pair(VertexSemantic::Position, 3), 
                                                     std::make_pair(VertexSemantic::Color, 2) });
        m_DefaultShader->Bind();

        unsigned int indices[] = {
            0, 2, 1,
            0, 3, 2
        };

        m_QuadIndices = m_Ctx.Api->CreateIndexBuffer(indices, sizeof(indices), BufferType::Static);
        m_QuadLayout = new VertexLayout(
        {
            { VertexSemantic::Position, VertexAttributeType::Float, VertexAttributeCount::Two },
            { VertexSemantic::Color, VertexAttributeType::UByte, VertexAttributeCount::Four, true }
        });
    }

    void Renderer::Destroy() {
        m_Ctx.Window->UnregisterWindowResizeCallback(m_WindowSubId);

        m_QuadIndices->Destroy();
        m_DefaultShader->Destroy();
        delete m_QuadIndices;
        delete m_DefaultShader;
    }
    
    void Renderer::ClearFrame() {
        m_Ctx.Api->ClearFrame(Color(0, 0, 0, 0));
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

        int screenWidth = m_Ctx.Window->GetWidth();
        int screenHeight = m_Ctx.Window->GetHeight();
        
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

        Vector2 ur = Vector2(upRightScreen.X() / (screenWidth / 2.0f), upRightScreen.Y() / (screenWidth / 2.0f));
        Vector2 dl = Vector2(downLeftScreen.X() / (screenWidth / 2.0f), downLeftScreen.Y() / (screenWidth / 2.0f));


        m_DefaultShader->SetVec4("tint", renderData->Tint);

        struct Vertex {
            float x;
            float y;
            std::uint8_t r;
            std::uint8_t g;
            std::uint8_t b;
            std::uint8_t a;
        };

        Vertex vertices[] = {
            { dl.X(), dl.Y(), 255, 255, 255, 255 },
            { dl.X(), ur.Y(), 100, 100, 100, 100 },
            { ur.X(), ur.Y(), 150, 150, 150, 150 },
            { ur.X(), dl.Y(), 050, 050, 050, 050 }
        };

        IVertexBuffer* vertexBuffer = m_Ctx.Api->CreateVertexBuffer(vertices, sizeof(vertices), BufferType::Static);
        IVertexArray* vertexArray = m_Ctx.Api->CreateVertexArray();

        vertexArray->LinkVertexBuffer(vertexBuffer);
        vertexArray->LinkIndexBuffer(m_QuadIndices);
        vertexArray->ApplyLayout(m_QuadLayout, m_DefaultShader);
        vertexArray->Bind();

        m_Ctx.Api->RenderQuad(6);

        vertexBuffer->Destroy();
        vertexArray->Destroy();
        delete vertexBuffer;
        delete vertexArray;
    }
}
