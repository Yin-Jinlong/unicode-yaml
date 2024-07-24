
set(CMAKE_CXX_STANDARD 23)

include(GoogleTest)

include_directories(
        "."
        "../include"
)

function(test NAME)
    set(SRCS)
    foreach (FILE ${ARGN})
        list(APPEND SRCS ${FILE})
    endforeach ()

    add_executable(${NAME}
            ${SRCS}
    )

    target_link_libraries(${NAME}
            $<TARGET_LINKER_FILE:unicode_yaml_static>
            GTest::gtest_main
    )

    gtest_discover_tests(${NAME})

endfunction()

function(test_dir NAME)
    set(SRCS)
    foreach (FILE ${ARGN})
        list(APPEND SRCS ${FILE})
    endforeach ()
    test(${PROJECT_NAME}_${NAME} ${SRCS})
endfunction()

add_subdirectory(main)
add_subdirectory(node)
