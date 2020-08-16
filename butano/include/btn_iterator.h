#ifndef BTN_ITERATOR_H
#define BTN_ITERATOR_H

#include <iterator>
#include "btn_common.h"

namespace btn
{
    using std::forward_iterator_tag;

    using std::bidirectional_iterator_tag;

    using std::random_access_iterator_tag;

    using std::reverse_iterator;
}

#endif
