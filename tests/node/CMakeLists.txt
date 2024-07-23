project(node CXX)

test_dir(new
        new/main.cpp
)

aux_source_directory(as AS_SRCS)

test_dir(as ${AS_SRCS})
