#pragma once

#include "Maths/Math.hpp"

namespace Phezu {

	struct InputData {
		bool W, A, S, D;
		bool Up, Right, Down, Left;
		bool Space, F, G, H;
		Vector2 MousePos;
		bool LeftMouse, RightMouse, MiddleMouse;
	};

	class IInput {
	public:
		virtual void Init() = 0;
		virtual void Destroy() = 0;
		virtual bool PollInput() = 0;
		virtual const InputData& GetInput() = 0;
	};

	IInput* CreateInput();
}