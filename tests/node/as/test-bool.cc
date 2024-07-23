#include "as.h"

using namespace std;
using namespace UYAML;


#define ASSERT_BOOL(node, tv) ASSERT_EQ(node.as<bool>(!tv), tv)
#define ASSERT_BOOL_DEF(node) ASSERT_EQ(node.as<bool>(true), true); ASSERT_EQ(node.as<bool>(false), false)

#define TEST_AS_BOOL(p, ct, msg, s, s1, s2, tgt) TEST_AS(bool,p, ct, msg, s, s1, s2, tgt,ASSERT_BOOL,ASSERT_BOOL_DEF)
#define TEST_AS_BOOL_DEF(p, ct, msg, s) TEST_AS_DEF(bool,p,ct,msg,s,ASSERT_BOOL_DEF)

//
// char
//

#define TEST_C_AS_BOOL(msg, s, s1, s2, tgt) TEST_AS_BOOL(C,char,msg, s, s1, s2, tgt)
#define TEST_C_AS_BOOL_DEF(msg, s) TEST_AS_BOOL_DEF(C,char,msg, s)

TEST_C_AS_BOOL(true, "true", "1true", "true1", true)

TEST_C_AS_BOOL(false, "false", "1false", "false1", false)

TEST_C_AS_BOOL(on, "on", "1on", "on1", true)

TEST_C_AS_BOOL(off, "off", "1off", "off1", false)

TEST_C_AS_BOOL(yes, "yes", "1yes", "yes1", true)

TEST_C_AS_BOOL(no, "no", "1no", "no1", false)

TEST_C_AS_BOOL_DEF(bad, "?")

TEST_C_AS_BOOL_DEF(bad0, "0")

TEST_C_AS_BOOL_DEF(bad1, "1")


//
// wchar_t
//

#define TEST_W_AS_BOOL(msg, s, s1, s2, tgt) TEST_AS_BOOL(W,wchar_t ,msg, s, s1, s2, tgt)
#define TEST_W_AS_BOOL_DEF(msg, s) TEST_AS_BOOL_DEF(W,wchar_t ,msg, s)

TEST_W_AS_BOOL(true, L"true", L"1true", L"true1", true)

TEST_W_AS_BOOL(false, L"false", L"1false", L"false1", false)

TEST_W_AS_BOOL(on, L"on", L"1on", L"on1", true)

TEST_W_AS_BOOL(off, L"off", L"1off", L"off1", false)

TEST_W_AS_BOOL(yes, L"yes", L"1yes", L"yes1", true)

TEST_W_AS_BOOL(no, L"no", L"1no", L"no1", false)

TEST_W_AS_BOOL_DEF(bad, L"?")

TEST_W_AS_BOOL_DEF(bad0, L"0")

TEST_W_AS_BOOL_DEF(bad1, L"1")


//
// char8_t
//

#define TEST_U8_AS_BOOL(msg, s, s1, s2, tgt) TEST_AS_BOOL(U8,char8_t ,msg, s, s1, s2, tgt)
#define TEST_U8_AS_BOOL_DEF(msg, s) TEST_AS_BOOL_DEF(U8,char8_t ,msg, s)

TEST_U8_AS_BOOL(true, u8"true", u8"1true", u8"true1", true)

TEST_U8_AS_BOOL(false, u8"false", u8"1false", u8"false1", false)

TEST_U8_AS_BOOL(on, u8"on", u8"1on", u8"on1", true)

TEST_U8_AS_BOOL(off, u8"off", u8"1off", u8"off1", false)

TEST_U8_AS_BOOL(yes, u8"yes", u8"1yes", u8"yes1", true)

TEST_U8_AS_BOOL(no, u8"no", u8"1no", u8"no1", false)

TEST_U8_AS_BOOL_DEF(bad, u8"?")

TEST_U8_AS_BOOL_DEF(bad0, u8"0")

TEST_U8_AS_BOOL_DEF(bad1, u8"1")


//
// char16_t
//

#define TEST_U16_AS_BOOL(msg, s, s1, s2, tgt) TEST_AS_BOOL(U16,char16_t ,msg, s, s1, s2, tgt)
#define TEST_U16_AS_BOOL_DEF(msg, s) TEST_AS_BOOL_DEF(U16,char16_t ,msg, s)

TEST_U16_AS_BOOL(true, u"true", u"1true", u"true1", true)

TEST_U16_AS_BOOL(false, u"false", u"1false", u"false1", false)

TEST_U16_AS_BOOL(on, u"on", u"1on", u"on1", true)

TEST_U16_AS_BOOL(off, u"off", u"1off", u"off1", false)

TEST_U16_AS_BOOL(yes, u"yes", u"1yes", u"yes1", true)

TEST_U16_AS_BOOL(no, u"no", u"1no", u"no1", false)

TEST_U16_AS_BOOL_DEF(bad, u"?")

TEST_U16_AS_BOOL_DEF(bad0, u"0")

TEST_U16_AS_BOOL_DEF(bad1, u"1")



//
// char32_t
//

#define TEST_U32_AS_BOOL(msg, s, s1, s2, tgt) TEST_AS_BOOL(U32,char32_t ,msg, s, s1, s2, tgt)
#define TEST_U32_AS_BOOL_DEF(msg, s) TEST_AS_BOOL_DEF(U32,char32_t ,msg, s)

TEST_U32_AS_BOOL(true, U"true", U"1true", U"true1", true)

TEST_U32_AS_BOOL(false, U"false", U"1false", U"false1", false)

TEST_U32_AS_BOOL(on, U"on", U"1on", U"on1", true)

TEST_U32_AS_BOOL(off, U"off", U"1off", U"off1", false)

TEST_U32_AS_BOOL(yes, U"yes", U"1yes", U"yes1", true)

TEST_U32_AS_BOOL(no, U"no", U"1no", U"no1", false)

TEST_U32_AS_BOOL_DEF(bad, U"?")

TEST_U32_AS_BOOL_DEF(bad0, U"0")

TEST_U32_AS_BOOL_DEF(bad1, U"1")
