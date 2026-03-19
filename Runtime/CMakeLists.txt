project(Runtime)

if (APPLE)
    set(LIBS_DEST_DIR "$<TARGET_FILE_DIR:Runtime>/../Frameworks")
    set(MONO_CORE_LIBS_DEST_DIR "$<TARGET_FILE_DIR:Runtime>/../MonoCoreLibs")
    set(SCRIPT_CORE_DLL_DEST_DIR "$<TARGET_FILE_DIR:Runtime>/${SCRIPT_CORE_DLL_RELATIVE_PATH}")
elseif (WIN32)
    set(LIBS_DEST_DIR "$<TARGET_FILE_DIR:Runtime>")
    set(MONO_CORE_LIBS_DEST_DIR "${LIBS_DEST_DIR}")
    set(SCRIPT_CORE_DLL_DEST_DIR "${LIBS_DEST_DIR}")
endif()

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
    COMMAND ${CMAKE_COMMAND} -E make_directory "${LIBS_DEST_DIR}"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SCRIPT_CORE_DLL_SRC_DIR}/Phezu-ScriptCore.dll" "${SCRIPT_CORE_DLL_DEST_DIR}/Phezu-ScriptCore.dll"

    COMMAND ${CMAKE_COMMAND} 
        -DSRC_DIR="${MONO_DIR}/lib/mono/4.5" 
        -DCHECK_DIR="${MONO_CORE_LIBS_DEST_DIR}/mono"
        -DDEST_DIR="${MONO_CORE_LIBS_DEST_DIR}/mono/lib/mono" 
        -P "${CMAKE_CURRENT_SOURCE_DIR}/CopyIfMissing.cmake"
)

if(APPLE)
    add_custom_command(TARGET Runtime POST_BUILD
        #copy libs from source to build directory
    )
elseif(WIN32)
    add_custom_command(TARGET Runtime POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${MONO_DIR}/bin/mono-2.0-sgen.dll" "${LIBS_DEST_DIR}/mono-2.0-sgen.dll"
        COMMAND ${CMAKE_COMMAND} -E copy "${MONO_DIR}/lib/mono-2.0-sgen.pdb" "${LIBS_DEST_DIR}/mono-2.0-sgen.pdb"
    )
endif()

add_dependencies(Runtime ScriptCore)
