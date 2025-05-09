project(ScriptCore)

find_program(DOTNET_EXE dotnet REQUIRED)

set(SCRIPTCORE_CONFIG "Release")
set(SCRIPTCORE_OUTPUT_DIR "${CMAKE_BINARY_DIR}/ScriptCore/Build")

add_custom_target(ScriptCore ALL
    COMMAND ${DOTNET_EXE} build ScriptCore.csproj
            -c ${SCRIPTCORE_CONFIG}
            -o ${SCRIPTCORE_OUTPUT_DIR}/bin
            -p:BaseIntermediateOutputPath=${SCRIPTCORE_OUTPUT_DIR}/obj/
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building ScriptCore (.NET) project..."
)

add_custom_command(TARGET ScriptCore POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        "${CMAKE_BINARY_DIR}/ScriptCore/Build/bin/Phezu-ScriptCore.dll"
        "${LIB_DEST_PATH}/Phezu-ScriptCore.dll"
)

add_dependencies(ScriptCore Runtime)