file(WRITE "${OUT_FILE}" "#pragma once\n\n")
file(APPEND "${OUT_FILE}" "#define SCRIPT_CORE_DLL_RELATIVE_PATH \"${SCRIPT_PATH}\"\n")
file(APPEND "${OUT_FILE}" "#define MONO_CORE_LIBS_RELATIVE_PATH \"${MONO_PATH}\"\n")
