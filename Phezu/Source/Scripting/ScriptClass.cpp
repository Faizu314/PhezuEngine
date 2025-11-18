#include "Scripting/ScriptClass.hpp"
#include "Platform/Logger.hpp"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

namespace Phezu {

	ScriptClass::ScriptClass(MonoClass* monoClass, const std::string& classNamespace, const std::string& className, ScriptClassType scriptClassType) : m_Class(monoClass), m_ClassName(className), m_Namespace(classNamespace), m_ScriptClassType(scriptClassType) {}
    
    ScriptClass* ScriptClass::TryCreate(MonoAssembly* assembly, const std::string &classNamespace, const std::string &className, ScriptClassType scriptClassType) {
        MonoImage* image = mono_assembly_get_image(assembly);
        MonoClass* monoClass = mono_class_from_name(image, classNamespace.c_str(), className.c_str());
        
        if (monoClass == nullptr) {
        	MonoAssemblyName* assemblyName = mono_assembly_get_name(assembly);
        	const char* name = mono_assembly_name_get_name(assemblyName);

	        Log("Unable to find class %s.%s in assembly %s\n", classNamespace.c_str(), className.c_str(), name);
            return nullptr;
        }

        return new ScriptClass(monoClass, classNamespace, className, scriptClassType);
    }

	MonoMethod* ScriptClass::GetMonoMethod(const std::string& methodName, int parameterCount) {
		MonoMethod* method = mono_class_get_method_from_name(m_Class, methodName.c_str(), parameterCount);

		if (method == nullptr)
		{
			Log("%s method not found in script class: %s.%s\n", methodName.c_str(), m_Namespace.c_str(), m_ClassName.c_str());
		}

		return method;
	}

	MonoProperty* ScriptClass::GetMonoProperty(const std::string& propertyName) {
		MonoProperty* property = mono_class_get_property_from_name(m_Class, propertyName.c_str());

		if (property == nullptr) {
			Log("%s property not found in script class: %s.%s\n", propertyName.c_str(), m_Namespace.c_str(), m_ClassName.c_str());
		}

		return property;
	}

	MonoClassField* ScriptClass::GetMonoClassField(const std::string& fieldName) {
		MonoClassField* field = mono_class_get_field_from_name(m_Class, fieldName.c_str());

		if (field == nullptr) {
			Log("%s property not found in script class: %s.%s\n", fieldName.c_str(), m_Namespace.c_str(), m_ClassName.c_str());
		}

		return field;
	}
};
