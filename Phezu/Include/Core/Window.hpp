#pragma once

#include <functional>

namespace Phezu {

	using ResizeCallback = std::function<void(int width, int height)>;

	class IWindow {
	public:
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual int GetRenderScale() const = 0;
		virtual int RegisterWindowResizeCallback(const ResizeCallback& callback) = 0;
		virtual void UnregisterWindowResizeCallback(int subscriberId) = 0;
	};
}