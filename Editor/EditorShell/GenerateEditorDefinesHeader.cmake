file(WRITE "${OUT_FILE}" "#pragma once\n\n")
file(APPEND "${OUT_FILE}" "#define CSHARP_BUILD_COMMAND \"${CSHARP_BUILD_COMMAND}\"\n")
file(APPEND "${OUT_FILE}" "#define PROJECT_SRC_DIR \"${PROJECT_SRC_DIR}\"\n")