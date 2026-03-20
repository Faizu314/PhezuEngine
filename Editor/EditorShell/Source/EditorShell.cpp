#include "EditorShell.hpp"
#include "Project.hpp"
#include "EditorDefines.hpp"

#include <fstream>
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
            std::ofstream compilerBat("CS_COMPILER.bat");

            compilerBat << CSHARP_BUILD_COMMAND << " -target:library -out:\"" << buildDir.generic_string() << "/Game.dll\" ";

            for (int i = 0; i < m_OpenedProject->ScriptFiles.size(); i++) {
                compilerBat << "\"" << m_OpenedProject->ScriptFiles[i].generic_string() << "\" ";
            }

            compilerBat << "-reference:\"" << SCRIPT_CORE_DLL_SRC_DIR << "/Phezu-ScriptCore.dll\"";

            compilerBat.close();

            printf("Running generated bat file\n");

            std::system("CS_COMPILER.bat");
        }

        printf("Copy pasting asset files\n");

        std::filesystem::path dest = buildDir / "Assets";

        for (int i = 0; i < m_OpenedProject->GetFilesCount(); i++) {
            std::filesystem::copy(m_OpenedProject->GetFilePathByIndex(i), dest, std::filesystem::copy_options::overwrite_existing);
        }

        printf("Copy pasting mono core libs\n");

        std::filesystem::path exeDir = RUNTIME_EXE_DIR;

        std::filesystem::create_directory(buildDir / "mono");
        std::filesystem::copy(exeDir / "mono", buildDir / "mono", std::filesystem::copy_options::recursive | std::filesystem::copy_options::skip_existing);

        const char* toCopy[] = {
            "Phezu-ScriptCore.dll",
            "mono-2.0-sgen.dll",
#if defined PZ_DEBUG
            "mono-2.0-sgen.pdb",
#endif
            "Runtime.exe",
        };

        size_t toCopySize = sizeof(toCopy) / sizeof(toCopy[0]);

        printf("Copy pasting exe, dlls and libs\n");

        for (int i = 0; i < toCopySize; i++) {
            std::filesystem::path filepath = exeDir / toCopy[i];
            std::filesystem::copy(filepath, buildDir, std::filesystem::copy_options::skip_existing);
        }
    }

    void EditorShell::CloseProject() {
        if (m_OpenedProject == nullptr)
            return;

        delete m_OpenedProject;
        m_OpenedProject = nullptr;
    }

    void EditorShell::AddFilePath(const std::filesystem::path& extension, const std::filesystem::path& path) {
        if (extension == ".cs") {
            m_OpenedProject->ScriptFiles.push_back(path);
        }
        else if (extension == ".scene") {
            m_OpenedProject->SceneFiles.push_back(path);
        }
        else if (extension == ".prefab") {
            m_OpenedProject->PrefabFiles.push_back(path);
        }
        else if (extension == ".shader") {
            m_OpenedProject->ShaderFiles.push_back(path);
        }
        else if (extension == ".mat") {
            m_OpenedProject->MaterialFiles.push_back(path);
        }
        else if (extension == ".mesh") {
            m_OpenedProject->MeshFiles.push_back(path);
        }
        else if (extension == ".png") {
            m_OpenedProject->ImageFiles.push_back(path);
        }
        else if (extension == ".texture") {
            m_OpenedProject->PrefabFiles.push_back(path);
        }
        else if (extension == ".config") {
            m_OpenedProject->ConfigFiles.push_back(path);
        }
    }

    void EditorShell::GetFilesPathInDirectory(const std::filesystem::path & directory) {
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                if (entry.path().extension() == ".meta") {
                    std::filesystem::path stem = entry.path().stem();
                    AddFilePath(stem.extension(), entry);
                }
                else {
                    AddFilePath(entry.path().extension(), entry);
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