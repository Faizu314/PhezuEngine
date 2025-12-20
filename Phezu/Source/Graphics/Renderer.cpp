#include "Core/Platform.hpp"
#include "Core/Window.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Core/Resources/VertexBuffer.hpp" 
#include "Graphics/Core/Resources/IndexBuffer.hpp" 
#include "Graphics/Core/Resources/VertexArray.hpp" 
#include "Graphics/Core/Resources/Shader.hpp"
#include "Graphics/Core/GraphicsAPI.hpp"
#include "Graphics/Data/Mesh.hpp"
#include "Graphics/Data/MeshBuilder.hpp"
#include "Asset/Core/AssetManager.hpp"
#include "Asset/Types/MeshAsset.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components/TransformData.hpp"
#include "Scene/Components/ShapeData.hpp"
#include "Scene/Components/RenderData.hpp"
#include "Scene/Components/CameraData.hpp"
#include "Maths/Math.hpp"


#include "Graphics/OpenGL/Resources/GLShader.hpp"

namespace Phezu {
    
    Renderer::Renderer()
    : m_Ctx(), m_DefaultShader(nullptr), m_WindowSubId(0) {}
    
    Renderer::~Renderer() {}
    
    void Renderer::Init(RendererContext ctx) {
        m_Ctx = ctx;

        m_WindowSubId = m_Ctx.Window->RegisterWindowResizeCallback(
            [this](int width, int height) { m_Ctx.Api->SetViewport(0, 0, width, height); }
        );

        std::string vert = R"(
            #version 460 core
            layout (location = 3) in vec2 aPos;
            layout (location = 2) in vec4 color;
            uniform mat3 objectToWorld;
            uniform mat3 worldToView;
            uniform mat3 viewToScreen;
            out vec4 vertexColor;
            void main()
            {
                vec3 worldPos = objectToWorld * vec3(aPos.xy, 1.0);
                vec3 viewPos = worldToView * vec3(worldPos.xy, 1.0);
                vec3 screenPos = viewToScreen * vec3(viewPos.xy, 1.0);

                gl_Position = vec4(screenPos.xy, 0.0, 1.0);
                vertexColor = color;
            })";

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

        AssetHandle<MeshAsset> handle = { 99 };
        const MeshAsset* meshAsset = m_Ctx.Asset->GetAsset(handle);

        m_Meshes.insert(std::make_pair(handle.GetGuid(), MeshBuilder::CreateMesh(meshAsset, m_Ctx.Api)));
    }

    void Renderer::Destroy() {
        m_Ctx.Window->UnregisterWindowResizeCallback(m_WindowSubId);

        m_DefaultShader->Destroy();
    }
    
    void Renderer::ClearFrame() {
        m_Ctx.Api->ClearFrame(Color(0, 0, 0, 0));
    }
    
    void Renderer::DrawEntities(const std::vector<Entity*>& renderableEntities, size_t count, CameraData* camera) {
        TransformData* cameraTransform = dynamic_cast<TransformData*>(camera->GetEntity()->GetDataComponent(ComponentType::Transform));

        m_ViewTransform.SetTranslation(cameraTransform->GetWorldPosition());

        int screenWidth = m_Ctx.Window->GetWidth();
        int screenHeight = m_Ctx.Window->GetHeight();

        float aspectRatio = static_cast<float>(screenWidth) / screenHeight;
        float vSize = camera->Size * 2.0f;
        float hSize = vSize * aspectRatio;

        m_ScreenTransform.Set(0, 0, 1.0f / (aspectRatio * camera->Size));
        m_ScreenTransform.Set(1, 1, 1.0f / camera->Size);

        m_DefaultShader->SetMat3("worldToView", m_ViewTransform);
        m_DefaultShader->SetMat3("viewToScreen", m_ScreenTransform);

        int index = 0;
        for (auto& entity : renderableEntities) {
            if (index >= count)
                break;
            DrawEntity(entity, camera);
            index++;
        }
    }
    
    void Renderer::DrawEntity(Entity* entity, CameraData* camera) {
        TransformData* transformData = dynamic_cast<TransformData*>(entity->GetDataComponent(ComponentType::Transform));
        ShapeData* shapeData = dynamic_cast<ShapeData*>(entity->GetDataComponent(ComponentType::Shape));
        RenderData* renderData = dynamic_cast<RenderData*>(entity->GetDataComponent(ComponentType::Render));
        
        if (shapeData == nullptr || renderData == nullptr)
            return;

        m_DefaultShader->SetMat3("objectToWorld", transformData->GetLocalToWorld());
        m_DefaultShader->SetColor("tint", renderData->Tint);

        auto meshHandle = shapeData->GetMeshHandle();
        Mesh& mesh = m_Meshes.at(meshHandle.GetGuid());

        mesh.Bind(m_DefaultShader);

        m_Ctx.Api->RenderTriangles(mesh.GetIndicesCount());
    }
}
