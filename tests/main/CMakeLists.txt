project(main CXX)

aux_source_directory("." SRCS)

add_executable(main
        main.cpp
)
