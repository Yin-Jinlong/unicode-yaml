project(node CXX)

test_dir(get get/main.cpp)

test_dir(new
        new/main.cpp
)

set(STR_AS_TYPES
        bool
        int
        float
)

foreach (T ${STR_AS_TYPES})
    test_dir(str_as_${T} as/string-${T}.cc)
endforeach ()

