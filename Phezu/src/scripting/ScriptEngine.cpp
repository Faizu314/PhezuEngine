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

    void MonoLog(const char* log_domain, const char* log_level, const char* message, mono_bool fatal, void* userData) {
        Log("Domain : %s\n", log_domain);
        Log("Level  : %s\n", log_level);
        Log("Message: %s\n", message);
        Log("Fatal  : %s\n\n", fatal ? "true" : "false");
    }

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

    MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
    {
        MonoImage* image = mono_assembly_get_image(assembly);
        MonoClass* klass = mono_class_from_name(image, namespaceName, className);

        if (klass == nullptr)
        {
            // Log error here
            return nullptr;
        }

        return klass;
    }

    ScriptEngine::ScriptEngine(Engine* engine) : m_Engine(engine), m_RootDomain(nullptr), m_AppDomain(nullptr) {}

    void ScriptEngine::Init() {
        std::filesystem::path monoCoreAssembliesPath = m_Engine->GetExePath() / "mono" / "lib" / "4.5";

        m_MonoLogger.Start();

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

        //printing all types to test

        MonoImage* image = mono_assembly_get_image(assembly);
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

        for (int32_t i = 0; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

            Log("%s.%s\n", nameSpace, name);
        }


        // Get a reference to the class we want to instantiate
        MonoClass* testingClass = GetClassInAssembly(assembly, "MonoTest", "CSharpTesting");

        // Allocate an instance of our class
        MonoObject* classInstance = mono_object_new(m_AppDomain, testingClass);

        if (classInstance == nullptr)
        {
            // Log error here and abort
        }

        // Call the parameterless (default) constructor
        mono_runtime_object_init(classInstance);




        // Get a reference to the method in the class
        MonoMethod* method = mono_class_get_method_from_name(testingClass, "IncrementFloat", 1);

        if (method == nullptr)
        {
            Log("No method called IncrementFloat with 1 parameters in the class, log error or something\n");
            return;
        }

        // Call the C# method on the objectInstance instance, and get any potential exceptions
        MonoObject* exception = nullptr;
        int testInt = 1;
        void* params[] =
        {
            &testInt
        };

        mono_runtime_invoke(method, classInstance, params, &exception);

        // TODO: Handle the exception
    }

    void ScriptEngine::Shutdown() {
        m_MonoLogger.Stop();
    }
}