#include "as.h"

using namespace std;
using namespace UYAML;


#define ASSERT_FLOAT(t, node, tv) ASSERT_NEAR(node.as<t>(0), tv,1e-5)
#define ASSERT_FLOAT_DEF(t, node) ASSERT_NEAR(node.as<t>(0), 0,1e-5); ASSERT_NEAR(node.as<t>(1), 1,1e-5)

#define TEST_AS_FLOAT(p, ct, t, msg, s, tgt) TEST_STR_AS1_T(t,p, ct, msg, s, tgt,ASSERT_FLOAT)
#define TEST_AS_FLOAT_DEF(t, p, ct, msg, s) TEST_STR_AS_DEF_T(t,p,ct,msg,s,ASSERT_FLOAT_DEF)

#define TEST_C_AS_FLOAT(t, msg, s, tgt) TEST_AS_FLOAT(C,char,t,msg,s,tgt)
#define TEST_C_AS_FLOAT_DEF(t, msg, s) TEST_AS_FLOAT_DEF(t,C, char, msg, s)

TEST_C_AS_FLOAT(double, double, "1.0", 1.0)

TEST_C_AS_FLOAT(double, doublee, "1e5", 1e5)

TEST_C_AS_FLOAT(double, double_e, "1.1234e5", 1.1234e5)

TEST_C_AS_FLOAT(double, double_e1, "+1.1234e5", +1.1234e5)

TEST_C_AS_FLOAT(double, negdouble_e, "-1.1234e5", -1.1234e5)

TEST_C_AS_FLOAT_DEF(double, double_def1, "1.2.3")

TEST_C_AS_FLOAT_DEF(double, double_def2, "1.e2e4")

TEST_ALL