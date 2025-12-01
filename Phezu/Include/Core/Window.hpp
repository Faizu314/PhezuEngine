#pragma once

namespace Phezu {

	class IWindow {
	public:
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual int GetRenderScale() const = 0;
	};
}