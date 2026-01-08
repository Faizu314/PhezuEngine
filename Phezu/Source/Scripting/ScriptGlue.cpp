#include "glm/glm.hpp"
#include <mono/jit/jit.h>

#include "Core/Platform.hpp"
#include "Core/Engine.hpp"
#include "Scripting/ScriptEngine.hpp"
#include "Scripting/ScriptGlue.hpp"
#include "Graphics/Data/ResourceManager.hpp"
#include "Graphics/Data/Material.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components/PhysicsData.hpp"
#include "Scene/Components/RenderData.hpp"
#include "Maths/Math.hpp"

namespace Phezu {
	
	struct Data {
		SceneManager* SceneManager;
		ScriptEngine* ScriptEngine;
		ResourceManager* ResourceManager;
	};

	static Data* s_Data = nullptr;

	Entity* GetEntity(uint64_t entityID) {
		SceneManager* sceneManager = s_Data->SceneManager;
		Entity* entity;

		if (auto scene = sceneManager->GetActiveScene()) {
			entity = scene->GetEntity(entityID);
		}
		else if (auto scene = sceneManager->GetMasterScene()) {
			entity = scene->GetEntity(entityID);
		}

		return entity;
	}

	ManagedType MonoToManagedType(MonoType* monoType, std::string& outFullname) {
		MonoClass* monoClass = mono_type_get_class(monoType);
		std::string nameSpace = mono_class_get_namespace(monoClass);
		std::string name = mono_class_get_name(monoClass);
		outFullname = nameSpace + "." + name;

		if (outFullname == "PhezuEngine.Transform")
			return ManagedType::Transform;
		else if (outFullname == "PhezuEngine.Shape")
			return ManagedType::Shape;
		else if (outFullname == "PhezuEngine.Renderer")
			return ManagedType::Renderer;
		else if (outFullname == "PhezuEngine.Physics")
			return ManagedType::Physics;
		else if (mono_class_is_subclass_of(monoClass, s_Data->ScriptEngine->GetBehaviourComponentClass(), false))
			return ManagedType::ScriptComponent;
		
		return ManagedType::None;
	}
    
	/*----Entity-Internal-Calls----*/

    MonoString* Entity_GetTag(uint64_t entityID) {
        auto entity = GetEntity(entityID);
        const std::string& tag = entity->Tag;
        
        return mono_string_new(mono_domain_get(), tag.c_str());
    }
    
