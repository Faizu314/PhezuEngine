#include "scripting/ScriptClass.hpp"
#include "Logger.hpp"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

namespace Phezu {

	ScriptClass::ScriptClass(MonoAssembly* assembly, const std::string& classNamespace, const std::string& className) 
	: m_ClassName(className), m_Namespace(classNamespace) {
		MonoImage* image = mono_assembly_get_image(assembly);
		m_Class = mono_class_from_name(image, classNamespace.c_str(), className.c_str());

		if (m_Class == nullptr)
		{
			Log("Error getting mono class from name");
		}
	}

	MonoMethod* ScriptClass::GetMonoMethod(const std::string& methodName, int paramterCount) {
		MonoMethod* method = mono_class_get_method_from_name(m_Class, methodName.c_str(), paramterCount);

		if (method == nullptr)
		{
			Log("%s method not found in script class: %s.%s\n", methodName.c_str(), m_Namespace.c_str(), m_ClassName.c_str());
		}

		return method;
	}
};