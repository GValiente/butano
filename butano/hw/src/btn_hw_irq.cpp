#include "../include/btn_hw_irq.h"

#include "tonc.h"

namespace btn::hw::irq
{

void init()
{
    irq_init(nullptr);
}

void add(id irq_id, void(*isr)())
{
    irq_add(eIrqIndex(irq_id), isr);
}

void remove(id irq_id)
{
    irq_delete(eIrqIndex(irq_id));
}

}
