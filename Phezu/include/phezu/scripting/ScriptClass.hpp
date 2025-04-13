#pragma once

#include "scripting/MonoDefs.hpp"
#include <string>

namespace Phezu {
	class ScriptClass {
	public:
		ScriptClass(MonoAssembly* assembly, const std::string& classNamespace, const std::string& className);
	public:
		MonoClass* GetMonoClass() { return m_Class; }
		MonoMethod* GetMonoMethod(const std::string& methodName, int paramterCount);
	private:
		std::string m_Namespace;
		std::string m_ClassName;
		MonoClass* m_Class;
	};
}