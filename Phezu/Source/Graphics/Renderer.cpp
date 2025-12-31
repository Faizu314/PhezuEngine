#include "Core/Platform.hpp"
#include "Core/Window.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Core/Resources/Shader.hpp"
#include "Graphics/Core/Resources/Texture.hpp"
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
    : m_Ctx(), m_WindowSubId(0) {}
    
    Renderer::~Renderer() {}
    
    void Renderer::Init(RendererContext ctx) {
        m_Ctx = ctx;

        m_WindowSubId = m_Ctx.Window->RegisterWindowResizeCallback(
            [this](int width, int height) { m_Ctx.Api->SetViewport(0, 0, width, height); }
        );
    }

    void Renderer::Destroy() {
        m_Ctx.Window->UnregisterWindowResizeCallback(m_WindowSubId);
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
