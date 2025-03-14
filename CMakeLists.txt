cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(PhezuEngine)

if (NOT DEFINED PROCESSOR_COUNT)
    set(PROCESSOR_COUNT 1)
    if (WIN32)
        set(PROCESSOR_COUNT "$ENV{NUMBER_OF_PROCESSORS}")
        message(STATUS "Cores Found: ${PROCESSOR_COUNT}")
    endif()
endif()

if (APPLE)
    set(LIB_SRC_PATH "${CMAKE_SOURCE_DIR}/Phezu/lib/mac" CACHE INTERNAL "")
    set(LIB_DEST_PATH "${CMAKE_CFG_INTDIR}/Runtime.app/Contents/Frameworks" CACHE INTERNAL "")
elseif (WIN32)
    set(LIB_SRC_PATH "${CMAKE_SOURCE_DIR}/Phezu/lib/windows" CACHE INTERNAL "")
    set(LIB_DEST_PATH "$<TARGET_FILE_DIR:Runtime>" CACHE INTERNAL "")
endif()

add_subdirectory(Phezu)
add_subdirectory(Runtime)
