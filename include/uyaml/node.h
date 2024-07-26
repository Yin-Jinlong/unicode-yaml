#pragma once

#include "def.h"

#include <list>
#include <locale>

#include "parser.h"

namespace UYAML {

    template<typename C, typename T>
    struct as_if_convert;

    template<typename C, typename K>
    struct as_if_get;

    template<typename C, typename V>
    struct as_if_set;

    template<typename C>
    class UYAML_API Node {
    private:
        ValueType type;
        Value<C> *value;

        void cleanValue() {
            delete value;
            value = nullptr;
        }

    protected:
        explicit Node(ValueType t) {
            type = t;
        }

        static std::exception error_line(const char *msg, int line) {
            return std::runtime_error(std::format("{} as line {}", msg, line));
        }

        void convert2obj() {
            if (type != ValueType::Null)
                return;
            type = ValueType::Object;
            value = new Value<C>(new std::map<str<C>, std::shared_ptr<Node<C>>>());
        }

        void convert2list() {
            if (type != ValueType::Null)
                return;
            type = ValueType::List;
            value = new Value<C>(new std::vector<std::shared_ptr<Node<C>>>());
        }

        Node<C> *parse_value(std::list<str<C>> lines, int row) {
            auto node = new Node<C>();
            node->parse(lines, row);
            return node;
        }

        void parse(std::list<str<C>> lines, int row) {
            if (type != ValueType::Null && type != ValueType::Object)
                throw std::runtime_error("not Object node could not have named child node");
            if (type == ValueType::Null) {
                convert2obj();
            }
            while (lines.size()) {
                row++;
                auto line = lines.front();
                lines.pop_front();
                int pi = parser_trim_start(line) / 2;
                if (parser_is_comment_or_blank(line))// 空行
                    continue;
                if (pi)
                    throw error_line("bad indent", row);
                str<C> k, v;
                if (!parser_split_kv(line, k, v))
                    throw error_line("bad key-value", row);

                parser_trim(k);
                parser_trim(v);
                if (v.empty()) {
                    std::list<str<C>> sublines;
                    while (lines.size()) {
                        row++;
                        auto sline = lines.front();
                        pi = parser_trim_start(sline, 2);
                        if (parser_is_comment_or_blank(sline))
                            continue;
                        if (!pi)
                            break;
                        lines.pop_front();
                        if (pi != 2)
                            throw error_line("bad indent", row);
                        sublines.push_back(sline);
                    }
                    auto node = parse_value(sublines, row - 1);
                    value->obj->insert(std::pair(k, node));
                } else {
                    value->obj->insert(std::pair(k, new Node<C>(v)));
                }
            }
        }

        std::list<str<C>> dump() {
            std::list<str<C>> lines;
            std::string line;
            switch (type) {
                case Null:
                    break;
                case Bool:
                    line = value->b ? "true" : "false";
                    break;
                case Int:
                    line = std::to_string(value->i);
                    break;
                case Float:
                    line = std::to_string(value->f);
                    break;
                case String: {
                    auto s = value->s;
                    lines.push_back(str<C>(s.begin(), s.end()));
                    return lines;
                }
                case List:
                    throw std::runtime_error("not support list yes");
                case Object: {
                    auto obj = *value->obj;
                    for (auto pair: obj) {
                        str<C> keyline;
                        auto v = pair.second;
                        auto valueLines = v->dump();
                        keyline += pair.first;
                        keyline += ':';
                        keyline += ' ';
                        if (v->type != ValueType::Object) {
                            if (!valueLines.empty()) {
                                keyline += valueLines.front();
                                lines.push_back(keyline);
                            }
                        } else {
                            lines.push_back(keyline);
                            for (auto sline: valueLines) {
                                str<C> s;
                                s += ' ';
                                s += ' ';
                                s += sline;
                                lines.push_back(s);
                            }
                        }
                    }
                    return lines;
                }
                default:
                    return lines;
            }
            str<C> s;
            for (auto c: line) {
                s += c;
            }
            lines.push_back(s);
            return lines;
        }

    public:
        explicit Node() : Node(ValueType::Null) {
            value = new Value<C>();
        }

#define CONSTRUCTOR(t, T) \
    explicit Node(t v) : Node(ValueType::T) { value = new Value<C>(v); }
#define CONSTRUCTOR_CAST(t, T) \
    explicit Node(t v) : Node(static_cast<T>(v)) {}

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
        CONSTRUCTOR(const C *, String)

#undef CONSTRUCTOR
#undef CONSTRUCTOR_CAST

        explicit Node(const std::vector<std::shared_ptr<Node<C>>> list) {
            type = ValueType::List;
            value = new Value<C>(new std::vector<Node<C> *>());
            for (auto &i: list) {
                if (i)
                    value->list->push_back(i);
            }
            value->list->shrink_to_fit();
        }

        explicit Node(const std::map<str<C>, std::shared_ptr<Node<C>>> obj) {
            type = ValueType::Object;
            value = new Value<C>(new std::map<str<C>, Node<C> *>());
            for (auto &i: obj) {
                if (i.second)
                    value->obj->at(i.first) = i.second;
            }
        }

        template<typename T>
        explicit Node(T val) : Node(as_if<C, T>(val)){};

        Node(const Node<C> &node) = delete;

        Node(Node<C> &node) = delete;

        Node(const Node<C> &&node) noexcept {
            type = node.type;
            value = node.value;
        };

