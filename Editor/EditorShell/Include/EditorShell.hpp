#pragma once

#include "Command.hpp"
#include <filesystem>

namespace Phezu::Editor {

    struct Project;

    class EditorShell {
    public:
        EditorShell();
    public:
        void ExecuteCommand(const Command& command);
        void Destroy();
    private:
        void TryOpenProject(const std::filesystem::path& projectPath);
        void TryBuild(const std::filesystem::path& projectPath);
        void CloseProject();
        void GetFilesPathRecursively(const std::filesystem::path& directory);
    private:
        Project* m_OpenedProject;
    };
}