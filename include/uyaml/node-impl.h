#pragma once

#include "node.h"

#include <regex>

#define IMPL_CONVERTER(c, t, toFn) template<> struct converter<c, t>: private converter_helper<c>{ \
static t to(const Node<c> &node, t def) {auto type=node.Type();auto value = node.ValueRaw();toFn}\
static Node<c> from(t val){return Node<c>(val);}}

namespace UYAML {

    template<typename C>
    struct converter_helper;

    template<typename C, typename T>
    struct converter {

    };

    template<typename C, typename T>
    struct as_if {
        explicit as_if(const Node<C> &node_) : node(node_) {}

        const Node<C> &node;

        T operator[](T def) const {
            return converter<C, T>::to(node, def);
        }

        Node<C> operator()(T val) {
            return converter<C, T>::from(val);
        }
    };

    template<typename C>
    struct converter_helper {
    private:
        /**
         * @brief 转换成小写（只会转换字母）
         * @param s 原始
         * @param out 结果（会被清空）
         * @return 是否全部转换成小写，如果含有非字母会返回false
         */
        static bool wordToLower(const str<C> &s, str<C> &out) {
            out.clear();
            for (auto &c: s) {
                if (c >= 'a' && c <= 'z') {
                    out += c;
                    continue;
                }
                if (c >= 'A' && c <= 'Z') {
                    out += c + 'a' - 'A';
                }
                return false;
            }
            return true;
        }

        static bool to_ascii_cstr(const str<C> &s, str<char> &out) {
            out.clear();
            for (auto c: s) {
                if (c < 0 || c > 127)
                    return false;
                out += c;
            }
            return true;
        }

    public:

        static bool parse_int(const str<C> &s, int64_t &out) {
            if (s.empty())
                return false;
            if (s.length() > 21)
                return false;
            bool neg = false;
            int i = 1;
            if (s[0] == '+') {
                neg = false;
            } else if (s[0] == '-') {
                neg = true;
            } else {
                i = 0;
            }
            out = 0;
            for (; i < s.length(); i++) {
                auto c = s[i];
                if (c <= '0' || c > '9')
                    return false;
                out = out * 10 + c - '0';
            }
            if (neg)
                out = -out;
            return true;
        }

        static bool parse_float(const str<C> &s, double &out) {
            static std::regex reg("^[+-]?[0-9]+(\\.[0-9]+)?([eE][+-]?[0-9]+)?$");

            out = 0;
            str<char> cs;
            for (auto c: s) {
                if ((c >= '0' && c <= '9') ||
                    c == '.' ||
                    c == '-' || c == '+' ||
                    c == 'e' || c == 'E') {
                    cs += c;
                } else
                    return false;
            }
            if (!std::regex_match(cs, reg))
                return false;
            out = std::stod(cs);
            return true;
        }

        static bool try_convert(ValueType t, const Value<C> *v, bool &out) {
            static std::regex trueReg("true|on|yes");
            static std::regex falseReg("false|off|no");

            switch (t) {
                case Bool:
                    out = v->b;
                    break;
                case Int:
                    out = v->i != 0;
                    break;
                case Float:
                    out = v->f != 0;
                    break;
                case String: {
                    auto s = v->s;
                    if (s.length() < 2 || s.length() > 5)
                        return false;
                    str<C> r;
                    if (!wordToLower(s, r))
                        return false;
                    str<char> ascii;
                    if (!to_ascii_cstr(r, ascii))
                        return false;
                    if (std::regex_match(ascii, trueReg))
                        out = true;
                    else if (std::regex_match(ascii, falseReg))
                        out = false;
                    else
                        return false;
                    break;
                }
                default:
                    return false;
            }
            return true;
        }

        static bool try_convert(ValueType t, const Value<C> *v, int64_t &out) {
            switch (t) {
                case Int:
                    out = v->i;
                    break;
                case Float:
                    out = static_cast<int64_t>(v->f);
                    break;
                case String:
                    return parse_int(v->s, out);
                default:
                    return false;
            }
            return true;
        }

        static bool try_convert(ValueType t, const Value<C> *v, double &out) {
            switch (t) {
                case Int:
                    out = static_cast<double >(v->i);
                    break;
                case Float:
                    out = v->f;
                    break;
                case String:
                    return parse_float(v->s, out);
                default:
                    return false;
            }
            return true;
        }
    };

}

#define IMPL_CONVERTER_STR(c) IMPL_CONVERTER(c, str<c>, {\
if (type == ValueType::String)\
    return value->s;\
return def;\
})

#define IMPL_CONVERTER_TRY_DEF(c, t, vt) IMPL_CONVERTER(c, t, {\
    vt r;\
    if (try_convert(type, value, r))\
        return r;\
    return def;\
})

#define IMPL_CONVERTER_INT(c, t) IMPL_CONVERTER_TRY_DEF(c,t,int64_t)
#define IMPL_CONVERTER_FLOAT(c, t) IMPL_CONVERTER_TRY_DEF(c,t,double)

#define IMPL_CONVERTER_INTS(c) \
IMPL_CONVERTER_INT(c, int8_t);\
IMPL_CONVERTER_INT(c, uint8_t);\
IMPL_CONVERTER_INT(c, int16_t);\
IMPL_CONVERTER_INT(c, uint16_t);\
IMPL_CONVERTER_INT(c, int32_t);\
IMPL_CONVERTER_INT(c, uint32_t);\
IMPL_CONVERTER_INT(c, int64_t)

#define IMPL_CONVERTER_FLOATS(c) \
IMPL_CONVERTER_FLOAT(c, float);\
IMPL_CONVERTER_FLOAT(c, double)