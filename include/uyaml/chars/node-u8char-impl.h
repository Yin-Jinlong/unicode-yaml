#pragma once

#include "../node-impl.h"

namespace UYAML {

    IMPL_CONVERTER_TRY_DEF(char8_t, bool, bool);

    IMPL_CONVERTER_INTS(char8_t);

    IMPL_CONVERTER_FLOATS(char8_t);

    IMPL_CONVERTER_STR(char8_t);

}