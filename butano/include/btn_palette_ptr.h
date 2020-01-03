#ifndef BTN_PALETTE_PTR_H
#define BTN_PALETTE_PTR_H

#include "btn_common.h"

namespace btn
{

class color;

class palette_ptr
{

public:
    [[nodiscard]] int id() const
    {
        return _id;
    }

protected:
    int8_t _id;

    explicit palette_ptr(int id) :
        _id(int8_t(id))
    {
    }
};

}

#endif
