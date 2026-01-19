// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

namespace Phezu {

	class ScriptEngine;
	class SceneManager;
	class ResourceManager;

	class ScriptGlue {
	public:
		void static Init(SceneManager* sceneManager, ScriptEngine* scriptEngine, ResourceManager* resourceManager);
		void static Bind();
		void static Destroy();
	};
}
