#include "EditorShell.hpp"
#include "Project.hpp"
#include "EditorDefines.hpp"

#include <sstream>

namespace Phezu::Editor {

    EditorShell::EditorShell() : m_OpenedProject(nullptr) {}

    void EditorShell::ExecuteCommand(const Command& command) {
        printf("Executing command of type %i\n", command.Type);

        switch (command.Type) {
            case CommandType::Open: {
                std::filesystem::path projectPath = command.Arguments[0];
                TryOpenProject(projectPath);
                break;
            }
            case CommandType::Build: {
                std::filesystem::path buildDir = command.Arguments[0];
                TryBuild(buildDir);
                break;
            }
        }
    }

    void EditorShell::TryOpenProject(const std::filesystem::path& projectPath) {
        if (!std::filesystem::exists(projectPath)) {
            printf("Invalid path: %ls\n", projectPath.c_str());
            return;
        }
        if (m_OpenedProject != nullptr) {
            printf("Project already open at %ls\n", m_OpenedProject->Path.c_str());
            return;
        }

        m_OpenedProject = new Project();
        m_OpenedProject->Path = projectPath;

        GetFilesPathRecursively(projectPath);
    }

    void EditorShell::TryBuild(const std::filesystem::path& buildDir) {
        if (m_OpenedProject == nullptr) {
            printf("No project opened\n");
            return;
        }

        std::ostringstream stringBuilder;

        stringBuilder << "mkdir \"" << buildDir.string() << "\"";
        std::system(stringBuilder.str().c_str());
        stringBuilder.str("");

        stringBuilder << "mkdir \"" << buildDir.string() << "/Assets\"";
        std::system(stringBuilder.str().c_str());
        stringBuilder.str("");

        stringBuilder << CSHARP_BUILD_COMMAND << " -target:library -out:" << buildDir.string() << "/Game.dll ";

        for (int i = 0; i < m_OpenedProject->ScriptFiles.size(); i++) {
            stringBuilder << "\"" << m_OpenedProject->ScriptFiles[i].string() << "\" ";
        }

        stringBuilder << "-reference:\"" << SCRIPT_CORE_DLL_SRC_DIR << "/Phezu-ScriptCore.dll\"";

        printf("Executing Command: %s\n", stringBuilder.str().c_str());

        std::system(stringBuilder.str().c_str());

        std::string dest = (buildDir / "Assets").string();

        for (int i = 0; i < m_OpenedProject->PrefabFiles.size(); i++) {
            stringBuilder.str("");
            stringBuilder << "copy \"" << m_OpenedProject->PrefabFiles[i].string() << "\" ";
            stringBuilder << "\"" << dest << "\"";

            std::system(stringBuilder.str().c_str());
        }
        for (int i = 0; i < m_OpenedProject->SceneFiles.size(); i++) {
            stringBuilder.str("");
            stringBuilder << "copy \"" << m_OpenedProject->SceneFiles[i].string() << "\" ";
            stringBuilder << "\"" << dest << "\"";

            std::system(stringBuilder.str().c_str());
        }
        for (int i = 0; i < m_OpenedProject->ConfigFiles.size(); i++) {
            stringBuilder.str("");
            stringBuilder << "copy \"" << m_OpenedProject->ConfigFiles[i].string() << "\" ";
            stringBuilder << "\"" << dest << "\"";

            std::system(stringBuilder.str().c_str());
        }
    }

    void EditorShell::CloseProject() {
        if (m_OpenedProject == nullptr)
            return;

        delete m_OpenedProject;
        m_OpenedProject = nullptr;
    }

    void EditorShell::GetFilesPathRecursively(const std::filesystem::path& folder) {
        for (const auto& entry : std::filesystem::directory_iterator(folder)) {
            if (entry.is_regular_file()) {
                if (entry.path().extension() == ".cs") {
                    m_OpenedProject->ScriptFiles.push_back(entry);
                }
                else if (entry.path().extension() == ".scene") {
                    m_OpenedProject->SceneFiles.push_back(entry);
                }
                else if (entry.path().extension() == ".prefab") {
                    m_OpenedProject->PrefabFiles.push_back(entry);
                }
                else if (entry.path().extension() == ".config") {
                    m_OpenedProject->ConfigFiles.push_back(entry);
                }
                else if (entry.path().extension() == ".meta") {
                    std::filesystem::path stem = entry.path().stem();

                    if (stem.extension() == ".scene") {
                        m_OpenedProject->SceneFiles.push_back(entry);
                    }
                    else if (stem.extension() == ".prefab") {
                        m_OpenedProject->PrefabFiles.push_back(entry);
                    }
                    else if (stem.extension() == ".config") {
                        m_OpenedProject->ConfigFiles.push_back(entry);
                    }
                }
            }
            else if (entry.is_directory()) {
                GetFilesPathRecursively(entry.path());
            }
        }
    }

    void EditorShell::Destroy() {
        CloseProject();
    }
}