        Node(Node<C> &&node) = default;

        ~Node() {
            cleanValue();
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

        bool IsNull() noexcept {
            return type == ValueType::Null;
        }

        str<C> Str() {
            return type == ValueType::String ? value->s : str<C>();
        }

#define AS_DEF(t, N, d)    \
    t as##N(t def = d) {   \
        return as<t>(def); \
    }

        AS_DEF(bool, Bool, false)
        AS_DEF(int64_t, Int, 0)
        AS_DEF(double, Float, 0)
        AS_DEF(str<C>, String, str<C>())

#undef AS_DEF
        str<C> asStr(const str<C> &def) {
            return as<str<C>>(def);
        }

        std::vector<std::shared_ptr<Node<C>>> asList() const noexcept {
            return type == ValueType::List ? value->list : std::vector<Node<C> *>();
        }

        std::map<str<C>, std::shared_ptr<Node<C>>> asMap() const noexcept {
            return type == ValueType::Object ? value->obj : std::map<str<C>, Node<C> *>();
        }

        /**
         * @brief 获取子节点，本节点必须是Object或Null类型，如果是Null类型则会转换成Object类型
         * @param key 节点名
         * @return 子节点或新的Null节点
         * @throw std::runtime_error 如果不是Object类型
         * @throw std::invalid_argument 如果key为空
         */
        std::shared_ptr<Node<C>> get(const str<C> &key) {
            if (type == ValueType::Null) {
                convert2obj();
            } else if (type != ValueType::Object)
                throw std::runtime_error("not a object");

            auto map = value->obj;
            str<C> k = key;
            k.erase(k.begin(), std::find_if(k.begin(), k.end(), [](C ch) {
                return !std::isspace((int) ch);
            }));
            k.erase(std::find_if(k.rbegin(), k.rend(), [](C ch) {
                return !std::isspace((int) ch);
            }).base(),
                    k.end());

            if (k.empty())
                goto bad_arg;

            if (map->find(key) == map->end()) {
                map->insert(std::make_pair(key, std::make_shared<Node<C>>()));
            }
            return std::shared_ptr(map->at(key));
        bad_arg:
            throw std::invalid_argument("bad key");
        }

        /**
         * @brief 获取子节点，本节点必须是List或Null类型，如果是Null类型则会转换成List类型
         * @param index 索引，可以是负数，表示从后往前
         * @return 子节点
         * @throw std::runtime_error 如果不是List类型
         * @throw std::out_of_range 如果下标越界
         */
        std::shared_ptr<Node<C>> get(int index) {
            if (type == ValueType::Null) {
                convert2list();
                throw std::runtime_error("empty list");
            } else if (type != ValueType::List)
                throw std::runtime_error("not a list");
            auto list = value->list;
            int size = list->size();
            if (index <= -size || index >= size)
                throw std::out_of_range("index out of range");
            return std::shared_ptr(index >= 0 ? list->at(index) : list->at(size + index));
        }

        /**
         * @brief 转换到值，不能转换则为默认值
         * @tparam T 目标类型
         * @return 结果
         */
        template<typename T>
        T as(T def) const noexcept {
            return as_if_convert<C, T>(*this)[def];
        }

        template<typename K>
        std::shared_ptr<Node<C>> operator[](K key) noexcept {
            return as_if_get<C, K>(*this)[key];
        }

        void set() {
            cleanValue();
            value = new Value<C>();
        }

        template<typename V>
        void set(V val);

#define SETTER(t, T)             \
    template<>                   \
    void set(t v) {              \
        cleanValue();            \
        type = T;                \
        value = new Value<C>(v); \
    }

#define SETTER_CAST(t, ct, T)                      \
    template<>                                     \
    void set(t v) {                                \
        cleanValue();                              \
        type = T;                                  \
        value = new Value<ct>(static_cast<ct>(v)); \
    }

        SETTER(bool, Bool)
        SETTER_CAST(int8_t, int64_t, Int)
        SETTER_CAST(uint8_t, int64_t, Int)
        SETTER_CAST(int16_t, int64_t, Int)
        SETTER_CAST(uint16_t, int64_t, Int)
        SETTER_CAST(int32_t, int64_t, Int)
        SETTER_CAST(uint32_t, int64_t, Int)
        SETTER(int64_t, Int)
        SETTER(double, Float)
        SETTER_CAST(float, double, Float)
        SETTER(str<C>, String)
        SETTER(const C *, String)
        SETTER(std::vector<std::shared_ptr<Node<C>>> *, List)

        template<>
        void set(std::map<str<C>, std::shared_ptr<Node<C>>> *v) {
            cleanValue();
            type = Object;
            value = new Value<C>(v);
        }

#undef SETTER_CAST
#undef SETTER


        template<typename V>
        Node<C> &operator=(V val) {
            as_if_set<C, V>(this)(val);
            return *this;
        }

        void Parse(const str<C> &text) {
            if (type != ValueType::Null)
                throw std::runtime_error("could not run parse on not a null node");
            std::list<str<C>> lines;
            str<C> s = text, line;
            while (parser_get_line(s, line))
                lines.push_back(str<C>(line.begin(), line.end()));
            parse(lines, 0);
        }

        template<std::size_t N>
        str<C> Dump(const C eol[N]) {
            std::basic_ostringstream<C> out;
            auto lines = dump();
            for (auto &i: lines) {
                out << i << eol;
            }
            return out.str();
        }
    };
}// namespace UYAML
