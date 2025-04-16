#pragma once

#include "Engine.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include "Physics.hpp"
#include "maths/Random.hpp"
#include "maths/Math.hpp"
#include "scene/Prefab.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneManager.hpp"
#include "scene/Blueprint.hpp"
#include "scene/BlueprintEntry.hpp"
#include "scene/Entity.hpp"
#include "scene/components/DataComponent.hpp"
#include "scene/components/TransformData.hpp"
#include "scene/components/ShapeData.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/PhysicsData.hpp"
#include "scene/components/ScriptComponent.hpp"

namespace Phezu {
    
    Engine& CreateEngine();
}
