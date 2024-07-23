#pragma once

#include "dll.h"

#include <cstdint>
#include <xstring>
#include <vector>
#include <map>

#include "value-type.h"

namespace UYAML {

    template<typename C>
    using str = std::basic_string<C, std::char_traits<C>, std::allocator<C>>;

    template<typename C>
    class Node;

    template<typename C>
    union Value;

    typedef Node<char> CNode;
    typedef Node<wchar_t> WNode;
    typedef Node<char8_t> U8Node;
    typedef Node<char16_t> U16Node;
    typedef Node<char32_t> U32Node;

    typedef std::vector<CNode *> CNodeList;
    typedef std::vector<WNode *> WNodeList;
    typedef std::vector<U8Node *> U8NodeList;
    typedef std::vector<U16Node *> U16NodeList;
    typedef std::vector<U32Node *> U32NodeList;

    typedef std::map<str<char>, CNode *> CNodeMap;
    typedef std::map<str<wchar_t>, WNode *> WNodeMap;
    typedef std::map<str<char8_t>, U8Node *> U8NodeMap;
    typedef std::map<str<char16_t>, U16Node *> U16NodeMap;
    typedef std::map<str<char32_t>, U32Node *> U32NodeMap;

}