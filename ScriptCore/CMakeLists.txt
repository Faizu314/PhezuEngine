project(ScriptCore)

find_program(DOTNET_EXE dotnet REQUIRED)

add_custom_target(Phezu-ScriptCore ALL
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}

    COMMAND ${DOTNET_EXE} build ScriptCore.csproj
            -c "Release"
            -o ${CMAKE_BINARY_DIR}/ScriptCore/Build/bin
            -p:BaseIntermediateOutputPath=${CMAKE_BINARY_DIR}/ScriptCore/Build/obj/
)

add_custom_command(TARGET Phezu-ScriptCore POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SCRIPT_CORE_DLL_PATH}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        "${CMAKE_BINARY_DIR}/ScriptCore/Build/bin/Phezu-ScriptCore.dll"
        "${SCRIPT_CORE_DLL_PATH}/Phezu-ScriptCore.dll"
)

add_dependencies(Phezu-ScriptCore Runtime)