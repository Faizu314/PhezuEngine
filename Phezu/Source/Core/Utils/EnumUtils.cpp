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
			ENUM_LIST																	\
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

	#define VERTEX_SEMANTICS_LIST														\
		ENUM_TO_MAP_ENTRY(VertexSemantic, Position)										\
		ENUM_TO_MAP_ENTRY(VertexSemantic, Normal)										\
		ENUM_TO_MAP_ENTRY(VertexSemantic, Color)										\
		ENUM_TO_MAP_ENTRY(VertexSemantic, TexCoord0)									\
		ENUM_TO_MAP_ENTRY(VertexSemantic, TexCoord1)									\
		ENUM_TO_MAP_ENTRY(VertexSemantic, TexCoord2)									\
		ENUM_TO_MAP_ENTRY(VertexSemantic, TexCoord3)									\
		ENUM_TO_MAP_ENTRY(VertexSemantic, Custom0)										\
		ENUM_TO_MAP_ENTRY(VertexSemantic, Custom1)										\
		ENUM_TO_MAP_ENTRY(VertexSemantic, Custom2)										\
		ENUM_TO_MAP_ENTRY(VertexSemantic, Custom3)					 

	#define VERTEX_ATTRIBUTE_TYPE_LIST													\
		ENUM_TO_MAP_ENTRY(VertexAttributeType, Float)									\
		ENUM_TO_MAP_ENTRY(VertexAttributeType, Int)										\
		ENUM_TO_MAP_ENTRY(VertexAttributeType, UInt)									\
		ENUM_TO_MAP_ENTRY(VertexAttributeType, Byte)									\
		ENUM_TO_MAP_ENTRY(VertexAttributeType, UByte)

	#define VERTEX_ATTRIBUTE_COUNT_LIST													\
		ENUM_TO_MAP_ENTRY(VertexAttributeCount, One)									\
		ENUM_TO_MAP_ENTRY(VertexAttributeCount, Two)									\
		ENUM_TO_MAP_ENTRY(VertexAttributeCount, Three)	   								\
		ENUM_TO_MAP_ENTRY(VertexAttributeCount, Four)

	#define TEXTURE_FILTERING_MODE_LIST													\
		ENUM_TO_MAP_ENTRY(TextureFilteringMode, Point)									\
		ENUM_TO_MAP_ENTRY(TextureFilteringMode, Bilinear)

	#define TEXTURE_WRAP_MODE_LIST														\
		ENUM_TO_MAP_ENTRY(TextureWrapMode, Repeat)										\
		ENUM_TO_MAP_ENTRY(TextureWrapMode, MirroredRepeat)								\
		ENUM_TO_MAP_ENTRY(TextureWrapMode, ClampToEdge)									\
		ENUM_TO_MAP_ENTRY(TextureWrapMode, ClampToBorder)

	#define ASSET_SOURCE_LIST															\
		ENUM_TO_MAP_ENTRY(AssetSource, Engine)											\
		ENUM_TO_MAP_ENTRY(AssetSource, Project)

	#define ASSET_TYPE_LIST																\
		ENUM_TO_MAP_ENTRY(AssetType, None)												\
		ENUM_TO_MAP_ENTRY(AssetType, Scene)												\
		ENUM_TO_MAP_ENTRY(AssetType, Prefab)											\
		ENUM_TO_MAP_ENTRY(AssetType, Mesh)												\
		ENUM_TO_MAP_ENTRY(AssetType, Shader)											\
		ENUM_TO_MAP_ENTRY(AssetType, Image)												\
		ENUM_TO_MAP_ENTRY(AssetType, Texture)											\
		ENUM_TO_MAP_ENTRY(AssetType, Material)											\
		ENUM_TO_MAP_ENTRY(AssetType, Config)

	#define MANAGED_TYPE_LIST															\
		ENUM_TO_MAP_ENTRY(ManagedType, None)											\
		ENUM_TO_MAP_ENTRY(ManagedType, Transform)										\
		ENUM_TO_MAP_ENTRY(ManagedType, Shape)											\
		ENUM_TO_MAP_ENTRY(ManagedType, Renderer)										\
		ENUM_TO_MAP_ENTRY(ManagedType, Rigidbody)										\
		ENUM_TO_MAP_ENTRY(ManagedType, ScriptComponent)									\

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
