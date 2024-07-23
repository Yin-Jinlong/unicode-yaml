#pragma once

#define NAME Node

#include "test.h"

#define TEST_AS(t, p, ct, msg, s, s1, s2, tgt, A, AD) T(p##_as_##t##_##msg){ \
     str<ct> v(s),v1(s1),v2(s2);\
     p##Node node(v),node1(v1),node2(v2);\
     A(node, tgt);\
     AD(node1);\
     AD(node2);\
}

#define TEST_AS_DEF(t, p, ct, msg, s, AD) T(p##_as_##t##_##msg){ \
     str<ct> v(s);\
     p##Node node(v);\
     AD(node);\
}
