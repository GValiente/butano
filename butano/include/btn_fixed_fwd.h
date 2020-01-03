#ifndef BTN_FIXED_FWD_H
#define BTN_FIXED_FWD_H

#include "btn_common.h"

namespace btn
{

template<size_t Precision>
class fixed_t;

using fixed = fixed_t<12>;

}

#endif
