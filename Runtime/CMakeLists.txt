project(Runtime)

file(GLOB_RECURSE RUNTIME_SOURCES "${CMAKE_SOURCE_DIR}/Runtime/Source/*.cpp")
file(GLOB_RECURSE RUNTIME_HEADERS "${CMAKE_SOURCE_DIR}/Runtime/Include/*.hpp")

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
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SCRIPT_CORE_DLL_DEST_DIR}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${LIB_DEST_DIR}"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SCRIPT_CORE_DLL_SRC_DIR}/Phezu-ScriptCore.dll" "${SCRIPT_CORE_DLL_DEST_DIR}/Phezu-ScriptCore.dll"

    COMMAND ${CMAKE_COMMAND} 
        -DSRC_DIR="${MONO_CORE_LIBS_SRC_DIR}" 
        -DCHECK_DIR="${MONO_CORE_LIBS_DEST_DIR}/mono"
        -DDEST_DIR="${MONO_CORE_LIBS_DEST_DIR}" 
        -P "${CMAKE_CURRENT_SOURCE_DIR}/CopyIfMissing.cmake"
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
