#pragma once

#include "../node-impl.h"

namespace UYAML {

    IMPL_CONVERTER_TRY_DEF(wchar_t, bool, bool);

    IMPL_CONVERTER_INTS(wchar_t);

    IMPL_CONVERTER_FLOATS(wchar_t);

    IMPL_CONVERTER_STR(wchar_t);

}// namespace UYAML