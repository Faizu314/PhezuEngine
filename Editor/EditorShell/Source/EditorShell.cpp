#include "EditorShell.hpp"
#include "Project.hpp"
#include "EditorDefines.hpp"

#include <sstream>
#include <windows.h>

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

        GetFilesPathInDirectory(projectPath);
        GetFilesPathRecursively(projectPath / "Assets");
    }

    void EditorShell::TryBuild(const std::filesystem::path& buildDir) {
        if (m_OpenedProject == nullptr) {
            printf("No project opened\n");
            return;
        }

        std::filesystem::create_directories(buildDir);
        std::filesystem::create_directory(buildDir / "Assets");

        {
            std::ostringstream stringBuilder;

            stringBuilder << CSHARP_BUILD_COMMAND << " -target:library -out:\"" << buildDir.string() << "/Game.dll\" ";

            for (int i = 0; i < m_OpenedProject->ScriptFiles.size(); i++) {
                stringBuilder << "\"" << m_OpenedProject->ScriptFiles[i].string() << "\" ";
            }

            stringBuilder << "-reference:\"" << SCRIPT_CORE_DLL_SRC_DIR << "/Phezu-ScriptCore.dll\"";

            printf("Executing Command: %s\n", stringBuilder.str().c_str());

            std::system(stringBuilder.str().c_str());
        }

        std::filesystem::path dest = buildDir / "Assets";

        for (int i = 0; i < m_OpenedProject->PrefabFiles.size(); i++) {
            std::filesystem::copy(m_OpenedProject->PrefabFiles[i], dest, std::filesystem::copy_options::overwrite_existing);
        }
        for (int i = 0; i < m_OpenedProject->SceneFiles.size(); i++) {
            std::filesystem::copy(m_OpenedProject->SceneFiles[i], dest, std::filesystem::copy_options::overwrite_existing);
        }
        for (int i = 0; i < m_OpenedProject->ConfigFiles.size(); i++) {
            std::filesystem::copy(m_OpenedProject->ConfigFiles[i], dest, std::filesystem::copy_options::overwrite_existing);
        }

        std::filesystem::path exeDir = RUNTIME_EXE_DIR;

        std::filesystem::create_directory(buildDir / "mono");
        std::filesystem::copy(exeDir / "mono", buildDir / "mono", std::filesystem::copy_options::recursive | std::filesystem::copy_options::skip_existing);

        const char* toCopy[10] = {
            "Phezu-ScriptCore.dll",
            "mono-2.0-sgen.dll",
            "mono-2.0-sgen.lib",
            "Runtime.exe",
            "SDL2.dll",
            "SDL2.lib",
            "SDL2_image.dll",
            "SDL2_image.lib",
            "SDL2_ttf.dll",
            "SDL2_ttf.lib"
        };

        for (int i = 0; i < 10; i++) {
            std::filesystem::copy(exeDir / toCopy[i], buildDir, std::filesystem::copy_options::skip_existing);
        }
    }

    void EditorShell::CloseProject() {
        if (m_OpenedProject == nullptr)
            return;

        delete m_OpenedProject;
        m_OpenedProject = nullptr;
    }

    void EditorShell::GetFilesPathInDirectory(const std::filesystem::path & directory) {
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
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
        }
    }

    void EditorShell::GetFilesPathRecursively(const std::filesystem::path& directory) {
        GetFilesPathInDirectory(directory);

        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_directory()) {
                GetFilesPathRecursively(entry.path());
            }
        }
    }

    void EditorShell::Destroy() {
        CloseProject();
    }
}