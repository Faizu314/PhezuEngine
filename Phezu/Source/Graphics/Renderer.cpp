#include "Core/Platform.hpp"
#include "Core/Window.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Core/Resources/Texture.hpp"
#include "Graphics/Core/Resources/Shader.hpp"
#include "Graphics/Core/Resources/FrameBuffer.hpp"
#include "Graphics/Core/GraphicsAPI.hpp"
#include "Graphics/Data/Mesh.hpp"
#include "Graphics/Data/Material.hpp"
#include "Graphics/Data/ResourceManager.hpp"
#include "Asset/Core/AssetManager.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components/TransformData.hpp"
#include "Scene/Components/ShapeData.hpp"
#include "Scene/Components/RenderData.hpp"
#include "Scene/Components/CameraData.hpp"
#include "Maths/Math.hpp"

namespace Phezu {
    
    Renderer::Renderer()
    : m_Ctx(), m_IntermediateTex(nullptr), m_IntermediateTarget(nullptr), m_RenderTarget(nullptr), m_WindowSubId(0), m_QuadMesh(nullptr), m_BlitShader(nullptr) { }
    
    Renderer::~Renderer() {}
    
    void Renderer::Init(RendererContext ctx, RenderTarget renderTarget) {
        m_Ctx = ctx;
        m_RenderTarget = renderTarget.Target;

        m_WindowSubId = m_Ctx.Window->RegisterWindowResizeCallback(
            [this](int width, int height) { OnWindowResized(width, height); }
        );

        m_QuadMesh = m_Ctx.Asset->GetMesh({ static_cast<int>(BuiltInAssetType::QuadMesh), AssetSource::Engine });
        m_BlitShader = m_Ctx.Asset->GetShader({ static_cast<int>(BuiltInAssetType::BlitShader), AssetSource::Engine});
        m_BlitShader->SetInt("mainTex", 0);

        SamplerDesc samplerDesc = { TextureWrapMode::ClampToEdge, TextureFilteringMode::Point };

        float renderScale = static_cast<float>(m_Ctx.Window->GetRenderScale());
        int texWidth = static_cast<int>(m_Ctx.Window->GetWidth() / renderScale);
        int texHeight = static_cast<int>(m_Ctx.Window->GetHeight() / renderScale);
        m_IntermediateTex = m_Ctx.Api->CreateTexture(nullptr, texWidth, texHeight, samplerDesc);

        m_IntermediateTarget = m_Ctx.Api->CreateFrameBuffer();
        m_IntermediateTarget->Bind();
        m_IntermediateTarget->AttachTexture(m_IntermediateTex);
    }

    void Renderer::OnWindowResized(int width, int height) {
        float renderScale = static_cast<float>(m_Ctx.Window->GetRenderScale());
        int texWidth = static_cast<int>(m_Ctx.Window->GetWidth() / renderScale);
        int texHeight = static_cast<int>(m_Ctx.Window->GetHeight() / renderScale);

        m_IntermediateTex->Resize(nullptr, texWidth, texHeight);
    }

    void Renderer::Destroy() {
        m_Ctx.Window->UnregisterWindowResizeCallback(m_WindowSubId);

        m_IntermediateTex->Destroy();
        delete m_IntermediateTex;
        
        m_IntermediateTarget->Destroy();
        delete m_IntermediateTarget;
    }
    
    void Renderer::DrawScene(const std::vector<Entity*>& renderableEntities, size_t count, CameraData* camera) {
        TransformData* cameraTransform = dynamic_cast<TransformData*>(camera->GetEntity()->GetDataComponent(ComponentType::Transform));

        m_ViewTransform.SetTranslation(cameraTransform->GetWorldPosition());

        int screenWidth = m_Ctx.Window->GetWidth();
        int screenHeight = m_Ctx.Window->GetHeight();
        float renderScale = static_cast<float>(m_Ctx.Window->GetRenderScale());
        float aspectRatio = static_cast<float>(screenWidth) / screenHeight;

        Vector2 scaling(1.0f / (aspectRatio * camera->Size), 1.0f / camera->Size);
        m_ScreenTransform.SetScaling(scaling);

        m_IntermediateTarget->Bind();
        m_Ctx.Api->SetViewport(0, 0, static_cast<int>(screenWidth / renderScale), static_cast<int>(screenHeight / renderScale));
        m_Ctx.Api->ClearFrame(Color::Clear);

        int index = 0;
        for (auto& entity : renderableEntities) {
            if (index >= count)
                break;
            DrawEntity(entity, camera);
            index++;
        }

        m_Ctx.Api->SetRenderTarget(m_RenderTarget);
        m_Ctx.Api->SetViewport(0, 0, screenWidth, screenHeight);
        m_Ctx.Api->ClearFrame(Color::Clear);

        m_QuadMesh->Bind(m_BlitShader);
        m_BlitShader->Bind();
        m_IntermediateTex->Bind(0);

        m_Ctx.Api->RenderTriangles(6);
    }
    
    void Renderer::DrawEntity(Entity* entity, CameraData* camera) {
        TransformData* transformData = dynamic_cast<TransformData*>(entity->GetDataComponent(ComponentType::Transform));
        ShapeData* shapeData = dynamic_cast<ShapeData*>(entity->GetDataComponent(ComponentType::Shape));
        RenderData* renderData = dynamic_cast<RenderData*>(entity->GetDataComponent(ComponentType::Render));
        
        if (shapeData == nullptr || renderData == nullptr)
            return;

        auto materialHandle = renderData->GetMaterialHandle();
        Material* material = m_Ctx.Asset->GetMaterial(materialHandle);

        auto meshHandle = shapeData->GetMeshHandle();
        const Mesh* mesh = m_Ctx.Asset->GetMesh(meshHandle);

        mesh->Bind(material->GetShader());
        material->Bind();

        material->SetMat3("objectToWorld", transformData->GetLocalToWorld());
        material->SetMat3("worldToView", m_ViewTransform);
        material->SetMat3("viewToScreen", m_ScreenTransform);

        m_Ctx.Api->RenderTriangles(mesh->GetIndicesCount());
    }
}
