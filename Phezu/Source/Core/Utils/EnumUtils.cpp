#pragma once

#include <unordered_map>

#include "Core/Defs/Assert.hpp"
#include "Core/Utils/EnumUtils.hpp"
#include "Core/Types/Types.hpp"
#include "Core/Platform.hpp"

namespace Phezu {

	#define ENUM_TO_MAP_ENTRY(EnumType, x) {#x, EnumType::x},

	#define DEFINE_ENUM_STRING_MAP(EnumType, ENUM_LIST, DEFAULT_VALUE)                  \
		static std::unordered_map<std::string, EnumType> s_StrTo##EnumType = {          \
			ENUM_LIST(EnumType, ENUM_TO_MAP_ENTRY)                                      \
		};                                                                              \
																						\
		EnumType To##EnumType(const std::string& str) {                                 \
			auto it = s_StrTo##EnumType.find(str);                                      \
			if (it == s_StrTo##EnumType.end()) {                                        \
				const char* enumTypeStr = #EnumType;                                    \
				Log("Unknown %s: %s\n", enumTypeStr, str.c_str());                      \
				return EnumType::DEFAULT_VALUE;                                         \
			}                                                                           \
			return it->second;                                                          \
		}                                                                               \
																						\
		std::string ToString(EnumType value) {											\
			for (auto& kvp : s_StrTo##EnumType) {										\
				if (kvp.second == value)												\
					return kvp.first;													\
			}																			\
			const char* str = #EnumType;												\
			PZ_ASSERT(false, "Unknown %s, value: %i", str, static_cast<int>(value))		\
			return "";																	\
		}																				

	#define VERTEX_SEMANTICS_LIST(E, X)       \
		X(E, Position)                        \
		X(E, Normal)                          \
		X(E, Color)                           \
		X(E, TexCoord0)                       \
		X(E, TexCoord1)                       \
		X(E, TexCoord2)                       \
		X(E, TexCoord3)                       \
		X(E, Custom0)                         \
		X(E, Custom1)                         \
		X(E, Custom2)                         \
		X(E, Custom3)					 

	#define VERTEX_ATTRIBUTE_TYPE_LIST(E, X)  \
		X(E, Float)                           \
		X(E, Int)                             \
		X(E, UInt)                            \
		X(E, Byte)                            \
		X(E, UByte)

	#define VERTEX_ATTRIBUTE_COUNT_LIST(E, X) \
		X(E, One)                             \
		X(E, Two)                             \
		X(E, Three)                           \
		X(E, Four)

	#define TEXTURE_FILTERING_MODE_LIST(E, X) \
		X(E, Point)                           \
		X(E, Bilinear)

	#define TEXTURE_WRAP_MODE_LIST(E, X)      \
		X(E, Repeat)                          \
		X(E, MirroredRepeat)                  \
		X(E, ClampToEdge)                     \
		X(E, ClampToBorder)

	#define ASSET_SOURCE_LIST(E, X)			  \
		X(E, Engine)                          \
		X(E, Project)

	#define ASSET_TYPE_LIST(E, X)		      \
		X(E, None)                            \
		X(E, Scene)                           \
		X(E, Prefab)                          \
		X(E, Mesh)                            \
		X(E, Shader)                          \
		X(E, Image)                           \
		X(E, Texture)                         \
		X(E, Material)                        \
		X(E, Config)

	#define MANAGED_TYPE_LIST(E, X)           \
		X(E, None)                            \
		X(E, Transform)                       \
		X(E, Shape)                           \
		X(E, Renderer)                        \
		X(E, Physics)                         \
		X(E, ScriptComponent)                 \

	DEFINE_ENUM_STRING_MAP(VertexSemantic, VERTEX_SEMANTICS_LIST, Position);
	DEFINE_ENUM_STRING_MAP(VertexAttributeType, VERTEX_ATTRIBUTE_TYPE_LIST, Float);
	DEFINE_ENUM_STRING_MAP(VertexAttributeCount, VERTEX_ATTRIBUTE_COUNT_LIST, One);
	DEFINE_ENUM_STRING_MAP(TextureFilteringMode, TEXTURE_FILTERING_MODE_LIST, Point);
	DEFINE_ENUM_STRING_MAP(TextureWrapMode, TEXTURE_WRAP_MODE_LIST, Repeat);
	DEFINE_ENUM_STRING_MAP(AssetSource, ASSET_SOURCE_LIST, Project);
	DEFINE_ENUM_STRING_MAP(AssetType, ASSET_TYPE_LIST, None);
	DEFINE_ENUM_STRING_MAP(ManagedType, MANAGED_TYPE_LIST, None);

	unsigned int GetVertexAttributeSize(VertexAttributeType attribute) {
		switch (attribute) {
			case VertexAttributeType::Float:
				return sizeof(float);

			case VertexAttributeType::Int:
				return sizeof(int);
			case VertexAttributeType::UInt:
				return sizeof(unsigned int);

			case VertexAttributeType::Byte:
			case VertexAttributeType::UByte:
				return 1;

			default: {
				PZ_ASSERT(false, "Unknown VertexAttributeType.\n");
				return 0;
			}
		}
	}

	unsigned int GetVertexAttributeCount(VertexAttributeCount countEnum) {
		switch (countEnum) {
			case VertexAttributeCount::One:
				return 1;
			case VertexAttributeCount::Two:
				return 2;
			case VertexAttributeCount::Three:
				return 3;
			case VertexAttributeCount::Four:
				return 4;

			default: {
				PZ_ASSERT(false, "Unknown VertexAttributeCount.\n");
				return 0;
			}
		}
	}
}
