#pragma once

#include <string>

#include "Scripting/MonoDefs.hpp"

namespace Phezu {
	enum class ScriptClassType {
		None,

		//Engine types
		Object,
        Entity,
        Component,
        EngineComponent,
        BehaviourComponent,
        CSharpClass,

		//User types
		ScriptComponent,
	};

	class ScriptClass {
	private:
		ScriptClass(MonoClass* monoClass, const std::string& classNamespace, const std::string& className, ScriptClassType scriptClassType);
	public:
		MonoClass* GetMonoClass() { return m_Class; }
		MonoMethod* GetMonoMethod(const std::string& methodName, int parameterCount);
		MonoProperty* GetMonoProperty(const std::string& propertyName);
		MonoClassField* GetMonoClassField(const std::string& fieldName);
		ScriptClassType GetScriptClassType() const { return m_ScriptClassType; }
		std::string GetFullname() const { return m_Namespace + "." + m_ClassName; }
    public:
        static ScriptClass* TryCreate(MonoAssembly* assembly, const std::string& classNamespace, const std::string& className, ScriptClassType scriptClassType);
	private:
		std::string m_Namespace;
		std::string m_ClassName;
		MonoClass* m_Class;
		ScriptClassType m_ScriptClassType;
	};
}
