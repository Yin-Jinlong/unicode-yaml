#pragma once

#include "def.h"

namespace UYAML {

    template<typename C>
    union UYAML_API Value {
        char nul;
        bool b;
        int64_t i;
        double f;
        str<C> s;
        std::vector<Node<C> *> *list;
        std::map<str<C>, Node<C> *> *obj;

    public:
        Value() : nul(0) {}

        ~Value(){};

        explicit Value(bool b) : b(b) {}

        explicit Value(int64_t i) : i(i) {}

        explicit Value(double f) : f(f) {}

        explicit Value(const C *s) : s(s) {}

        explicit Value(const str<C> &s) : s(s) {}

        explicit Value(std::vector<Node<C> *> *list) : list(list) {}

        explicit Value(std::map<str<C>, Node<C> *> *obj) : obj(obj) {}
    };
}// namespace UYAML
