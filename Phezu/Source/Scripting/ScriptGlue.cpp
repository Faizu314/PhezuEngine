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
    
    uint32_t Entity_Instantiate(GUID prefabGuid, uint64_t source) {
		AssetHandle handle { prefabGuid, static_cast<AssetSource>(source) };

		Entity* entity = s_Data->SceneManager->GetMasterScene()->CreateEntity(handle);
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

	/*----Renderer-Internal-Calls----*/

	uint64_t Renderer_GetMaterial(uint64_t entityID) {
		Entity* entity = GetEntity(entityID);

		if (entity) {
			RenderData* render = dynamic_cast<RenderData*>(entity->GetDataComponent(ComponentType::Render));

			return s_Data->ResourceManager->GetMaterialID(render->GetMaterial());
		}

		Log("Throw C# error here: entity does not exist\n");
		return 0;
	}

	void Renderer_SetMaterial(uint64_t entityID, uint64_t materialID) {
		Entity* entity = GetEntity(entityID);

		if (entity == nullptr) {
			Log("Throw C# error here: entity does not exist\n");
			return;
		}
		
		Material* mat = s_Data->ResourceManager->GetMaterial(materialID);

		if (mat == nullptr) {
			Log("Throw C# error here: invalid material\n");
			return;
		}

		if (entity) {
			RenderData* render = dynamic_cast<RenderData*>(entity->GetDataComponent(ComponentType::Render));

			render->SetMaterial(mat);
		}
	}

	/*----Material-Internal-Calls----*/


	uint64_t Material_Create(uint64_t sourceMaterialID) {
		Material* mat = s_Data->ResourceManager->GetMaterial(sourceMaterialID);

		if (mat == nullptr) {
			Log("Throw C# error here: invalid material\n");
			return 0;
		}

		return s_Data->ResourceManager->CreateUserMaterial(sourceMaterialID);
	}

	uint64_t Material_Get(GUID guid, uint64_t source) {
		AssetHandle handle{ guid, static_cast<AssetSource>(source) };

		Material* mat = s_Data->ResourceManager->GetMaterial(handle);

		if (mat == nullptr) {
			Log("Throw C# error here: invalid material asset ref\n");
			return 0;
		}

		return s_Data->ResourceManager->GetMaterialID(mat);
	}

	void Material_Destroy(uint64_t materialID) {
		s_Data->ResourceManager->DestroyUserMaterial(materialID);
	}

    void Material_GetColor(uint64_t materialID, MonoString* propertyName, Color* tint) {
		Material* mat = s_Data->ResourceManager->GetMaterial(materialID);

		if (mat == nullptr) {
			Log("Throw C# error here: invalid material\n");
			return;
		}

		char* propertyCStr = mono_string_to_utf8(propertyName);

		MaterialProperty property = mat->GetProperty(propertyCStr);
			
		if (property.Type != MaterialPropertyType::Color) {
			*tint = Color::Clear;
			return;
		}

		*tint = std::get<Color>(property.Value);
    }
    
    void Material_SetColor(uint64_t materialID, MonoString* propertyName, Color* tint) {
		if (!s_Data->ResourceManager->IsUserMaterial(materialID)) {
			Log("Throw C# error here: writing to a readonly material\n");
			return;
		}

		Material* mat = s_Data->ResourceManager->GetMaterial(materialID);

		if (mat == nullptr) {
			Log("Throw C# error here: invalid material\n");
			return;
		}

		char* propertyCStr = mono_string_to_utf8(propertyName);

		MaterialProperty property;
		property.Type = MaterialPropertyType::Color;
		property.Value = *tint;

		mat->SetProperty(propertyCStr, property);
    }
    
	void ScriptGlue::Init(SceneManager* sceneManager, ScriptEngine* scriptEngine, ResourceManager* resourceManager) {
		s_Data = new Data();
		s_Data->SceneManager = sceneManager;
		s_Data->ScriptEngine = scriptEngine;
		s_Data->ResourceManager = resourceManager;
	}

	void ScriptGlue::Destroy() {
		delete s_Data;
		s_Data = nullptr;
	}

	static const std::string prefix = "PhezuEngine.InternalCalls::";

	#define INTERNAL_CALL(x) mono_add_internal_call((prefix + std::string(#x)).c_str(), reinterpret_cast<const void*>(&##x))

	void ScriptGlue::Bind() {
		INTERNAL_CALL(Entity_GetTag);
		INTERNAL_CALL(Entity_HasComponent);
		INTERNAL_CALL(Entity_GetComponent);
		INTERNAL_CALL(Entity_RemoveComponent);
		INTERNAL_CALL(Entity_Instantiate);
		INTERNAL_CALL(Entity_Destroy);

		INTERNAL_CALL(Transform_GetPosition);
		INTERNAL_CALL(Transform_SetPosition);

		INTERNAL_CALL(Physics_GetVelocity);
		INTERNAL_CALL(Physics_SetVelocity);


		INTERNAL_CALL(Renderer_GetMaterial);
		INTERNAL_CALL(Renderer_SetMaterial);

		INTERNAL_CALL(Material_Create);
		INTERNAL_CALL(Material_Get);
		INTERNAL_CALL(Material_Destroy);
		INTERNAL_CALL(Material_GetColor);
		INTERNAL_CALL(Material_SetColor);
	}
}
