project(Runtime)

file(GLOB_RECURSE RUNTIME_SOURCES "${CMAKE_SOURCE_DIR}/Runtime/src/*.cpp")
file(GLOB_RECURSE RUNTIME_HEADERS "${CMAKE_SOURCE_DIR}/Runtime/include/*.hpp")


if (APPLE)
    add_executable(Runtime ${RUNTIME_SOURCES} ${RUNTIME_HEADERS})

    set(CMAKE_OSX_DEPLOYMENT_TARGET "12.2")

    set_target_properties(Runtime PROPERTIES
        INSTALL_RPATH "@loader_path/../Frameworks"
        BUILD_WITH_INSTALL_RPATH TRUE
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/Info.plist"
    )
elseif (WIN32)
    add_executable(Runtime WIN32 ${RUNTIME_SOURCES} ${RUNTIME_HEADERS})

    math(EXPR BUILD_CORES "${PROCESSOR_COUNT} * 3 / 4")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP${BUILD_CORES}")
endif()


target_include_directories(Runtime PUBLIC ${CMAKE_SOURCE_DIR}/Runtime/include)
target_link_libraries(Runtime PRIVATE Phezu)

add_custom_command(TARGET Runtime POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${LIB_DEST_PATH}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${LIB_SRC_PATH}" "${LIB_DEST_PATH}"
)
