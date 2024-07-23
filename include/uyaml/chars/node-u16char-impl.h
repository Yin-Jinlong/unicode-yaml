#pragma once

#include "../node-impl.h"

namespace UYAML {

    IMPL_CONVERTER_TRY_DEF(char16_t, bool, bool);

    IMPL_CONVERTER_INTS(char16_t);

    IMPL_CONVERTER_FLOATS(char16_t);

    IMPL_CONVERTER_STR(char16_t);

}