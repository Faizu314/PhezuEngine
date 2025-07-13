project(ScriptCore)

set(CSC_EXE "${CMAKE_SOURCE_DIR}/Vendor/Mac/mono/bin/csc")

file(GLOB_RECURSE SCRIPT_CORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cs")

set(SCRIPT_CORE_OUTPUT_DIR "${CMAKE_BINARY_DIR}/ScriptCore/Build/bin")

add_custom_target(ScriptCore ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SCRIPT_CORE_OUTPUT_DIR}"
    COMMAND ${CSC_EXE}
        -target:library
        -out:${SCRIPT_CORE_OUTPUT_DIR}/Phezu-ScriptCore.dll
        ${SCRIPT_CORE_SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_command(TARGET ScriptCore POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SCRIPT_CORE_DLL_PATH}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        "${SCRIPT_CORE_OUTPUT_DIR}/Phezu-ScriptCore.dll"
        "${SCRIPT_CORE_DLL_PATH}/Phezu-ScriptCore.dll"
)

add_dependencies(ScriptCore Runtime)