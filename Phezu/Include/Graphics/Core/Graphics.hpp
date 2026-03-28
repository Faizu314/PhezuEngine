// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <stdint.h>
#include <variant>

#include "Core/Types/Types.hpp"
#include "Maths/Objects/Vector2.hpp"
#include "Maths/Objects/Vector3.hpp"

namespace Phezu {

	inline constexpr uint64_t INVALID_RESOURCE_ID = 0;

	enum class ResourceType {
		None = 0,
		Material,
		Mesh,
		Shader,
		Texture
	};

	struct ResourceHandle {
	public:
		ResourceHandle() = default;
		ResourceHandle(uint64_t id, ResourceType type = ResourceType::None) : m_ID(id), m_Type(type) {}
	public:
		uint64_t GetID() const { return m_ID; }
		ResourceType GetType() const { return m_Type; }
	public:
		bool operator==(const ResourceHandle& other) const { return m_ID == other.m_ID; }
		bool operator==(const uint64_t other) const { return m_ID == other; }
	private:
		uint64_t m_ID = INVALID_RESOURCE_ID;
		ResourceType m_Type = ResourceType::None;
	};

	struct ResourcePtr {
		uint64_t Ptr = 0;
	};

	// Vertex Types

	enum class VertexSemantic : uint8_t {
		Position,
		Normal,
		Color,
		TexCoord0,
		TexCoord1,
		TexCoord2,
		TexCoord3,
		Custom0,
		Custom1,
		Custom2,
		Custom3
	};

	enum class VertexAttributeType : uint8_t {
		Float,
		Int,
		UInt,
		Byte,
		UByte
	};

	enum class VertexAttributeCount : uint8_t {
		One = 1,
		Two = 2,
		Three = 3,
		Four = 4
	};

	struct VertexAttribute {
		VertexAttribute() = default;

		VertexAttribute(VertexSemantic semantic, VertexAttributeType type, VertexAttributeCount count, bool normalized = true)
			: Semantic(semantic), AttributeType(type), AttributeCount(count), Normalized(normalized) {}

		VertexSemantic Semantic;
		VertexAttributeType AttributeType;
		VertexAttributeCount AttributeCount;
		size_t Offset = 0;
		bool Normalized = true;
	};

	enum class BufferType {
		Static,
		Dynamic
	};

	class IFrameBuffer;

	struct RenderTarget {
		IFrameBuffer* Target;

		static RenderTarget Default() { return { nullptr }; }
		static RenderTarget Offscreen(IFrameBuffer* fbo) { return { fbo }; }
	};

	// Texture Types

	enum class TextureWrapMode {
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	enum class TextureFilteringMode {
		Point,
		Bilinear
	};

	struct SamplerDesc {
		TextureWrapMode WrapMode;
		TextureFilteringMode FilteringMode;
	};

	// Material Types

	enum class MaterialPropertyType {
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

namespace std {
	template<>
	class hash<Phezu::ResourceHandle> {
	public:
		size_t operator()(const Phezu::ResourceHandle& h) const noexcept {
			return h.GetID();
		}
	};
}