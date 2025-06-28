#include "Engine.hpp"
#include "scripting/ScriptEngine.hpp"
#include "scripting/ScriptGlue.hpp"
#include "scripting/ScriptInstance.hpp"

#include "scene/Scene.hpp"
#include "scene/Entity.hpp"
#include "maths/Math.hpp"

#include "Logger.hpp"

#include "glm/glm.hpp"
#include "mono/metadata/object.h"

namespace Phezu {
	
	struct Data {
		ScriptEngine* ScriptEngine;
		Engine* Engine;
	};

	static Data* s_Data = nullptr;

	enum class NativeType {
		None,
		TransformData,
		ShapeData,
		RenderData,
		PhysicsData,
		ScriptComponent
	};

	std::shared_ptr<Entity> GetEntity(uint64_t entityID) {
		SceneManager sceneManager = s_Data->Engine->GetSceneManager();
		std::shared_ptr<Entity> entity;

		if (auto scene = sceneManager.GetActiveScene().lock()) {
			entity = scene->GetEntity(entityID).lock();
		}
		else if (auto scene = sceneManager.GetMasterScene().lock()) {
			entity = scene->GetEntity(entityID).lock();
		}

		return entity;
	}

	NativeType MonoToNativeType(MonoType* monoType, std::string& outFullname) {
		MonoClass* monoClass = mono_type_get_class(monoType);
		std::string nameSpace = mono_class_get_namespace(monoClass);
		std::string name = mono_class_get_name(monoClass);
		outFullname = nameSpace + "." + name;

		if (outFullname == "PhezuEngine.Transform")
			return NativeType::TransformData;
		else if (outFullname == "PhezuEngine.Shape")
			return NativeType::ShapeData;
		else if (outFullname == "PhezuEngine.Renderer")
			return NativeType::RenderData;
		else if (outFullname == "PhezuEngine.RigidBody")
			return NativeType::PhysicsData;
		else if (mono_class_is_subclass_of(monoClass, s_Data->ScriptEngine->GetBehaviourComponentClass(), false))
			return NativeType::ScriptComponent;
		
		return NativeType::None;
	}

	bool Entity_HasComponent(uint64_t entityID, MonoType* monoType) {
		auto entity = GetEntity(entityID);

		std::string classFullname;
		NativeType nativeType = MonoToNativeType(monoType, classFullname);

		switch (nativeType) {
			case NativeType::TransformData:
			{
				return true;
			}
			case NativeType::ShapeData:
			{
				return entity->GetShapeData() != nullptr;
			}
			case NativeType::RenderData:
			{
				return entity->GetRenderData() != nullptr;
			}
			case NativeType::PhysicsData:
			{
				return entity->GetPhysicsData() != nullptr;
			}
			case NativeType::ScriptComponent:
			{
				return entity->HasScriptComponent(classFullname);
			}
			default: {
				Log("MonoType not recognized in Entity_HasComponent");
			}
		}

		return false;
	}

	intptr_t Entity_GetComponent(uint64_t entityID, MonoType* monoType) {
		auto entity = GetEntity(entityID);

		std::string classFullname;
		NativeType nativeType = MonoToNativeType(monoType, classFullname);

		if (nativeType != NativeType::ScriptComponent) {
			Log("Trying to get a native component as a C# class\n");
			return 0;
		}

		ScriptInstance* scriptInstance = 
			s_Data->ScriptEngine->GetScriptInstance(entityID, classFullname);

		if (scriptInstance == nullptr) {
			Log("Unable to find script component: %s on entity: %i\n", classFullname.c_str(), entityID);
			return 0;
		}

		return scriptInstance->GetMonoGcHandle();
	}

	void Transform_GetPosition(uint64_t entityID, glm::vec2* position) {
		std::shared_ptr<Entity> entity = GetEntity(entityID);

		if (entity) {
			Vector2 pos = entity->GetTransformData()->GetWorldPosition();
			position->x = pos.X();
			position->y = pos.Y();
		}
	}

	void Transform_SetPosition(uint64_t entityID, glm::vec2* position) {
		std::shared_ptr<Entity> entity = GetEntity(entityID);

		if (entity) {
			entity->GetTransformData()->SetWorldPosition(Vector2(position->x, position->y));
		}
	}

	void ScriptGlue::Init(Engine* engine, ScriptEngine* scriptEngine) {
		s_Data = new Data();
		s_Data->Engine = engine;
		s_Data->ScriptEngine = scriptEngine;
	}

	void ScriptGlue::Bind() {
		mono_add_internal_call("PhezuEngine.InternalCalls::Transform_GetPosition", reinterpret_cast<const void*>(&Transform_GetPosition));
		mono_add_internal_call("PhezuEngine.InternalCalls::Transform_SetPosition", reinterpret_cast<const void*>(&Transform_SetPosition));
		mono_add_internal_call("PhezuEngine.InternalCalls::Entity_HasComponent", reinterpret_cast<const void*>(&Entity_HasComponent));
		mono_add_internal_call("PhezuEngine.InternalCalls::Entity_GetComponent", reinterpret_cast<const void*>(&Entity_GetComponent));
	}

	void ScriptGlue::Destroy() {
		delete s_Data;
		s_Data = nullptr;
	}
}
