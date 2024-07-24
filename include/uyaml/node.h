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

        explicit Node(bool b) : Node(ValueType::Bool) {
            value = new Value<C>(b);
        }

#define CAST_CONSTRUCTOR(type) explicit Node(type i) : Node(static_cast<int64_t>(i)) {}

        CAST_CONSTRUCTOR(int8_t)

        CAST_CONSTRUCTOR(uint8_t)

        CAST_CONSTRUCTOR(int16_t)

        CAST_CONSTRUCTOR(uint16_t)

        CAST_CONSTRUCTOR(int32_t)

        CAST_CONSTRUCTOR(uint32_t)

#undef CAST_CONSTRUCTOR

        explicit Node(int64_t i) : Node(ValueType::Int) {
            value = new Value<C>(i);
        }

        explicit Node(float f) : Node(static_cast<double >(f)) {
        }

        explicit Node(double f) : Node(ValueType::Float) {
            value = new Value<C>(f);
        }

        explicit Node(const str<C> &s) : Node(ValueType::String) {
            value = new Value<C>(s);
        }

        explicit Node(const C *s) : Node(ValueType::String) {
            value = new Value<C>(s);
        }

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
                    return Node<C>();
                case Bool:
                    return Node<C>(value->b);
                case Int:
                    return Node<C>(value->i);
                case Float:
                    return Node<C>(value->f);
                case String:
                    return Node<C>(value->s);
                case List:
                    return Node<C>(value->list);
                case Object:
                    return Node<C>(value->obj);
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
