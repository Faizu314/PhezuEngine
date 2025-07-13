project(Phezu)

file(GLOB PHEZU_SOURCES "${CMAKE_SOURCE_DIR}/Phezu/src/*.cpp")
file(GLOB PHEZU_ASSET_SOURCES "${CMAKE_SOURCE_DIR}/Phezu/src/AssetManagement/*.cpp")
file(GLOB PHEZU_MATH_SOURCES "${CMAKE_SOURCE_DIR}/Phezu/src/maths/*.cpp")
file(GLOB PHEZU_SCENE_SOURCES "${CMAKE_SOURCE_DIR}/Phezu/src/scene/*.cpp")
file(GLOB PHEZU_COMP_SOURCES "${CMAKE_SOURCE_DIR}/Phezu/src/scene/components/*.cpp")
file(GLOB PHEZU_SERIALIZE_SOURCES "${CMAKE_SOURCE_DIR}/Phezu/src/serialization/*.cpp")
file(GLOB PHEZU_SCRIPTING_SOURCES "${CMAKE_SOURCE_DIR}/Phezu/src/scripting/*.cpp")

file(GLOB PHEZU_HEADERS "${CMAKE_SOURCE_DIR}/Phezu/include/phezu/*.hpp")
file(GLOB PHEZU_ASSET_HEADERS "${CMAKE_SOURCE_DIR}/Phezu/include/phezu/AssetManagement/*.hpp")
file(GLOB PHEZU_MATH_HEADERS "${CMAKE_SOURCE_DIR}/Phezu/include/phezu/maths/*.hpp")
file(GLOB PHEZU_SCENE_HEADERS "${CMAKE_SOURCE_DIR}/Phezu/include/phezu/scene/*.hpp")
file(GLOB PHEZU_COMP_HEADERS "${CMAKE_SOURCE_DIR}/Phezu/include/phezu/scene/components/*.hpp")
file(GLOB PHEZU_SERIALIZE_HEADERS "${CMAKE_SOURCE_DIR}/Phezu/include/phezu/serialization/*.hpp")
file(GLOB PHEZU_SCRIPTING_HEADERS "${CMAKE_SOURCE_DIR}/Phezu/include/phezu/scripting/*.hpp")

file(GLOB_RECURSE PHEZU_VENDOR_SOURCES "${CMAKE_SOURCE_DIR}/Phezu/vendor/*.cpp")

set(PHEZU_ALL_SOURCES
    ${PHEZU_SOURCES}
    ${PHEZU_ASSET_SOURCES}
    ${PHEZU_MATH_SOURCES}
    ${PHEZU_SCENE_SOURCES}
    ${PHEZU_COMP_SOURCES}
    ${PHEZU_SERIALIZE_SOURCES}
    ${PHEZU_SCRIPTING_SOURCES}
    ${PHEZU_VENDOR_SOURCES}
)

set(PHEZU_ALL_HEADERS
    ${PHEZU_HEADERS}
    ${PHEZU_ASSET_HEADERS}
    ${PHEZU_MATH_HEADERS}
    ${PHEZU_SCENE_HEADERS}
    ${PHEZU_SERIALIZE_HEADERS}
    ${PHEZU_SCRIPTING_HEADERS}
    ${PHEZU_COMP_HEADERS}
)


add_library(Phezu STATIC ${PHEZU_ALL_SOURCES} ${PHEZU_ALL_HEADERS})
set_target_properties(Phezu PROPERTIES OUTPUT_NAME Phezu PREFIX "")

target_include_directories(Phezu PUBLIC ${CMAKE_SOURCE_DIR}/Phezu/include/phezu)
target_include_directories(Phezu PUBLIC ${CMAKE_SOURCE_DIR}/Phezu/include/common)

source_group("Header Files" FILES ${PHEZU_HEADERS})
source_group("Header Files\\AssetManagement" FILES ${PHEZU_ASSET_HEADERS})
source_group("Header Files\\Math" FILES ${PHEZU_MATH_HEADERS})
source_group("Header Files\\Scene" FILES ${PHEZU_SCENE_HEADERS})
source_group("Header Files\\Scene\\Components" FILES ${PHEZU_COMP_HEADERS})
source_group("Header Files\\Serialization" FILES ${PHEZU_SERIALIZE_HEADERS})
source_group("Header Files\\Scripting" FILES ${PHEZU_SCRIPTING_HEADERS})

source_group("Source Files" FILES ${PHEZU_SOURCES})
source_group("Source Files\\AssetManagement" FILES ${PHEZU_ASSET_SOURCES})
source_group("Source Files\\Math" FILES ${PHEZU_MATH_SOURCES})
source_group("Source Files\\Scene" FILES ${PHEZU_SCENE_SOURCES})
source_group("Source Files\\Scene\\Components" FILES ${PHEZU_COMP_SOURCES})
source_group("Source Files\\Serialization" FILES ${PHEZU_SERIALIZE_SOURCES})
source_group("Source Files\\Scripting" FILES ${PHEZU_SCRIPTING_SOURCES})


if (APPLE)
    target_include_directories(Phezu PRIVATE ${CMAKE_SOURCE_DIR}/Phezu/include/mac)
    target_link_libraries(Phezu PRIVATE
        "-F${LIB_SRC_DIR}"
        "-framework SDL2"
        "-framework SDL2_image"
        "-framework SDL2_ttf"
        "-framework Mono"
    )
elseif (WIN32)
    target_include_directories(Phezu PRIVATE ${CMAKE_SOURCE_DIR}/Phezu/include/windows)
    target_link_libraries(Phezu PRIVATE
        "${LIB_SRC_DIR}/SDL2.lib"
        "${LIB_SRC_DIR}/SDL2_image.lib"
        "${LIB_SRC_DIR}/SDL2_ttf.lib"
        "${LIB_SRC_DIR}/mono-2.0-sgen.lib"
    )
endif()
