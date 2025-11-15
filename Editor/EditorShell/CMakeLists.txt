project(EditorShell)

file(GLOB_RECURSE SHELL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")
file(GLOB_RECURSE SHELL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.hpp")

if (WIN32)
    add_executable(EditorShell ${SHELL_SOURCES} ${SHELL_HEADERS})
endif()

target_include_directories(EditorShell PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)
target_include_directories(EditorShell PUBLIC ${CMAKE_BINARY_DIR}/Editor/Generated)

set(EDITOR_DEFINES_HEADER "${CMAKE_BINARY_DIR}/Editor/Generated/EditorDefines.hpp")

add_custom_command(
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/Editor/Generated"
        OUTPUT "${EDITOR_DEFINES_HEADER}"
        COMMAND ${CMAKE_COMMAND} -DOUT_FILE=${EDITOR_DEFINES_HEADER}
        -DCSHARP_BUILD_COMMAND=${CSHARP_BUILD_COMMAND}
        -DPROJECT_SRC_DIR=${PROJECT_SRC_DIR}
        -P "${CMAKE_SOURCE_DIR}/Editor/EditorShell/GenerateEditorDefinesHeader.cmake"
        VERBATIM
)

add_custom_target(EditorDefinesHeader
        DEPENDS "${EDITOR_DEFINES_HEADER}"
)

add_dependencies(EditorShell EditorDefinesHeader)