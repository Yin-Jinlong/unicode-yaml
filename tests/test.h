#include <gtest/gtest.h>

#define UYAML_USE_STATIC

#include <uyaml/uyaml.h>

#define TEST_ALL int main(int argc, char **argv) {\
    ::testing::InitGoogleTest(&argc, argv);\
    return RUN_ALL_TESTS();\
}

#define T(name) TEST(NAME##Test,name)
