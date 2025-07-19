project(Runtime)

file(GLOB_RECURSE RUNTIME_SOURCES "${CMAKE_SOURCE_DIR}/Runtime/src/*.cpp")
file(GLOB_RECURSE RUNTIME_HEADERS "${CMAKE_SOURCE_DIR}/Runtime/include/*.hpp")

set(RUNTIME_DEFINES_HEADER "${CMAKE_BINARY_DIR}/Runtime/Generated/RuntimeDefines.hpp")

if (APPLE)
    add_executable(Runtime ${RUNTIME_SOURCES} ${RUNTIME_HEADERS} "${RUNTIME_DEFINES_HEADER}")
    
    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "")
    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO")
    
    set(CMAKE_OSX_DEPLOYMENT_TARGET "12.2")
    
    set_target_properties(Runtime PROPERTIES
        INSTALL_RPATH "@loader_path/../Frameworks"
        BUILD_WITH_INSTALL_RPATH TRUE
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
    )
elseif (WIN32)
    add_executable(Runtime WIN32 ${RUNTIME_SOURCES} ${RUNTIME_HEADERS} "${RUNTIME_DEFINES_HEADER}")
endif()


# Generate Defines Header

add_custom_command(
    OUTPUT "${RUNTIME_DEFINES_HEADER}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/Runtime/Generated"
    COMMAND ${CMAKE_COMMAND} -E echo_append "#pragma once\n" > "${RUNTIME_DEFINES_HEADER}"
    COMMAND ${CMAKE_COMMAND} -E echo_append "#define SCRIPT_CORE_DLL_RELATIVE_PATH \"${SCRIPT_CORE_DLL_RELATIVE_PATH}\"\n" >> "${RUNTIME_DEFINES_HEADER}"
    COMMAND ${CMAKE_COMMAND} -E echo_append "#define MONO_CORE_LIBS_RELATIVE_PATH \"${MONO_CORE_LIBS_RELATIVE_PATH}\"\n" >> "${RUNTIME_DEFINES_HEADER}"
    VERBATIM
)

# Generate Defines Header


target_include_directories(Runtime PUBLIC ${CMAKE_SOURCE_DIR}/Runtime/include)
target_include_directories(Runtime PUBLIC ${CMAKE_BINARY_DIR}/Runtime/Generated)
target_link_libraries(Runtime PRIVATE Phezu)

add_custom_command(TARGET Runtime POST_BUILD
    DEPENDS "${RUNTIME_DEFINES_HEADER}"

    COMMAND ${CMAKE_COMMAND} -E make_directory "${LIB_DEST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJ_DEST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MONO_CORE_LIBS_DEST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SCRIPT_CORE_DLL_DEST_DIR}"

    COMMAND rsync -a --links "${LIB_SRC_DIR}/" "${LIB_DEST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SRC_DIR}" "${PROJ_DEST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${MONO_CORE_LIBS_SRC_DIR}" "${MONO_CORE_LIBS_DEST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SCRIPT_CORE_DLL_SRC_DIR}/Phezu-ScriptCore.dll" "${SCRIPT_CORE_DLL_DEST_DIR}/Phezu-ScriptCore.dll"
)

add_dependencies(Runtime ScriptCore)
