#ifndef BTN_FIXED_FWD_H
#define BTN_FIXED_FWD_H

#include "btn_common.h"

namespace btn
{

template<int Precision>
class fixed_t;

using fixed = fixed_t<12>;

}

#endif
