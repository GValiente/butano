// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020 Antonio Niño Díaz

#ifndef INPUT_H__
#define INPUT_H__

#include <stdint.h>

#include "definitions.h"

// Update keypad state.
EXPORT_API void KEYS_Update(void);

// Returns keys that have just been pressed.
EXPORT_API uint16_t KEYS_Pressed(void);

// Returns keys that are pressed.
EXPORT_API uint16_t KEYS_Held(void);

// Returns keys that have just been released.
EXPORT_API uint16_t KEYS_Released(void);

// Enable keypad interrupt that is triggered when all specified keys are pressed
// at the same time.
EXPORT_API void KEYS_IRQEnablePressedAll(uint16_t keys);

// Enable keypad interrupt that is triggered when any of the specified keys is
// pressed at any time.
EXPORT_API void KEYS_IRQEnablePressedAny(uint16_t keys);

// Disable keypad interrupt.
EXPORT_API void KEYS_IRQDisable(void);

#endif // INPUT_H__
