#include "btn_hblank_effects.h"

#include "btn_hblank_effects_manager.h"

namespace btn::hblank_effects
{

int used_count()
{
    return hblank_effects_manager::used_count();
}

int available_count()
{
    return hblank_effects_manager::available_count();
}

}
