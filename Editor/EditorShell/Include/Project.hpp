// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include <filesystem>
#include <vector>

namespace Phezu::Editor {

    struct Project {
        std::filesystem::path Path;
        std::vector<std::filesystem::path> ScriptFiles;
        std::vector<std::filesystem::path> SceneFiles;
        std::vector<std::filesystem::path> PrefabFiles;
        std::vector<std::filesystem::path> ShaderFiles;
        std::vector<std::filesystem::path> MaterialFiles;
        std::vector<std::filesystem::path> ImageFiles;
        std::vector<std::filesystem::path> TextureFiles;
        std::vector<std::filesystem::path> ConfigFiles;

        size_t GetFilesCount() {
            return ScriptFiles.size() + SceneFiles.size() + PrefabFiles.size() + ShaderFiles.size() + MaterialFiles.size() + ImageFiles.size() + TextureFiles.size() + ConfigFiles.size();
        }

        std::filesystem::path GetFilePathByIndex(size_t fileIndex) {
            if (fileIndex < ScriptFiles.size())
                return ScriptFiles[fileIndex];
            fileIndex -= ScriptFiles.size();

            if (fileIndex < SceneFiles.size())
                return SceneFiles[fileIndex];
            fileIndex -= SceneFiles.size();

            if (fileIndex < PrefabFiles.size())
                return PrefabFiles[fileIndex];
            fileIndex -= PrefabFiles.size();

            if (fileIndex < ShaderFiles.size())
                return ShaderFiles[fileIndex];
            fileIndex -= ShaderFiles.size();

            if (fileIndex < MaterialFiles.size())
                return MaterialFiles[fileIndex];
            fileIndex -= MaterialFiles.size();

            if (fileIndex < ImageFiles.size())
                return ImageFiles[fileIndex];
            fileIndex -= ImageFiles.size();

            if (fileIndex < TextureFiles.size())
                return TextureFiles[fileIndex];
            fileIndex -= TextureFiles.size();

            if (fileIndex < ConfigFiles.size())
                return ConfigFiles[fileIndex];
            
            return "";
        }
    };
}