#include "test.h"

using namespace std;
using namespace UYAML;

WNode node;
int main(int argc, char **argv) {

    node.Parse(L"b: true\ni: 123\nf: 1.23\ns: hi");

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#define TEST_CONVERT(t, vt, k, kv)          \
    T(Node, convert_##t) {                  \
        auto &v = *node[L#k];               \
        bool r = v.convertNode<t>();        \
        ASSERT_EQ(r, true);                 \
        ASSERT_EQ(v.Type(), ValueType::vt); \
        ASSERT_EQ(v.ValueRaw()->k, kv);     \
    }

TEST_CONVERT(bool, Bool, b, true)
TEST_CONVERT(int64_t, Int, i, 123)

T(Node, convert_double) {
    auto &v = *node[L"f"];
    bool r = v.convertNode<double>();
    ASSERT_EQ(r, true);
    ASSERT_EQ(v.Type(), ValueType::Float);
    ASSERT_NEAR(v.ValueRaw()->f, 1.23, 1e-5);
}
