cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

if (NOT DEFINED PROCESSOR_COUNT)
    set(PROCESSOR_COUNT 1)

    if (WIN32)
        set(PROCESSOR_COUNT "$ENV{NUMBER_OF_PROCESSORS}")
        message(STATUS "Cores Found: ${PROCESSOR_COUNT}")
    endif()
endif()

project(Tetris)

file(GLOB_RECURSE PHEZU_SOURCES "${CMAKE_SOURCE_DIR}/Phezu/src/*.cpp")
file(GLOB_RECURSE PHEZU_HEADERS "${CMAKE_SOURCE_DIR}/Phezu/include/phezu/*.hpp")

file(GLOB_RECURSE PHEZU_VENDOR_SOURCES "${CMAKE_SOURCE_DIR}/Phezu/vendor/*.cpp")

source_group("Header Files" FILES ${PHEZU_HEADERS})
source_group("Source Files" FILES ${PHEZU_SOURCES})
source_group("Vendor\\Source Files" FILES ${PHEZU_VENDOR_SOURCES})

add_library(Phezu STATIC ${PHEZU_SOURCES} ${PHEZU_VENDOR_SOURCES} ${PHEZU_HEADERS})
set_target_properties(Phezu PROPERTIES OUTPUT_NAME Phezu PREFIX "")


file(GLOB_RECURSE GAME_SOURCES "${CMAKE_SOURCE_DIR}/Game/src/*.cpp")
file(GLOB_RECURSE GAME_HEADERS "${CMAKE_SOURCE_DIR}/Game/include/*.hpp")

source_group("Header Files" FILES ${GAME_HEADERS})
source_group("Source Files" FILES ${GAME_SOURCES})

if (APPLE)
    add_executable(Game ${GAME_SOURCES} ${GAME_HEADERS})

    set(LIB_SRC_PATH "${CMAKE_SOURCE_DIR}/Phezu/lib/mac")
    set(LIB_DEST_PATH "${CMAKE_CFG_INTDIR}/Game.app/Contents/Frameworks")

    set(CMAKE_OSX_DEPLOYMENT_TARGET "12.2")

    set_target_properties(Game PROPERTIES
        INSTALL_RPATH "@loader_path/../Frameworks"
        BUILD_WITH_INSTALL_RPATH TRUE
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/Info.plist"
    )

    target_link_libraries(Phezu PRIVATE
        "-F${LIB_SRC_PATH}"
        "-framework SDL2"
        "-framework SDL2_image"
        "-framework SDL2_ttf"
    )

    target_include_directories(Phezu PRIVATE Phezu/include/mac)
elseif (WIN32)
    add_executable(Game WIN32 ${GAME_SOURCES} ${GAME_HEADERS})

    math(EXPR BUILD_CORES "${PROCESSOR_COUNT} * 3 / 4")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP${BUILD_CORES}")

    set(LIB_SRC_PATH "${CMAKE_SOURCE_DIR}/Phezu/lib/windows")
    set(LIB_DEST_PATH "$<TARGET_FILE_DIR:Game>")

    target_link_libraries(Phezu PRIVATE
        "${LIB_SRC_PATH}/SDL2.lib"
        "${LIB_SRC_PATH}/SDL2_image.lib"
        "${LIB_SRC_PATH}/SDL2_ttf.lib"
    )

    target_include_directories(Phezu PRIVATE Phezu/include/windows)
endif()

target_include_directories(Phezu PUBLIC Phezu/include/phezu)
target_include_directories(Phezu PUBLIC Phezu/include/common)
target_include_directories(Game PUBLIC Game/include)
target_link_libraries(Game PRIVATE Phezu)

# copy and paste the libraries to the executable directory
add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${LIB_DEST_PATH}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${LIB_SRC_PATH}" "${LIB_DEST_PATH}"
)
