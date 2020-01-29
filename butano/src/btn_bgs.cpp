#include "btn_bgs.h"

#include "btn_bgs_manager.h"

namespace btn::bgs
{

int used_count()
{
    return bgs_manager::used_count();
}

int available_count()
{
    return bgs_manager::available_count();
}

}
