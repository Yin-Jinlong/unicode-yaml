#include "test.h"

using namespace std;
using namespace UYAML;

#define ASSERT_TYPE(n, t) EXPECT_EQ(n.Type(), ValueType::t)

#define TEST_NEW(p, t, v) T(Node,p##_new_##t){ASSERT_TYPE(p##Node(v), t);}

// char

TEST_NEW(C, Null,)

TEST_NEW(C, Bool, true)

TEST_NEW(C, Int, 123)

TEST_NEW(C, Float, 12.34)

TEST_NEW(C, String, "text")

TEST_NEW(C, List, CNodeList())

TEST_NEW(C, Object, CNodeMap())

// wchar_t

TEST_NEW(W, Null,)

TEST_NEW(W, Bool, true)

TEST_NEW(W, Int, 123)

TEST_NEW(W, Float, 12.34)

TEST_NEW(W, String, L"text")

TEST_NEW(W, List, WNodeList())

TEST_NEW(W, Object, WNodeMap())

// char8_t

TEST_NEW(U8, Null,)

TEST_NEW(U8, Bool, true)

TEST_NEW(U8, Int, 123)

TEST_NEW(U8, Float, 12.34)

TEST_NEW(U8, String, u8"text")

TEST_NEW(U8, List, U8NodeList())

TEST_NEW(U8, Object, U8NodeMap())

// char16_t

TEST_NEW(U16, Null,)

TEST_NEW(U16, Bool, true)

TEST_NEW(U16, Int, 123)

TEST_NEW(U16, Float, 12.34)

TEST_NEW(U16, String, u"text")

TEST_NEW(U16, List, U16NodeList())

TEST_NEW(U16, Object, U16NodeMap())

// char32_t

TEST_NEW(U32, Null,)

TEST_NEW(U32, Bool, true)

TEST_NEW(U32, Int, 123)

TEST_NEW(U32, Float, 12.34)

TEST_NEW(U32, String, U"text")

TEST_NEW(U32, List, U32NodeList())

TEST_NEW(U32, Object, U32NodeMap())

TEST_ALL
