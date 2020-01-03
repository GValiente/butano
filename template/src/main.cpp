#include "btn_core.h"

int main()
{
    btn::core::init();

    while(true)
    {
        btn::core::update();
    }
}