	bool Entity_HasComponent(uint64_t entityID, MonoType* monoType) {
		auto entity = GetEntity(entityID);

		std::string classFullname;
		ManagedType managedType = MonoToManagedType(monoType, classFullname);

		switch (managedType) {
			case ManagedType::Transform:
			{
				return true;
			}
			case ManagedType::Shape:
			{
                return entity->HasDataComponent(ComponentType::Shape);
			}
			case ManagedType::Renderer:
			{
                return entity->HasDataComponent(ComponentType::Render);
			}
			case ManagedType::Physics:
			{
                return entity->HasDataComponent(ComponentType::Physics);
			}
			case ManagedType::ScriptComponent:
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
		ManagedType managedType = MonoToManagedType(monoType, classFullname);
        
		if (managedType == ManagedType::None) {
            Log("Mono type %s not recognized", classFullname.c_str());
			return 0;
		}

        ScriptInstance* scriptInstance = nullptr;
        
        if (managedType == ManagedType::ScriptComponent)
			scriptInstance = s_Data->ScriptEngine->GetBehaviourScriptInstance(entityID, classFullname);
        else
            scriptInstance = s_Data->ScriptEngine->GetEngineComponentInstance(entityID, managedType);

		if (scriptInstance == nullptr) {
			Log("Unable to find script component: %s on entity: %i\n", classFullname.c_str(), entityID);
			return 0;
		}

		return scriptInstance->GetMonoGcHandle();
	}
    
    void Entity_RemoveComponent(uint64_t entityID, MonoType* monoType) {
        auto entity = GetEntity(entityID);

        std::string classFullname;
        ManagedType managedType = MonoToManagedType(monoType, classFullname);
        
        if (managedType == ManagedType::None) {
            Log("Mono type %s not recognized", classFullname.c_str());
            return;
        }
        
        s_Data->ScriptEngine->RemoveBehaviourScriptInstance(entityID, classFullname);
    }
    
    uint32_t Entity_Instantiate(GUID prefabGuid) {
		Entity* entity = s_Data->SceneManager->GetMasterScene()->CreateEntity({ prefabGuid, AssetSource::Project });
        return s_Data->ScriptEngine->GetEntityScriptInstanceGcHandle(entity->GetEntityID());
    }
    
    void Entity_Destroy(uint64_t entityID) {
        auto entity = GetEntity(entityID);

        entity->Destroy();
    }

    /*----Transform-Internal-Calls----*/
    
	void Transform_GetPosition(uint64_t entityID, glm::vec2* position) {
		Entity* entity = GetEntity(entityID);

		if (entity) {
            TransformData* transform = dynamic_cast<TransformData*>(entity->GetDataComponent(ComponentType::Transform));
            
			Vector2 pos = transform->GetWorldPosition();
			position->x = pos.X();
			position->y = pos.Y();
		}
	}

	void Transform_SetPosition(uint64_t entityID, glm::vec2* position) {
		Entity* entity = GetEntity(entityID);

		if (entity) {
            TransformData* transform = dynamic_cast<TransformData*>(entity->GetDataComponent(ComponentType::Transform));
            
            transform->SetWorldPosition(Vector2(position->x, position->y));
		}
	}
    
    /*----Physics-Internal-Calls----*/
    
    void Physics_GetVelocity(uint64_t entityID, glm::vec2* velocity) {
        Entity* entity = GetEntity(entityID);

        if (entity) {
            PhysicsData* physics = dynamic_cast<PhysicsData*>(entity->GetDataComponent(ComponentType::Physics));
            
            Vector2 vel = physics->Velocity;
            velocity->x = vel.X();
            velocity->y = vel.Y();
        }
    }

    void Physics_SetVelocity(uint64_t entityID, glm::vec2* velocity) {
        Entity* entity = GetEntity(entityID);

        if (entity) {
            PhysicsData* physics = dynamic_cast<PhysicsData*>(entity->GetDataComponent(ComponentType::Physics));
            
            physics->Velocity = Vector2(velocity->x, velocity->y);
        }
    }

    void Renderer_GetColor(uint64_t entityID, MonoString* propertyName, Color* tint) {
        Entity* entity = GetEntity(entityID);
        
		char* propertyCStr = mono_string_to_utf8(propertyName);

        if (entity) {
            RenderData* render = dynamic_cast<RenderData*>(entity->GetDataComponent(ComponentType::Render));
            
			auto matHandle = render->GetMaterialHandle();
			Material* mat = s_Data->ResourceManager->GetMaterial(matHandle);
			MaterialProperty property = mat->GetProperty(propertyCStr);
			
			if (property.Type != MaterialPropertyType::Color) {
				*tint = Color::Clear;
				return;
			}

			*tint = std::get<Color>(property.Value);
        }
    }
    
    void Renderer_SetColor(uint64_t entityID, MonoString* propertyName, Color* tint) {
		Entity* entity = GetEntity(entityID);

		char* propertyCStr = mono_string_to_utf8(propertyName);

		if (entity) {
			RenderData* render = dynamic_cast<RenderData*>(entity->GetDataComponent(ComponentType::Render));

			auto matHandle = render->GetMaterialHandle();
			Material* mat = s_Data->ResourceManager->GetMaterial(matHandle);
			MaterialProperty property;
			property.Type = MaterialPropertyType::Color;
			property.Value = *tint;
			mat->SetProperty(propertyCStr, property);
		}
    }
    
	void ScriptGlue::Init(SceneManager* sceneManager, ScriptEngine* scriptEngine, ResourceManager* resourceManager) {
		s_Data = new Data();
		s_Data->SceneManager = sceneManager;
		s_Data->ScriptEngine = scriptEngine;
		s_Data->ResourceManager = resourceManager;
	}

	void ScriptGlue::Bind() {
        mono_add_internal_call("PhezuEngine.InternalCalls::Entity_GetTag", reinterpret_cast<const void*>(&Entity_GetTag));
        mono_add_internal_call("PhezuEngine.InternalCalls::Entity_HasComponent", reinterpret_cast<const void*>(&Entity_HasComponent));
        mono_add_internal_call("PhezuEngine.InternalCalls::Entity_GetComponent", reinterpret_cast<const void*>(&Entity_GetComponent));
        mono_add_internal_call("PhezuEngine.InternalCalls::Entity_RemoveComponent", reinterpret_cast<const void*>(&Entity_RemoveComponent));
        mono_add_internal_call("PhezuEngine.InternalCalls::Entity_Instantiate", reinterpret_cast<const void*>(&Entity_Instantiate));
        mono_add_internal_call("PhezuEngine.InternalCalls::Entity_Destroy", reinterpret_cast<const void*>(&Entity_Destroy));
        
		mono_add_internal_call("PhezuEngine.InternalCalls::Transform_GetPosition", reinterpret_cast<const void*>(&Transform_GetPosition));
		mono_add_internal_call("PhezuEngine.InternalCalls::Transform_SetPosition", reinterpret_cast<const void*>(&Transform_SetPosition));
        
        mono_add_internal_call("PhezuEngine.InternalCalls::Physics_GetVelocity", reinterpret_cast<const void*>(&Physics_GetVelocity));
        mono_add_internal_call("PhezuEngine.InternalCalls::Physics_SetVelocity", reinterpret_cast<const void*>(&Physics_SetVelocity));
        
        mono_add_internal_call("PhezuEngine.InternalCalls::Renderer_GetColor", reinterpret_cast<const void*>(&Renderer_GetColor));
        mono_add_internal_call("PhezuEngine.InternalCalls::Renderer_SetColor", reinterpret_cast<const void*>(&Renderer_SetColor));
	}

	void ScriptGlue::Destroy() {
		delete s_Data;
		s_Data = nullptr;
	}
}
