#pragma once

#include "node.h"

#include <regex>

#define UYAML_IMPL_CONVERTER(t, toFn)                       \
    template<typename C>                                    \
    struct converter<C, t> {                                \
        static t to(const Node<C> &node, t def) {           \
            auto type = node.Type();                        \
            auto value = node.ValueRaw();                   \
            toFn                                            \
        }                                                   \
        static Node<C> from(t val) { return Node<C>(val); } \
    }


#define UYAML_IMPL_CONVERTER_TRY_DEF(t, vt) UYAML_IMPL_CONVERTER(t, {  \
    vt r;                                                              \
    return converter_helper<C>::try_convert(type, value, r) ? r : def; \
})

#define UYAML_IMPL_STR_CONVERTER(t, pt)                  \
    template<typename C>                                 \
    struct str_converter<C, t> {                         \
        static bool convert(Node<C> *node) {             \
            auto s = node->asString();                   \
            if (s.empty())                               \
                return false;                            \
            t r;                                         \
            if (converter_helper<C>::parse_##pt(s, r)) { \
                node->set(r);                            \
                return true;                             \
            }                                            \
            return false;                                \
        }                                                \
    };

#define UYAML_IMPL_TOSTR_NUM_CONVERTER(t)      \
    template<typename C>                       \
    struct tostr_converter<C, t> {             \
        static str<C> toStr(t v) {             \
            std::string s = std::to_string(v); \
            str<C> r;                          \
            for (auto c: s)                    \
                r += c;                        \
            return r;                          \
        }                                      \
    };

#define UYAML_IMPL_CONVERTER_INT(t) UYAML_IMPL_CONVERTER_TRY_DEF(t, int64_t)
#define UYAML_IMPL_CONVERTER_FLOAT(t) UYAML_IMPL_CONVERTER_TRY_DEF(t, double)

namespace UYAML {

    template<typename C>
    struct converter_helper;

    template<typename C, typename T>
    struct converter {
    };

    template<typename C, typename T>
    struct str_converter {
    };

    template<typename C, typename T>
    struct tostr_converter {
    };

    template<typename C, typename K>
    struct getter {
    };

    template<typename C, typename V>
    struct setter {
        static void set(Node<C> *node, V val) {
            node->template set<V>(val);
        }
    };

    template<typename C, typename T>
    struct as_if_convert {
        explicit as_if_convert(const Node<C> &node_) : node(node_) {}

        const Node<C> &node;

        static bool convert(Node<C> *node) {
            return str_converter<C, T>::convert(node);
        }

        static str<C> toStr(T val) {
            return tostr_converter<C, T>::toStr(val);
        }

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
        static bool parse_bool(const str<C> &s, bool &out) {
            static std::regex trueReg("true|on|yes");
            static std::regex falseReg("false|off|no");

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
            return true;
        }

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
            static std::regex reg("^[+-]?([0-9]+)?(\\.[0-9]+)?([eE][+-]?[0-9]+)?$");

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
            if (cs.empty() || !std::regex_match(cs, reg))
                return false;
            out = std::stod(cs);
            return true;
        }

        static bool try_convert(ValueType t, const Value<C> *v, bool &out) {
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
                    return parse_bool(s, out);
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
                    out = static_cast<double>(v->i);
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


    template<typename C, typename K>
    struct as_if_get {
        explicit as_if_get(Node<C> &node_) : node(node_) {}

        Node<C> &node;

        std::shared_ptr<Node<C>> operator[](K key) const {
            return getter<C, K>::get(node, key);
        }
    };

    template<typename C>
    struct getter<C, const C *> {
        static std::shared_ptr<Node<C>> get(Node<C> &node, const C *key) {
            return node.get(key);
        }
    };


    template<typename C>
    struct getter<C, const str<C> &> {
        static std::shared_ptr<Node<C>> get(Node<C> &node, const str<C> &key) {
            return node.get(key);
        }
    };

    template<typename C>
    struct getter<C, int> {
        static std::shared_ptr<Node<C>> get(Node<C> &node, int index) {
            return node.get(index);
        }
    };


    template<typename C, typename V>
    struct as_if_set {
        explicit as_if_set(Node<C> *node_) : node(node_) {}

        Node<C> *node;

        void operator()(V val) const {
            setter<C, V>::set(node, val);
        }
    };

    UYAML_IMPL_CONVERTER_TRY_DEF(bool, bool);

    UYAML_IMPL_CONVERTER_INT(int8_t);
    UYAML_IMPL_CONVERTER_INT(uint8_t);
    UYAML_IMPL_CONVERTER_INT(int16_t);
    UYAML_IMPL_CONVERTER_INT(uint16_t);
    UYAML_IMPL_CONVERTER_INT(int32_t);
    UYAML_IMPL_CONVERTER_INT(uint32_t);
    UYAML_IMPL_CONVERTER_INT(int64_t);

    UYAML_IMPL_CONVERTER_FLOAT(float);
    UYAML_IMPL_CONVERTER_FLOAT(double);

    UYAML_IMPL_CONVERTER(str<C>, {
        if (type == ValueType::String)
            return value->s;
        return def;
    });

    UYAML_IMPL_STR_CONVERTER(bool, bool)
    UYAML_IMPL_STR_CONVERTER(int8_t, int)
    UYAML_IMPL_STR_CONVERTER(uint8_t, int)
    UYAML_IMPL_STR_CONVERTER(int16_t, int)
    UYAML_IMPL_STR_CONVERTER(uint16_t, int)
    UYAML_IMPL_STR_CONVERTER(int32_t, int)
    UYAML_IMPL_STR_CONVERTER(uint32_t, int)
    UYAML_IMPL_STR_CONVERTER(int64_t, int)
    UYAML_IMPL_STR_CONVERTER(float, float)
    UYAML_IMPL_STR_CONVERTER(double, float)

    template<typename C>
    struct tostr_converter<C, bool> {
        static str<C> toStr(bool v) {
            static std::string s[2] = {"true", "false"};
            str<C> r;
            int i = v ? 1 : 0;
            for (auto c: s[i])
                r += c;
            return r;
        }
    };

    UYAML_IMPL_TOSTR_NUM_CONVERTER(int64_t)
    UYAML_IMPL_TOSTR_NUM_CONVERTER(float)
    UYAML_IMPL_TOSTR_NUM_CONVERTER(double)
}// namespace UYAML
