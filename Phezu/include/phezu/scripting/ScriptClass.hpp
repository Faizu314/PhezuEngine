#pragma once

#include "scripting/MonoDefs.hpp"
#include <string>

namespace Phezu {
	enum class ScriptClassType {
		None,

		//Engine types
		Object,
		Entity,
		Component,
		BehaviourComponent,

		//User types
		ScriptComponent,
		CSharpClass,
	};

	class ScriptClass {
	public:
		ScriptClass(MonoAssembly* assembly, const std::string& classNamespace, const std::string& className, ScriptClassType scriptClassType);
	public:
		MonoClass* GetMonoClass() { return m_Class; }
		MonoMethod* GetMonoMethod(const std::string& methodName, int paramterCount);
		MonoProperty* GetMonoProperty(const std::string& propertyName);
		MonoClassField* GetMonoClassField(const std::string& fieldName);
		ScriptClassType GetScriptClassType() { return m_ScriptClassType; }
		std::string GetFullname() { return m_Namespace + "." + m_ClassName; }
	private:
		std::string m_Namespace;
		std::string m_ClassName;
		MonoClass* m_Class;
		ScriptClassType m_ScriptClassType;
	};
}