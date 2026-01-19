// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <variant>
#include <cstdint>

#include "Core/Types/Color.hpp"
#include "Maths/Objects/Vector2.hpp"
#include "Maths/Objects/Vector3.hpp"

namespace Phezu {

	enum class MaterialPropertyType : uint8_t {
		None = 0,
		Float,
		Float2,
		Float3,
		Color,
		Int,
		Bool
	};

	using MaterialPropertyValue = std::variant<float, Vector2, Vector3, Color, int, bool>;

	struct MaterialProperty {
		MaterialPropertyType Type;
		MaterialPropertyValue Value;
	};
}