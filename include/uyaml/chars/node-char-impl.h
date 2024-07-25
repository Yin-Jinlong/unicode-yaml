#pragma once

#include "../node-impl.h"

namespace UYAML {

    IMPL_CONVERTER_TRY_DEF(char, bool, bool);

    IMPL_CONVERTER_INTS(char);

    IMPL_CONVERTER_FLOATS(char);

    IMPL_CONVERTER_STR(char);

}// namespace UYAML