#pragma once

#include "dll.h"

namespace UYAML {
    enum UYAML_API ValueType {
        Null,
        Bool,
        Int,
        Float,
        String,
        List,
        Object
    };
}
