project(ScriptCore)

if (APPLE)
    set(CS_BUILD_COMMAND csc)
elseif (WIN32)
    set(MONO_EXE "${MONO_DIR}/bin/mono.exe")
    set(CS_COMPILER "${MONO_DIR}/lib/mono/4.5/csc.exe")
    set(CS_BUILD_COMMAND ${MONO_EXE} ${CS_COMPILER})
endif()

file(GLOB_RECURSE SCRIPT_CORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cs")

add_custom_target(ScriptCore
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SCRIPT_CORE_DLL_SRC_DIR}"
    COMMAND ${CS_BUILD_COMMAND}
        -target:library
        -out:${SCRIPT_CORE_DLL_SRC_DIR}/Phezu-ScriptCore.dll
        ${SCRIPT_CORE_SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)