#pragma once

#include "../node-impl.h"

namespace UYAML {

    IMPL_CONVERTER_TRY_DEF(char32_t, bool, bool);

    IMPL_CONVERTER_INTS(char32_t);

    IMPL_CONVERTER_FLOATS(char32_t);

    IMPL_CONVERTER_STR(char32_t);

}// namespace UYAML