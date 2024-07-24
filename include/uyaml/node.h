#pragma once

#include <stdexcept>
#include "def.h"

namespace UYAML {

    template<typename C, typename T>
    struct as_if;

    template<typename C>
    class UYAML_API Node {
    private:
        ValueType type;
        Value<C> *value;
    protected:
        explicit Node(ValueType t) {
            type = t;
        }

    public:
        explicit Node() : Node(ValueType::Null) {
            value = new Value<C>();
        }

#define CONSTRUCTOR(t, T)  explicit Node(t v) : Node(ValueType::T) { value = new Value<C>(v); }
#define CONSTRUCTOR_CAST(t, T)  explicit Node(t v) :  Node(static_cast<T>(v)){}

        CONSTRUCTOR(bool, Bool)

        CONSTRUCTOR_CAST(int8_t, int64_t)

        CONSTRUCTOR_CAST(uint8_t, int64_t)

        CONSTRUCTOR_CAST(int16_t, int64_t)

        CONSTRUCTOR_CAST(uint16_t, int64_t)

        CONSTRUCTOR_CAST(int32_t, int64_t)

        CONSTRUCTOR_CAST(uint32_t, int64_t)

        CONSTRUCTOR(int64_t, Int)

        CONSTRUCTOR_CAST(float, double)

        CONSTRUCTOR(double, Float)

        CONSTRUCTOR(const str<C> &, String)

        CONSTRUCTOR(const C*, String)

#undef CONSTRUCTOR
#undef CONSTRUCTOR_CAST

        explicit Node(const std::vector<Node<C> *> list) {
            type = ValueType::List;
            value = new Value<C>(std::vector<Node<C> *>());
            for (auto &i: list) {
                if (i)
                    value->list.push_back(i);
            }
            value->list.shrink_to_fit();
        }

        explicit Node(const std::map<str<C>, Node<C> *> &obj) {
            type = ValueType::Object;
            value = new Value<C>(std::map<str<C>, Node<C> *>());
            for (auto &i: obj) {
                if (i.second)
                    value->obj[i.first] = i.second;
            }
        }

        template<typename T>
        explicit Node(T val):Node(as_if<C, T>(val)) {

        };

        Node(const Node<C> &node) = default;

        Node(const Node<C> &&node) noexcept {
            type = node.type;
            value = node.value;
        };

        Node(Node<C> &&node) = default;

        ~Node() {
            if (type == ValueType::List) {
                for (auto &i: value->list) {
                    delete i;
                }
            } else if (type == ValueType::Object) {
                for (auto &i: value->obj) {
                    delete i.second;
                }
            }
            delete value;
        }

        Node<C> clone() {
            switch (type) {
                case Null:
                    return Node();
                case Bool:
                    return Node(value->b);
                case Int:
                    return Node(value->i);
                case Float:
                    return Node(value->f);
                case String:
                    return Node(value->s);
                case List:
                    return Node(value->list);
                case Object:
                    return Node(value->obj);
                default:
                    throw std::runtime_error("unknown type");
            }
        }

        UYAML_USE_RET ValueType Type() const {
            return type;
        }

        const Value<C> *ValueRaw() const {
            return value;
        }

        bool IsNull() {
            return type == ValueType::Null;
        }

        std::vector<Node<C> *> asList() const noexcept {
            return type == ValueType::List ? value->list : std::vector<Node<C> *>();
        }

        std::map<str<C>, Node<C> *> asMap() const noexcept {
            return type == ValueType::Object ? value->obj : std::map<str<C>, Node<C> *>();
        }

        /**
         * @brief 转换到值，不能转换则为默认值
         * @tparam T 目标类型
         * @return 结果
         */
        template<typename T>
        T as(T def) const noexcept {
            return as_if<C, T>(*this)[def];
        }
    };
}
