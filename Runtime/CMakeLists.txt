project(Runtime)

if (PZ_PLATFORM_WINDOWS)
    set(LIBS_DEST_DIR "$<TARGET_FILE_DIR:Runtime>")
    set(MONO_CORE_LIBS_DEST_DIR "${LIBS_DEST_DIR}")
    set(SCRIPT_CORE_DLL_DEST_DIR "${LIBS_DEST_DIR}")
endif()

file(GLOB_RECURSE RUNTIME_SOURCES "${CMAKE_SOURCE_DIR}/Runtime/Source/*.cpp")
file(GLOB_RECURSE RUNTIME_HEADERS "${CMAKE_SOURCE_DIR}/Runtime/Include/*.hpp")

if (PZ_PLATFORM_WINDOWS)
    add_executable(Runtime WIN32 ${RUNTIME_SOURCES} ${RUNTIME_HEADERS})
elseif (PZ_PLATFORM_WEB)
    add_executable(Runtime ${RUNTIME_SOURCES} ${RUNTIME_HEADERS})
    set_target_properties(Runtime PROPERTIES SUFFIX ".html")
endif()

target_include_directories(Runtime PUBLIC ${CMAKE_SOURCE_DIR}/Runtime/Include)
target_link_libraries(Runtime PRIVATE Phezu)

if (PZ_PLATFORM_WINDOWS)
    add_custom_command(TARGET Runtime POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${SCRIPT_CORE_DLL_DEST_DIR}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${LIBS_DEST_DIR}"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SCRIPT_CORE_DLL_SRC_DIR}/Phezu-ScriptCore.dll" "${SCRIPT_CORE_DLL_DEST_DIR}/Phezu-ScriptCore.dll"

        COMMAND ${CMAKE_COMMAND} 
            -DSRC_DIR="${MONO_DIR}/lib/mono/4.5" 
            -DCHECK_DIR="${MONO_CORE_LIBS_DEST_DIR}/mono"
            -DDEST_DIR="${MONO_CORE_LIBS_DEST_DIR}/mono/lib/mono" 
            -P "${CMAKE_CURRENT_SOURCE_DIR}/CopyIfMissing.cmake"

        COMMAND ${CMAKE_COMMAND} -E copy "${MONO_DIR}/bin/mono-2.0-sgen.dll" "${LIBS_DEST_DIR}/mono-2.0-sgen.dll"
        COMMAND ${CMAKE_COMMAND} -E copy "${MONO_DIR}/lib/mono-2.0-sgen.pdb" "${LIBS_DEST_DIR}/mono-2.0-sgen.pdb"
    )
endif()

if (NOT PZ_PLATFORM_WEB)
    add_dependencies(Runtime ScriptCore)
endif()