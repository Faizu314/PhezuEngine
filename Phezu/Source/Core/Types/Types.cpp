#pragma once

#include <unordered_map>

#include "Core/Types/Types.hpp"
#include "Core/Platform.hpp"

namespace Phezu {

	static std::unordered_map<std::string, VertexSemantic> s_StrToVertSemantic = {
		{"Position",  VertexSemantic::Position},
		{"Normal",    VertexSemantic::Normal},
		{"Color",     VertexSemantic::Color},
		{"TexCoord0", VertexSemantic::TexCoord0},
		{"TexCoord1", VertexSemantic::TexCoord1},
		{"TexCoord2", VertexSemantic::TexCoord2},
		{"TexCoord3", VertexSemantic::TexCoord3},
		{"Custom0",   VertexSemantic::Custom0},
		{"Custom1",   VertexSemantic::Custom1},
		{"Custom2",   VertexSemantic::Custom2},
		{"Custom3",   VertexSemantic::Custom3}
	};

	static std::unordered_map<std::string, VertexAttributeType> s_StrToVertType = {
		{"Float",  VertexAttributeType::Float},
		{"Int",    VertexAttributeType::Int},
		{"UInt",     VertexAttributeType::UInt},
		{"Byte", VertexAttributeType::Byte},
		{"UByte", VertexAttributeType::UByte}
	};

	static std::unordered_map<std::string, VertexAttributeCount> s_StrToVertCount = {
		{"One",  VertexAttributeCount::One},
		{"Two",  VertexAttributeCount::Two},
		{"Three",  VertexAttributeCount::Three},
		{"Four",  VertexAttributeCount::Four}
	};


	VertexSemantic ToVertexSemantic(std::string semanticStr) {
		auto it = s_StrToVertSemantic.find(semanticStr);

		if (it == s_StrToVertSemantic.end()) {
			Log("Unknown vertex semantic: %s\n", semanticStr);

			return VertexSemantic::Position;
		}

		return it->second;
	}

	VertexAttributeType ToVertexAttributeType(std::string attributeTypeStr) {
		auto it = s_StrToVertType.find(attributeTypeStr);

		if (it == s_StrToVertType.end()) {
			Log("Unknown vertex attribute type: %s\n", attributeTypeStr);

			return VertexAttributeType::Float;
		}

		return it->second;
	}

	VertexAttributeCount ToVertexAttributeCount(std::string attributeCountStr) {
		auto it = s_StrToVertCount.find(attributeCountStr);

		if (it == s_StrToVertCount.end()) {
			Log("Unknown vertex attribute count: %s\n", attributeCountStr);

			return VertexAttributeCount::One;
		}

		return it->second;
	}

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
				Log("Should assert here\n");
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
				Log("Should assert here\n");
				return 0;
			}
		}
	}
}
