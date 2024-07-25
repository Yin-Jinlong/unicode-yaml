cmake_minimum_required(VERSION 3.28)
project(unicode_yaml CXX)

set(CMAKE_CXX_STANDARD 23)

option(ENABLE_TEST "" OFF)

include(cmake/msvc.cmake)

set_msvc_utf_8()

include_directories("include")

aux_source_directory("src" SRCS)

add_library(unicode_yaml SHARED
        ${SRCS}
)

target_compile_definitions(unicode_yaml
        PUBLIC -DUYAML_EXPORT
)

add_library(unicode_yaml_static STATIC
        ${SRCS}
)

target_compile_definitions(unicode_yaml_static
        PUBLIC -DUYAML_USE_STATIC
)

install(TARGETS unicode_yaml_static DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(DIRECTORY include/uyaml DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

if (ENABLE_TEST)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.15.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    add_subdirectory(tests)
endif ()
