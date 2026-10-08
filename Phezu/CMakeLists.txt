project(Phezu)

file(GLOB_RECURSE PHEZU_SOURCES "${CMAKE_SOURCE_DIR}/Phezu/Source/*.cpp")
file(GLOB_RECURSE PHEZU_HEADERS "${CMAKE_SOURCE_DIR}/Phezu/Include/*.hpp")

file(GLOB_RECURSE PHEZU_VENDOR_SOURCES
    "${CMAKE_SOURCE_DIR}/Phezu/Vendor/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/Phezu/Vendor/src/*.c"
)
file(GLOB_RECURSE PHEZU_VENDOR_HEADERS
    "${CMAKE_SOURCE_DIR}/Phezu/Vendor/include/*.hpp"
    "${CMAKE_SOURCE_DIR}/Phezu/Vendor/include/*.h"
)

set(PHEZU_ALL_SOURCES
    ${PHEZU_SOURCES}
    ${PHEZU_VENDOR_SOURCES}
)

set(PHEZU_ALL_HEADERS
    ${PHEZU_HEADERS}
    ${PHEZU_VENDOR_HEADERS}
)

if (PZ_PLATFORM_WINDOWS)
    list(FILTER PHEZU_ALL_SOURCES EXCLUDE REGEX ".*/[Ww]eb/.*")
    list(FILTER PHEZU_ALL_HEADERS EXCLUDE REGEX ".*/[Ww]eb/.*")
elseif (PZ_PLATFORM_WEB)
    list(FILTER PHEZU_ALL_SOURCES EXCLUDE REGEX ".*/[Ww]in32/.*")
    list(FILTER PHEZU_ALL_HEADERS EXCLUDE REGEX ".*/[Ww]in32/.*")

    list(FILTER PHEZU_ALL_SOURCES EXCLUDE REGEX ".*/Scripting/.*")
    list(FILTER PHEZU_ALL_HEADERS EXCLUDE REGEX ".*/Scripting/.*")

    list(APPEND PHEZU_ALL_SOURCES "${CMAKE_SOURCE_DIR}/Phezu/Source/Scripting/Systems/ScriptEngine.cpp")
    list(APPEND PHEZU_ALL_HEADERS "${CMAKE_SOURCE_DIR}/Phezu/Include/Scripting/Systems/ScriptEngine.hpp")
endif()

add_library(Phezu STATIC ${PHEZU_ALL_SOURCES} ${PHEZU_ALL_HEADERS})
set_target_properties(Phezu PROPERTIES OUTPUT_NAME Phezu PREFIX "")

target_compile_definitions(Phezu PUBLIC
    $<$<CONFIG:Debug>:PZ_DEBUG>
    $<$<CONFIG:Release>:PZ_RELEASE>
)

if (PZ_PLATFORM_WINDOWS)
    target_compile_definitions(Phezu PUBLIC PZ_PLATFORM_WINDOWS)
elseif (PZ_PLATFORM_WEB)
    target_compile_definitions(Phezu PUBLIC PZ_PLATFORM_WEB)
endif()

target_include_directories(Phezu PUBLIC ${CMAKE_SOURCE_DIR}/Phezu/Include)
target_include_directories(Phezu PUBLIC ${CMAKE_SOURCE_DIR}/Phezu/Vendor/include/common)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PHEZU_ALL_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PHEZU_ALL_HEADERS})


if (PZ_PLATFORM_WINDOWS)
    target_include_directories(Phezu PRIVATE 
        ${CMAKE_SOURCE_DIR}/Phezu/Vendor/include/win32
        ${MONO_DIR}/include/mono-2.0
    )
    target_link_libraries(Phezu PRIVATE 
        opengl32 
        winmm
        "${MONO_DIR}/lib/mono-2.0-sgen.lib"
    )
elseif (PZ_PLATFORM_WEB)
    target_link_libraries(Phezu PRIVATE 

    )
endif()
