#include "as.h"

using namespace std;
using namespace UYAML;


#define ASSERT_INT(t, node, tv) ASSERT_EQ(node.as<t>(0), tv)
#define ASSERT_INT_DEF(t, node) ASSERT_EQ(node.as<t>(0), 0); ASSERT_EQ(node.as<t>(1), 1)

#define TEST_AS_INT(p, ct, t, msg, s, tgt) TEST_STR_AS1_T(t,p, ct, msg, s, tgt,ASSERT_INT)
#define TEST_AS_INT_DEF(t, p, ct, msg, s) TEST_STR_AS_DEF_T(t,p,ct,msg,s,ASSERT_INT_DEF)

#define TEST_C_AS_INT(t, msg, s, tgt) TEST_AS_INT(C,char,t,msg,s,tgt)
#define TEST_C_AS_INT_DEF(t, msg, s) TEST_AS_INT_DEF(t,C, char, msg, s)

TEST_C_AS_INT(int64_t, 11, "11", 11)

TEST_C_AS_INT(int64_t, 22, "+22", 22)

TEST_C_AS_INT(int64_t, neg33, "-33", -33)

TEST_C_AS_INT_DEF(int64_t, bad1, "a")

TEST_C_AS_INT_DEF(int64_t, bad2, "1234567890987654321")

TEST_ALL