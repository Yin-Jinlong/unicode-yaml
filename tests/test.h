#include <gtest/gtest.h>

#define UYAML_USE_STATIC

#include <uyaml/uyaml.h>

#define TEST_ALL

#define T(tn, name) TEST(tn##Test,name)

#define TEST_HEAD using namespace std;\
using namespace UYAML;\
int main(int argc, char **argv) {\
    ::testing::InitGoogleTest(&argc, argv);\
    return RUN_ALL_TESTS();\
}