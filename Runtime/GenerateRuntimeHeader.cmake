add_custom_command(
    OUTPUT "${RUNTIME_DEFINES_HEADER}"
    COMMAND ${CMAKE_COMMAND} -DOUT_FILE="${RUNTIME_DEFINES_HEADER}"
                             -DSCRIPT_PATH="${SCRIPT_CORE_DLL_RELATIVE_PATH}"
                             -DMONO_PATH="${MONO_CORE_LIBS_RELATIVE_PATH}"
                             -P "${CMAKE_SOURCE_DIR}/CMake/GenerateRuntimeHeader.cmake"
    VERBATIM
)
