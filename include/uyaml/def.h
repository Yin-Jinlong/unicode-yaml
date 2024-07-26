#pragma once

#include "dll.h"

#include <cstdint>
#include <map>
#include <vector>
#include <xstring>

#include "value-type.h"

namespace UYAML {

    template<typename C>
    using str = std::basic_string<C>;

    template<typename C>
    class Node;

    template<typename C>
    union Value;

    typedef Node<char> CNode;
    typedef Node<wchar_t> WNode;
    typedef Node<char8_t> U8Node;
    typedef Node<char16_t> U16Node;
    typedef Node<char32_t> U32Node;

    typedef std::vector<std::shared_ptr<CNode>> CNodeList;
    typedef std::vector<std::shared_ptr<WNode>> WNodeList;
    typedef std::vector<std::shared_ptr<U8Node>> U8NodeList;
    typedef std::vector<std::shared_ptr<U16Node>> U16NodeList;
    typedef std::vector<std::shared_ptr<U32Node>> U32NodeList;

    typedef std::map<str<char>, std::shared_ptr<CNode>> CNodeMap;
    typedef std::map<str<wchar_t>, std::shared_ptr<WNode>> WNodeMap;
    typedef std::map<str<char8_t>, std::shared_ptr<U8Node>> U8NodeMap;
    typedef std::map<str<char16_t>, std::shared_ptr<U16Node>> U16NodeMap;
    typedef std::map<str<char32_t>, std::shared_ptr<U32Node>> U32NodeMap;

}// namespace UYAML