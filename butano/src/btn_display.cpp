#include "btn_display.h"

#include "btn_point.h"
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

point center()
{
    return point(hw::display::width() / 2, hw::display::height() / 2);
}

}
