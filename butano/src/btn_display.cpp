#include "btn_display.h"

#include "../hw/include/btn_hw_display.h"

namespace btn::display
{

int width()
{
    return hw::display::width();
}

int height()
{
    return hw::display::height();
}

size dimensions()
{
    return hw::display::dimensions();
}

}
