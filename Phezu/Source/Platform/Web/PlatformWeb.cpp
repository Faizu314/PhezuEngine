#include "Platform/Web/PlatformWeb.hpp"

#include "emscripten/html5.h"

#include "Core/Defs/Assert.hpp"
#include "Platform/Web/WindowWeb.hpp"
#include "Platform/Web/LoggerWeb.hpp"
#include "Graphics/Core/GraphicsAPI.hpp"
#include "Graphics/OpenGL/OpenGLAPI.hpp"

namespace Phezu {

	static WindowWeb* s_Window = nullptr;
	static LoggerWeb* s_Logger = nullptr;
	static IGraphicsAPI* s_GraphicsApi = nullptr;
	static InputData s_NullInput;

	PlatformWeb::PlatformWeb() {
		s_Window = new WindowWeb();
		s_Logger = new LoggerWeb();
	}

	void PlatformWeb::Init(const WindowArgs& args) {
		m_StartTime = emscripten_get_now();

		s_Logger->Init();
		s_Window->Init(args);

		CreateGraphicsContext();
	}

	void PlatformWeb::CreateGraphicsContext() {
		EmscriptenWebGLContextAttributes attr;
		emscripten_webgl_init_context_attributes(&attr);
		attr.majorVersion = 2;
		attr.minorVersion = 0;
		attr.alpha = false;
		attr.depth = false;
		attr.antialias = false;

		EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attr);

		if (ctx <= 0) {
			va_list dummyList;
			s_Logger->Log("Unable to create WebGL context\n", dummyList);
		}

		emscripten_webgl_make_context_current(ctx);

		s_GraphicsApi = new OpenGLAPI();
	}

	void PlatformWeb::PollEvents() {

	}

	IWindow* PlatformWeb::GetWindow() {
		return s_Window;
	}

	const InputData& PlatformWeb::GetInput() {
		return s_NullInput;
	}

	IGraphicsAPI* PlatformWeb::GetGraphicsApi() {
		return s_GraphicsApi;
	}


	void PlatformWeb::Update() {
		s_Window->Update();
	}

	void PlatformWeb::Wait(float delay) {

	}

	double PlatformWeb::GetTime() {
		return (emscripten_get_now() - m_StartTime) / 1000.0;
	}

	void PlatformWeb::Log(const char* msg, va_list args) {
		s_Logger->Log(msg, args);
	}

	void PlatformWeb::Destroy() {
		s_Window->Destroy();
		s_Logger->Destroy();
	}
}