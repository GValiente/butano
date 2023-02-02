/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_irq.h"

extern "C"
{
    #include "../3rd_party/libugba/include/ugba/interrupts.h"
}

namespace bn::hw::irq
{

void init()
{
    IRQ_Init();
}

void set_isr(id irq_id, void(*isr)())
{
    IRQ_SetHandler(irq_index(irq_id), isr);
}

void enable(id irq_id)
{
    IRQ_Enable(irq_index(irq_id));
}

void disable(id irq_id)
{
    IRQ_Disable(irq_index(irq_id));
}

}
