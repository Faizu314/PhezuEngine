project(Phezu)

file(GLOB_RECURSE PHEZU_SOURCES "${CMAKE_SOURCE_DIR}/Phezu/Source/*.cpp")
file(GLOB_RECURSE PHEZU_HEADERS "${CMAKE_SOURCE_DIR}/Phezu/Include/*.hpp")
file(GLOB_RECURSE PHEZU_VENDOR_SOURCES "${CMAKE_SOURCE_DIR}/Phezu/Vendor/src/*.cpp")

set(PHEZU_ALL_SOURCES
    ${PHEZU_SOURCES}
    ${PHEZU_VENDOR_SOURCES}
)

set(PHEZU_ALL_HEADERS
    ${PHEZU_HEADERS}
)

if (WIN32)
    list(FILTER PHEZU_ALL_SOURCES EXCLUDE REGEX ".*/Mac/.*")
    list(FILTER PHEZU_ALL_HEADERS EXCLUDE REGEX ".*/Mac/.*")
elseif(APPLE)
    list(FILTER PHEZU_ALL_SOURCES EXCLUDE REGEX ".*/Win32/.*")
    list(FILTER PHEZU_ALL_HEADERS EXCLUDE REGEX ".*/Win32/.*")
endif()

add_library(Phezu STATIC ${PHEZU_ALL_SOURCES} ${PHEZU_ALL_HEADERS})
set_target_properties(Phezu PROPERTIES OUTPUT_NAME Phezu PREFIX "")

if (WIN32)
    target_compile_definitions(Phezu PRIVATE NOMINMAX)
endif()

target_include_directories(Phezu PUBLIC ${CMAKE_SOURCE_DIR}/Phezu/Include)
target_include_directories(Phezu PUBLIC ${CMAKE_SOURCE_DIR}/Phezu/Vendor/include/common)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PHEZU_ALL_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PHEZU_ALL_HEADERS})


if (APPLE)
    target_include_directories(Phezu PRIVATE ${CMAKE_SOURCE_DIR}/Phezu/Vendor/include/mac)
    target_link_libraries(Phezu PRIVATE
        "-F${LIB_SRC_DIR}"
        "-framework Mono"
    )
elseif (WIN32)
    target_include_directories(Phezu PRIVATE ${CMAKE_SOURCE_DIR}/Phezu/Vendor/include/windows)
    target_link_libraries(Phezu PRIVATE
        "${LIB_SRC_DIR}/mono-2.0-sgen.lib"
        opengl32
    )
endif()
