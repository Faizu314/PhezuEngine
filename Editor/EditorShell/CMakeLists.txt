project(EditorShell)

file(GLOB_RECURSE SHELL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")
file(GLOB_RECURSE SHELL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.hpp")

if (WIN32)
    add_executable(EditorShell ${SHELL_SOURCES} ${SHELL_HEADERS})
endif()

target_include_directories(EditorShell PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)