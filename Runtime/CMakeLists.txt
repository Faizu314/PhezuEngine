project(Runtime)

file(GLOB_RECURSE RUNTIME_SOURCES "${CMAKE_SOURCE_DIR}/Runtime/src/*.cpp")
file(GLOB_RECURSE RUNTIME_HEADERS "${CMAKE_SOURCE_DIR}/Runtime/include/*.hpp")

if (APPLE)
    add_executable(Runtime ${RUNTIME_SOURCES} ${RUNTIME_HEADERS})
    
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
    add_executable(Runtime WIN32 ${RUNTIME_SOURCES} ${RUNTIME_HEADERS})
endif()

target_include_directories(Runtime PUBLIC ${CMAKE_SOURCE_DIR}/Runtime/include)
target_link_libraries(Runtime PRIVATE Phezu)

add_custom_command(TARGET Runtime POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${PROJ_DEST_DIR}"
    
    COMMAND ${CMAKE_COMMAND} -E make_directory "${LIB_DEST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJ_DEST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MONO_CORE_LIBS_DEST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SCRIPT_CORE_DLL_DEST_DIR}"

    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SRC_DIR}" "${PROJ_DEST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${MONO_CORE_LIBS_SRC_DIR}" "${MONO_CORE_LIBS_DEST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SCRIPT_CORE_DLL_SRC_DIR}/Phezu-ScriptCore.dll" "${SCRIPT_CORE_DLL_DEST_DIR}/Phezu-ScriptCore.dll"
)

if(APPLE)
    add_custom_command(TARGET Runtime POST_BUILD

        COMMAND rsync -a --links "${LIB_SRC_DIR}/" "${LIB_DEST_DIR}"
    )
elseif(WIN32)
    add_custom_command(TARGET Runtime POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${LIB_SRC_DIR}" "${LIB_DEST_DIR}"
    )
endif()

add_dependencies(Runtime ScriptCore)
