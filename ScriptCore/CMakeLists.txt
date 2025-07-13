project(ScriptCore)

set(CSC_EXE "${CMAKE_SOURCE_DIR}/Vendor/Mac/mono/bin/csc")

file(GLOB_RECURSE SCRIPT_CORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cs")

add_custom_target(ScriptCore ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SCRIPT_CORE_DLL_SRC_DIR}"
    COMMAND ${CSC_EXE}
        -target:library
        -out:${SCRIPT_CORE_DLL_SRC_DIR}/Phezu-ScriptCore.dll
        ${SCRIPT_CORE_SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)