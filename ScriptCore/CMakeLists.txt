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

