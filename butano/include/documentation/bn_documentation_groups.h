/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DOCUMENTATION_GROUPS_H
#define BN_DOCUMENTATION_GROUPS_H

/**
 * @defgroup core Core
 *
 * It coordinates all of the other modules.
 *
 * It is needed to start the engine and to update it each frame, among other things.
 */

/**
 * @defgroup display Display
 *
 * That square window of 240x160 pixels capable of displaying 32768 colors (15 bits).
 */

/**
 * @defgroup tile Tiles
 *
 * On the GBA, the pixel data of sprites and backgrounds is made of pixel tiles: 8x8 pixel matrices.
 *
 * @ingroup display
 */

/**
 * @defgroup color Colors
 *
 * GBA displays 15 bits colors: 5 bits for the blue component, 5 for the green one and 5 for the red one.
 *
 * @ingroup display
 */

/**
 * @defgroup palette Color palettes
 *
 * On the GBA, both sprites and backgrounds have 256 colors organized in blocks of multiples of 16 colors
 * (512 colors in total). Each one of these blocks is called color palette.
 *
 * There's 8 bits per pixel palettes (which can refer to all colors)
 * and 4 bits per pixel palettes (which can access only to 16 consecutive colors).
 *
 * Butano color palettes system allows to manage individual 4 bits per pixel palettes for sprites and backgrounds,
 * but it only can be one 8 bits per pixel backgrounds palette and one 8 bits per pixel sprites palette:
 * all 8 bits per pixel backgrounds share the same 8 bits per pixel backgrounds palette
 * and all 8 bits per pixel sprites share the same 8 bits per pixel sprites palette.
 *
 * @ingroup display
 */

/**
 * @defgroup affine_mat Affine transformation matrices
 *
 * Used to apply two-dimensional geometric transformations like rotating and/or scaling to sprites and backgrounds.
 *
 * @ingroup display
 */

/**
 * @defgroup bg Backgrounds
 *
 * Big graphics layers.
 *
 * Depending of their type, the GBA offers up to 4 of them.
 *
 * @ingroup display
 */

/**
 * @defgroup bg_map Background maps
 *
 * Also called tile-maps, backgrounds use them to refer to the tile to render in each 8x8 pixel block.
 *
 * @ingroup bg
 */

/**
 * @defgroup regular_bg Regular backgrounds
 *
 * Backgrounds which can't perform affine transformations.
 *
 * Also known as text backgrounds.
 *
 * @ingroup bg
 */

/**
 * @defgroup affine_bg Affine backgrounds
 *
 * Backgrounds which can perform affine transformations.
 *
 * Also known as rotation backgrounds.
 *
 * @ingroup bg
 */

/**
 * @defgroup sprite Sprites
 *
 * Small animated objects that can move freely from the background(s).
 *
 * In GBA's jargon they are called OBJs.
 *
 * @ingroup display
 */

/**
 * @defgroup backdrop Backdrop
 *
 * Single color layer displayed behind all backgrounds and sprites.
 *
 * @ingroup display
 */

/**
 * @defgroup mosaic Mosaic
 *
 * It makes sprites and backgrounds look blocky.
 *
 * It divides a sprite or background into square blocks of pixels.
 * The top-left pixel of each block is used to fill the rest of that block, which makes it blocky.
 *
 * @ingroup display
 */

/**
 * @defgroup blending Blending
 *
 * Allows transparency and fade effects for sprites and backgrounds.
 *
 * @ingroup display
 */

/**
 * @defgroup window Windows
 *
 * They allow to divide the screen into regions.
 *
 * For each one of these regions, they can show or hide sprites, backgrounds and blending between them.
 *
 * @ingroup display
 */

/**
 * @defgroup inside_window Inside windows
 *
 * Windows which region can be covered by other windows.
 *
 * @ingroup window
 */

/**
 * @defgroup rect_window Rectangle windows
 *
 * Rectangular windows.
 *
 * @ingroup window
 */

/**
 * @defgroup audio Audio
 *
 * Music for your ears, using both Direct Sound and DMG channels.
 */

/**
 * @defgroup music Music
 *
 * Module files (files with `*.mod`, `*.xm`, `*.s3m` and `*.it` extensions) played with Direct Sound channels
 * thanks to <a href="https://maxmod.devkitpro.org/">Maxmod</a>.
 *
 * @ingroup audio
 */

