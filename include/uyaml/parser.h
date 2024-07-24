#pragma once

#include <sstream>
#include <fstream>
#include <queue>
#include "node.h"

namespace UYAML {

    /**
        * @brief 移除头部空格
        * @param str 移除后的字符串
        * @return 空格个数
        */
    template<typename C>
    size_t parser_trim_start(str<C> &str) {
        size_t count = 0;
        while (str.size() && str[0] == ' ') {
            str.erase(0, 1);
            count++;
        }
        return count;
    }


    /**
     * @brief 移除尾部空格
     * @param str 移除后的字符串
     * @return 空格个数
     */
    template<typename C>
    size_t parser_trim_end(str<C> &str) {
        size_t count = 0;
        while (str.size()) {
            auto c = str[str.size() - 1];
            if (c == ' ' || c == '\n') {
                str.erase(str.size() - 1, 1);
                count++;
            } else
                break;
        }
        return count;
    }

    template<typename C>
    size_t parser_trim(str<C> &str) {
        return parser_trim_start(str) + parser_trim_end(str);
    }

    /**
     * @brief 分割key和value
     * @tparam C
     * @param [in]line
     * @param [out]key
     * @param [out]value
     * @return 是否成功分割
     */
    template<typename C>
    bool parser_split_kv(const str<C> &line, str<C> &key, str<C> &value) {
        if (line.size() < 2)
            return false;
        auto index = line.find_first_of(':');
        if (index == std::string::npos)
            return false;
        key = line.substr(0, index);
        value = line.substr(index + 1);
        parser_trim(key);
        parser_trim(value);
        return !key.empty();
    }

    template<typename C>
    bool parser_get_line(str<C> &text, str<C> &line) {
        int index = 0;
        while (index < text.size()) {
            if (text[index] == '\n') {
                line = text.substr(0, index);
                text.erase(0, index + 1);
                return true;
            }
            index++;
        }
        line = text;
        text.clear();
        return index;
    }
}