#include "EditorShell.hpp"
#include "Project.hpp"

namespace Phezu::Editor {

    EditorShell::EditorShell() = default;

    void EditorShell::ExecuteCommand(const Command& command) {
        printf("Executing command of type %i\n", command.Type);

        switch (command.Type) {
            case CommandType::Open:
                std::filesystem::path projectPath = command.Arguments[0];
                OpenProject(projectPath);
                break;
        }
    }

    void EditorShell::OpenProject(std::filesystem::path projectPath) {
        if (!std::filesystem::exists(projectPath)) {
            printf("Invalid path: %ls\n", projectPath.c_str());
            return;
        }

        m_OpenedProject = new Project();
        m_OpenedProject->Path = projectPath;

        GetScriptsPathRecursively(projectPath);
    }

    void EditorShell::CloseProject() {
        if (m_OpenedProject == nullptr)
            return;

        delete m_OpenedProject;
        m_OpenedProject = nullptr;
    }

    void EditorShell::GetScriptsPathRecursively(const std::filesystem::path& folder) {
        for (const auto& entry : std::filesystem::directory_iterator(folder)) {
            if (entry.is_regular_file() && entry.path().extension() == ".cs") {
                m_OpenedProject->Scripts.push_back(entry);
            }
            else if (entry.is_directory()) {
                GetScriptsPathRecursively(entry.path());
            }
        }
    }

    void EditorShell::Destroy() {
        CloseProject();
    }
}