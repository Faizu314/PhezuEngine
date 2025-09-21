#include "Engine.hpp"
#include "scripting/ScriptEngine.hpp"
#include "scripting/ScriptGlue.hpp"
#include "scripting/ScriptInstance.hpp"

#include "scene/Scene.hpp"
#include "scene/components/PhysicsData.hpp"
#include "scene/Entity.hpp"
#include "maths/Math.hpp"

#include "Logger.hpp"

#include "glm/glm.hpp"
#include <mono/jit/jit.h>
#include "mono/metadata/object.h"

namespace Phezu {
	
	struct Data {
		ScriptEngine* ScriptEngine;
		Engine* Engine;
	};

	static Data* s_Data = nullptr;

	Entity* GetEntity(uint64_t entityID) {
		SceneManager sceneManager = s_Data->Engine->GetSceneManager();
		Entity* entity;

		if (auto scene = sceneManager.GetActiveScene()) {
			entity = scene->GetEntity(entityID);
		}
		else if (auto scene = sceneManager.GetMasterScene()) {
			entity = scene->GetEntity(entityID);
		}

		return entity;
	}

	NativeType MonoToNativeType(MonoType* monoType, std::string& outFullname) {
		MonoClass* monoClass = mono_type_get_class(monoType);
		std::string nameSpace = mono_class_get_namespace(monoClass);
		std::string name = mono_class_get_name(monoClass);
		outFullname = nameSpace + "." + name;

		if (outFullname == "PhezuEngine.Transform")
			return NativeType::Transform;
		else if (outFullname == "PhezuEngine.Shape")
			return NativeType::Shape;
		else if (outFullname == "PhezuEngine.Renderer")
			return NativeType::Renderer;
		else if (outFullname == "PhezuEngine.Physics")
			return NativeType::Physics;
		else if (mono_class_is_subclass_of(monoClass, s_Data->ScriptEngine->GetBehaviourComponentClass(), false))
			return NativeType::ScriptComponent;
		
		return NativeType::None;
	}
    
    MonoString* Entity_GetTag(uint64_t entityID) {
        auto entity = GetEntity(entityID);
        const std::string& tag = entity->GetTag();
        
        return mono_string_new(mono_domain_get(), tag.c_str());
    }

	bool Entity_HasComponent(uint64_t entityID, MonoType* monoType) {
		auto entity = GetEntity(entityID);

		std::string classFullname;
		NativeType nativeType = MonoToNativeType(monoType, classFullname);

		switch (nativeType) {
			case NativeType::Transform:
			{
				return true;
			}
			case NativeType::Shape:
			{
				return entity->GetShapeData() != nullptr;
			}
			case NativeType::Renderer:
			{
				return entity->GetRenderData() != nullptr;
			}
			case NativeType::Physics:
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

	uint32_t Entity_GetComponent(uint64_t entityID, MonoType* monoType) {
		auto entity = GetEntity(entityID);

		std::string classFullname;
		NativeType nativeType = MonoToNativeType(monoType, classFullname);
        
		if (nativeType == NativeType::None) {
            Log("Mono type %s not recognized", classFullname.c_str());
			return 0;
		}

        ScriptInstance* scriptInstance = nullptr;
        
        if (nativeType == NativeType::ScriptComponent)
			scriptInstance = s_Data->ScriptEngine->GetBehaviourScriptInstance(entityID, classFullname);
        else
            scriptInstance = s_Data->ScriptEngine->GetNativeComponentInstance(entityID, nativeType);

		if (scriptInstance == nullptr) {
			Log("Unable to find script component: %s on entity: %i\n", classFullname.c_str(), entityID);
			return 0;
		}

		return scriptInstance->GetMonoGcHandle();
	}

	void Transform_GetPosition(uint64_t entityID, glm::vec2* position) {
		Entity* entity = GetEntity(entityID);

		if (entity) {
			Vector2 pos = entity->GetTransformData()->GetWorldPosition();
			position->x = pos.X();
			position->y = pos.Y();
		}
	}

	void Transform_SetPosition(uint64_t entityID, glm::vec2* position) {
		Entity* entity = GetEntity(entityID);

		if (entity) {
			entity->GetTransformData()->SetWorldPosition(Vector2(position->x, position->y));
		}
	}
    
    void Physics_GetVelocity(uint64_t entityID, glm::vec2* velocity) {
        Entity* entity = GetEntity(entityID);

        if (entity) {
            Vector2 vel = entity->GetPhysicsData()->Velocity;
            velocity->x = vel.X();
            velocity->y = vel.Y();
        }
    }

    void Physics_SetVelocity(uint64_t entityID, glm::vec2* velocity) {
        Entity* entity = GetEntity(entityID);

        if (entity) {
            entity->GetPhysicsData()->Velocity = Vector2(velocity->x, velocity->y);
        }
    }

	void ScriptGlue::Init(Engine* engine, ScriptEngine* scriptEngine) {
		s_Data = new Data();
		s_Data->Engine = engine;
		s_Data->ScriptEngine = scriptEngine;
	}

	void ScriptGlue::Bind() {
        mono_add_internal_call("PhezuEngine.InternalCalls::Entity_GetTag", reinterpret_cast<const void*>(&Entity_GetTag));
        mono_add_internal_call("PhezuEngine.InternalCalls::Entity_HasComponent", reinterpret_cast<const void*>(&Entity_HasComponent));
        mono_add_internal_call("PhezuEngine.InternalCalls::Entity_GetComponent", reinterpret_cast<const void*>(&Entity_GetComponent));
        
		mono_add_internal_call("PhezuEngine.InternalCalls::Transform_GetPosition", reinterpret_cast<const void*>(&Transform_GetPosition));
		mono_add_internal_call("PhezuEngine.InternalCalls::Transform_SetPosition", reinterpret_cast<const void*>(&Transform_SetPosition));
        
        mono_add_internal_call("PhezuEngine.InternalCalls::Physics_GetVelocity", reinterpret_cast<const void*>(&Physics_GetVelocity));
        mono_add_internal_call("PhezuEngine.InternalCalls::Physics_SetVelocity", reinterpret_cast<const void*>(&Physics_SetVelocity));
	}

	void ScriptGlue::Destroy() {
		delete s_Data;
		s_Data = nullptr;
	}
}