/**
 * @defgroup dmg_music DMG music
 *
 * Module files (files with `*.mod` and `*.s3m` extensions) played with DMG channels
 * thanks to <a href="https://github.com/AntonioND/gbt-player">GBT Player</a>
 * and VGM audio files (files with `*.vgm` extension) played with DMG channels
 * thanks to <a href="https://github.com/copyrat90/gbadev-ja-test">VGM player</a>.
 *
 * @ingroup audio
 */

/**
 * @defgroup sound Sound effects
 *
 * Waveform audio files (files with `*.wav` extension) sound effects played with Direct Sound channels
 * thanks to <a href="https://maxmod.devkitpro.org/">Maxmod</a>.
 *
 * @ingroup audio
 */

/**
 * @defgroup keypad Keypad
 *
 * GBA keys handling.
 *
 * Keypad logging can be enabled or disabled by overloading the definition of @ref BN_CFG_KEYPAD_LOG_ENABLED.
 *
 * Recorded key presses can be replayed later by passing the log to bn::core::init.
 */

/**
 * @defgroup text Text
 *
 * Butano text system, based on sprites.
 *
 * Currently, it supports 4 bits per pixel (16 colors) fixed width AND variable width characters.
 *
 * Text can be printed in one sprite per character or multiple characters per sprite.
 *
 * Also, UTF-8 characters are supported.
 *
 * @ingroup display
 */

/**
 * @defgroup green_swap Green swap
 *
 * When it is enabled, green intensity of each two screen pixels are exchanged.
 *
 * It produces an interesting dirt effect.
 *
 * @ingroup display
 */

/**
 * @defgroup hblank_effect H-Blank effects
 *
 * They allow to change how things are drawn in each screen horizontal line,
 * since they transfer data to the GBA during each horizontal blanking (H-Blank) period.
 *
 * For example, with a H-Blank effect you can change the transparent color every screen horizontal line,
 * which is how games like Chrono Trigger fills their menus with a color gradient.
 *
 * They differ from HDMA in that only a register can be written by a H-Blank effect in each screen horizontal line,
 * but since multiple H-Blank effects can be displayed at the same time,
 * non consecutive registers can be written with them.
 *
 * They are also higher level than HDMA, so they should be your first option.
 *
 * @ingroup display
 */

/**
 * @defgroup hdma H-Blank direct memory access
 *
 * It allows to change how things are drawn in each screen horizontal line,
 * since they transfer data to the GBA during each horizontal blanking (H-Blank) period.
 *
 * For example, with HDMA you can change the transparent color every screen horizontal line,
 * which is how games like Chrono Trigger fills their menus with a color gradient.
 *
 * It differs from H-Blank effects in that multiple registers can be written by HDMA in each screen horizontal line,
 * but since there's only one DMA channel available, written registers must be consecutive.
 *
 * It is also lower level than H-Blank effects, so you should try with H-Blank effects first.
 *
 * @ingroup display
 */

/**
 * @defgroup link Link communication
 *
 * Multiplayer communication through a link cable,
 * provided by <a href="https://github.com/rodri042/gba-link-connection">gba-link-connection</a>.
 *
 * Keep in mind that some messages will be lost between players.
 */

/**
 * @defgroup timer Timers
 *
 * They allow to measure elapsed times with high precision.
 */

/**
 * @defgroup game_pak Game Pak
 *
 * Each GBA game or application is stored on a plastic cartridge, officially called "Game Pak".
 */

/**
 * @defgroup sram Game Pak SRAM
 *
 * Allows game or application data to be saved when the GBA is turned off.
 *
 * @ingroup game_pak
 */

/**
 * @defgroup date_time Date and time
 *
 * Utilities for the real time clock (RTC), if the cart has one.
 */

/**
 * @defgroup rumble Rumble
 *
 * It allows your GBA to vibrate, if the cart supports it.
 */

/**
 * @defgroup camera Cameras
 *
 * They allow to set the position of sprites, backgrounds and rectangle windows relative to another position.
 *
 * @ingroup display
 */

/**
 * @defgroup memory Memory
 *
 * Memory management utilities.
 */

