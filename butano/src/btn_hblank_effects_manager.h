#ifndef BTN_HBLANK_EFFECTS_MANAGER_H
#define BTN_HBLANK_EFFECTS_MANAGER_H

#include "btn_common.h"

namespace btn::hblank_effects_manager
{
    void init();

    void enable();

    void disable();

    void update();

    void commit();

    void stop();
}

#endif
