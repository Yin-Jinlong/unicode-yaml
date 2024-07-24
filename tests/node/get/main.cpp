#include "test.h"

TEST_HEAD


T(Node, get_list) {
    vector<WNode *> list;
    list.push_back(new WNode(L"a"));
    list.push_back(new WNode(L"b"));
    WNode node(list);
    auto v1 = node[0];
    auto v2 = node[1];
    ASSERT_EQ(v1->as<str<wchar_t>>(L""), L"a");
    ASSERT_EQ(v2->as<str<wchar_t>>(L""), L"b");
}

T(Node, get_obj) {
    WNode node;
    auto a = node[L"a"];
    ASSERT_EQ(a->Type(), ValueType::Null);
}