/**
 * @defgroup assert Asserts
 *
 * Butano assert system.
 *
 * It can be enabled or disabled by overloading the definition of @ref BN_CFG_ASSERT_ENABLED.
 *
 * Note that these asserts can be used in `constexpr` contexts (`if consteval` returns `true`).
 */

/**
 * @defgroup log Logging
 *
 * Butano logging system.
 *
 * It allows to print information on the console of multiple emulators.
 *
 * It can be enabled or disabled by overloading the definition of @ref BN_CFG_LOG_ENABLED.
 *
 * It supports printing on only one emulator at once.
 * The supported emulator can be changed by overloading the definition of @ref BN_CFG_LOG_BACKEND.
 */

/**
 * @defgroup profiler Profiler
 *
 * Butano profiling system.
 *
 * It allows to measure elapsed time between code blocks defined by the user.
 *
 * It can be enabled or disabled by overloading the definition of @ref BN_CFG_PROFILER_ENABLED.
 */

/**
 * @defgroup std Standard library
 *
 * Butano standard library replacement.
 *
 * Main differences against C++'s standard library is the usage of asserts instead of exceptions
 * and that it avoids heap usage when possible.
 */

/**
 * @defgroup container Containers
 *
 * STL like containers with the capacity defined at compile time.
 *
 * They don't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup std
 */

/**
 * @defgroup vector Vector
 *
 * `std::vector` like container with the capacity defined at compile time.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup deque Deque
 *
 * `std::deque` like container with the capacity defined at compile time.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup list List
 *
 * `std::list` like container with the capacity defined at compile time.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup intrusive_list Intrusive list
 *
 * `std::list` like container that doesn't contain values, it just references them.
 *
 * To be part of an intrusive list, values must inherit bn::intrusive_list_node_type class.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup forward_list Forward list
 *
 * `std::forward_list` like container with the capacity defined at compile time.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup intrusive_forward_list Intrusive forward list
 *
 * `std::forward_list` like container that doesn't contain values, it just references them.
 *
 * To be part of an intrusive forward list, values must inherit bn::intrusive_forward_list_node_type class.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup unordered_map Unordered map
 *
 * `std::unordered_map` like container with the capacity defined at compile time.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup unordered_set Unordered set
 *
 * `std::unordered_set` like container with the capacity defined at compile time.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup string Strings
 *
 * Strings describe objects that can refer to a constant contiguous sequence of char values.
 *
 * @ingroup container
 */

/**
 * @defgroup array Array
 *
 * `std::array` like container.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup bitset Bitset
 *
 * `std::bitset` like container.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup span Span
 *
 * `std::span` like container.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup optional Optional
 *
 * `std::optional` like container.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup any Any
 *
 * `std::any` like container with the capacity defined at compile time.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup unique_ptr Unique pointer
 *
 * `std::unique_ptr` like container.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * Unlike most Butano containers, it uses the heap instead of the stack.
 *
 * @ingroup container
 */

/**
 * @defgroup pool Pool
 *
 * A fixed capacity object pool, where allocation and release are O(1) operations.
 *
 * A pool doesn't destroy its elements in its destructor, they must be destroyed manually.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup allocator Allocator
 *
 * Generic allocators where allocation and release are not O(1) operations.
 *
 * An allocator doesn't destroy its elements in its destructor, they must be destroyed manually.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup utility Utilities
 *
 * Standard library utilities.
 *
 * @ingroup std
 */

/**
 * @defgroup functional Functional
 *
 * Part of the standard function objects library.
 *
 * It provides the standard hash function.
 *
 * @ingroup std
 */

/**
 * @defgroup action Actions
 *
 * Actions allow the transformation of properties in time.
 *
 * Some of Butano classes and namespaces can have actions performed on it.
 *
 * For example, you can move a sprite from its current position to another and do it over a span of time.
 */

/**
 * @defgroup template_action Action templates
 *
 * Templates used to build Butano actions.
 *
 * You can use them to build your own actions too.
 *
 * @ingroup action
 */

/**
 * @defgroup tool Tools
 *
 * Stuff generated by the assets conversion tools to use your assets with Butano.
 */

/**
 * @defgroup math Math
 *
 * Math related stuff.
 */

/**
 * @defgroup random Random
 *
 * Random number generators.
 */

/**
 * @defgroup other Other
 *
 * Stuff which doesn't fit anywhere else.
 */

#endif
