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
        void OpenProject(std::filesystem::path projectPath);
        void CloseProject();
        void GetScriptsPathRecursively(const std::filesystem::path& directory);
    private:
        Project* m_OpenedProject;
    };
}