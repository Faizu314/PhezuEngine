#pragma once

#include "nlohmann/json.hpp"

#include "Core/Defs/Assert.hpp"
#include "Core/Platform.hpp"
#include "Assets/Types/ShaderAsset.hpp"

namespace Phezu {

    struct ParsedVertexInput {
        std::string Name;
        std::string DataType;
        std::string Semantic;
    };

    static std::vector<std::string> SplitLines(const std::string& text)
    {
        std::vector<std::string> lines;

        size_t start = 0;
        while (start < text.size())
        {
            size_t end = text.find('\n', start);
            if (end == std::string::npos)
                end = text.size();

            lines.emplace_back(text.substr(start, end - start));
            start = end + 1;
        }

        return lines;
    }

    static std::string Trim(const std::string& s)
    {
        size_t begin = 0;
        while (begin < s.size() && std::isspace((unsigned char)s[begin]))
            ++begin;

        size_t end = s.size();
        while (end > begin && std::isspace((unsigned char)s[end - 1]))
            --end;

        return s.substr(begin, end - begin);
    }

    static ParsedVertexInput ParseVertexInputLine(std::string& line)
    {
        line = Trim(line);

        PZ_ASSERT(!line.empty(), "Shader compiler error.\n");

        size_t colon = line.find(':');

        PZ_ASSERT(colon != std::string_view::npos, "Shader compiler error.\n");

        std::string left = Trim(line.substr(0, colon));
        std::string right = Trim(line.substr(colon + 1));

        size_t space = left.find(' ');

        PZ_ASSERT(space != std::string::npos, "Shader compiler error.\n");

        ParsedVertexInput result;
        result.DataType = Trim(left.substr(0, space));
        result.Name = Trim(left.substr(space + 1));
        result.Semantic = right;

        return result;
    }


    void ShaderAsset::Deserialize(const std::string& data) {
        const std::string vertexTag = "##Vertex";
        const std::string fragmentTag = "##Fragment";

        size_t vertexPos = data.find(vertexTag);
        size_t fragmentPos = data.find(fragmentTag);

        PZ_ASSERT(vertexPos != std::string::npos && fragmentPos != std::string::npos, "Shader compiler error.\n");

        vertexPos += vertexTag.length();

        std::string vertexStr = data.substr(
            vertexPos,
            fragmentPos - vertexPos
        );

        size_t vertexInputStart = 0, vertexInputEnd = 0;
        std::string vertexInputBlock = FindVertexInputBlock(vertexStr, &vertexInputStart, &vertexInputEnd);
        std::string vertexInputGlsl = ParseVertexInputBlock(vertexInputBlock);

        VertexSource = ReplaceVertexInputBlock(vertexStr, vertexInputStart, vertexInputEnd, vertexInputGlsl);

        fragmentPos += fragmentTag.length();

        FragmentSource = data.substr(fragmentPos);
    }

    std::string ShaderAsset::FindVertexInputBlock(const std::string& src, size_t* start, size_t* end) {
        const std::string keyword = "VertexInput";

        size_t keywordPos = src.find(keyword);

        PZ_ASSERT(keywordPos != std::string::npos, "Shader compiler error: VertexInput block missing.\n");

        *start = keywordPos;

        size_t braceOpen = src.find('{', keywordPos);

        PZ_ASSERT(braceOpen != std::string::npos, "Shader compiler error.\n")

        size_t i = braceOpen + 1;

        for (; i < src.size(); i++)
        {
            if (src[i] == '}')
                break;
        }

        PZ_ASSERT(i < src.size(), "Shader compiler error.\n")

        *end = i;

        return src.substr(*start, *end - *start + 1);
    }

    std::string ShaderAsset::ReplaceVertexInputBlock(const std::string& src, size_t blockStart, size_t blockEnd, const std::string& replacement) {
        std::string glsl;

        glsl.reserve(src.size());
        glsl.append(src, 0, blockStart);
        glsl.append(replacement);
        glsl.append(src, blockEnd + 1, std::string::npos);

        return glsl;
    }

    std::string ShaderAsset::ParseVertexInputBlock(const std::string& src) {
        std::string glsl;

        size_t braceOpen = src.find('{');

        size_t braceClose = src.find('}', braceOpen);

        std::string inputBlock = src.substr(braceOpen + 1, braceClose - braceOpen - 1);

        inputBlock = Trim(inputBlock);

        unsigned int location = 0;

        for (auto line : SplitLines(inputBlock))
        {
            auto parsed = ParseVertexInputLine(line);

            VertexSemantic semantic = ToVertexSemantic(parsed.Semantic);
            Semantics.insert(std::make_pair(semantic, location));

            glsl += "layout (location = " + std::to_string(location) + ") in " + parsed.DataType + " " + parsed.Name + ";\n";

            location++;
        }

        return glsl;
    }

}