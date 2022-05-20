// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020 Antonio Niño Díaz

#ifndef TIMER_H__
#define TIMER_H__

#include "definitions.h"

// Start timer. The value passed to prescaler can be 1, 64, 256 or 1024.
EXPORT_API
void TM_TimerStart(int index, uint16_t reload_value, int prescaler,
                   int enable_irq);

// Start timer in cascade mode. Note that the prescaler value is ignored in this
// mode, so it isn't needed.
EXPORT_API
void TM_TimerStartCascade(int index, uint16_t period_ticks, int enable_irq);

// Start timer that is triggered every `period_ms` milliseconds. Internally it
// looks for the lowest prescaler that allows this period (to have the best
// accuracy). It returns 0 on success.
EXPORT_API
int TM_TimerStartMs(int index, uint32_t period_ms, int enable_irq);

// Stop the specified timer.
EXPORT_API
void TM_TimerStop(int index);

#endif // TIMER_H__

