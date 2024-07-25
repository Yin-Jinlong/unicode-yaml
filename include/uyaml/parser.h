#pragma once

#include "def.h"

#include <fstream>
#include <queue>
#include <sstream>
#include <stdexcept>

namespace UYAML {

#ifndef UYAML_BLANKS
    #define UYAML_BLANKS U" \t\r\n"
#endif

    typedef const char32_t blank;

    /**
        * @brief 移除头部最多n个空格
        * @param str 移除后的字符串
        * @param count 最多个数
        * @return 移除的空格个数
        */
    template<typename C, std::size_t N = 3>
    size_t parser_trim_start(str<C> &str, int count = INT32_MAX, blank blanks[N] = UYAML_BLANKS) {
        size_t sc = 0;
    next:
        while (str.size()) {
            C c = str[0];
            for (int i = 0; i < N; ++i) {
                if (c != blanks[i])
                    continue;
                str.erase(0, 1);
                sc++;
                if (sc >= count)
                    return sc;
                goto next;
            }
            break;
        }
        return sc;
    }


    /**
     * @brief 移除尾部空格
     * @param str 移除后的字符串
     * @return 空格个数
     */
    template<typename C, std::size_t N = 3>
    size_t parser_trim_end(str<C> &str, blank blanks[N] = UYAML_BLANKS) {
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

    template<typename C, std::size_t N = 3>
    size_t parser_trim(str<C> &str, blank blanks[N] = UYAML_BLANKS) {
        return parser_trim_start<C, N>(str, INT32_MAX, blanks) + parser_trim_end<C, N>(str, blanks);
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

    template<typename C>
    bool parser_is_comment_or_blank(const str<C> &line) {
        return line.empty() || line[0] == '#';
    }
}// namespace UYAML