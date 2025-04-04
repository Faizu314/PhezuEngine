#include "scripting/ScriptEngine.hpp"

#include "Engine.hpp"
#include "Logger.hpp"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/debug-helpers.h"
#include "mono/metadata/mono-config.h"
#include "mono/utils/mono-logger.h"

#include <fstream>

namespace Phezu {

    void MonoLog(const char* logDomain, const char* logLevel, const char* message, mono_bool fatal, void* userData);

    char* ReadBytes(const std::string& filepath, uint32_t* outSize)
    {
        std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

        if (!stream)
        {
            // Failed to open the file
            return nullptr;
        }

        std::streampos end = stream.tellg();
        stream.seekg(0, std::ios::beg);
        size_t size = end - stream.tellg();

        if (size == 0)
        {
            // File is empty
            return nullptr;
        }

        char* buffer = new char[size];
        stream.read((char*)buffer, size);
        stream.close();

        *outSize = static_cast<uint32_t>(size);
        return buffer;
    }

    MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
    {
        uint32_t fileSize = 0;
        char* fileData = ReadBytes(assemblyPath, &fileSize);

        // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
        MonoImageOpenStatus status;
        MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

        if (status != MONO_IMAGE_OK)
        {
            const char* errorMessage = mono_image_strerror(status);
            // Log some error message using the errorMessage data
            return nullptr;
        }

        MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
        mono_image_close(image);

        // Don't forget to free the file data
        delete[] fileData;

        return assembly;
    }

    ScriptEngine::ScriptEngine(Engine* engine) : m_Engine(engine), m_RootDomain(nullptr), m_AppDomain(nullptr) {}

    void ScriptEngine::Init() {
        std::filesystem::path monoCoreAssembliesPath = m_Engine->GetExePath() / "mono" / "lib" / "4.5";

        mono_trace_set_level_string("debug");
        mono_trace_set_log_handler(MonoLog, nullptr);
        mono_set_assemblies_path(monoCoreAssembliesPath.string().c_str());

        MonoDomain* rootDomain = mono_jit_init("MyScriptRuntime");
        if (rootDomain == nullptr)
        {
            // TODO: Logging
            return;
        }

        m_RootDomain = rootDomain;

        m_AppDomain = mono_domain_create_appdomain("MyAppDomain", nullptr);
        mono_domain_set(m_AppDomain, true);

        MonoAssembly* assembly = LoadCSharpAssembly("E:/Users/faiza/Visual Studio Projects/MonoTest/MonoTest/bin/Debug/net6.0/MonoTest.dll");
    }

    void MonoLog(const char* log_domain, const char* log_level, const char* message, mono_bool fatal, void* userData) {
        Log("Domain : %s\n", log_domain);
        Log("Level  : %s\n", log_level);
        Log("Message: %s\n", message);
        Log("Fatal  : %s\n\n", fatal ? "true" : "false");
    }
}