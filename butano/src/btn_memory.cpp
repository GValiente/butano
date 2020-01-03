#include "btn_memory.h"

#include "../hw/include/btn_hw_memory.h"

namespace btn
{

int used_static_iwram()
{
    return hw::used_static_iwram();
}

int used_static_ewram()
{
    return hw::used_static_ewram();
}

int used_malloc_ewram()
{
    return hw::used_malloc_ewram();
}

}
