/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_DOCUMENTATION_H
#define BTN_DOCUMENTATION_H

// NAMESPACE

/**
 * @namespace btn
 * @brief Main butano namespace.
 */


// GROUPS

/**
 * @defgroup tile Tiles
 *
 * On the GBA, the pixel data of sprites and backgrounds is made of pixel tiles: 8x8 pixel matrices.
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
 * Butano's color palettes system allows to manage individual 4 bits per pixel palettes for sprites and backgrounds,
 * but it only can be one 8 bits per pixel backgrounds palette and one 8 bits per pixel sprites palette:
 * all 8 bits per pixel backgrounds share the same 8 bits per pixel backgrounds palette
 * and all 8 bits per pixel sprites share the same 8 bits per pixel sprites palette.
 */

/**
 * @defgroup affine_mat Affine transformation matrices
 *
 * Used to apply two-dimensional geometric transformations like rotating and/or scaling to sprites and backgrounds.
 */

/**
 * @defgroup bg Backgrounds
 *
 * Big graphics layers.
 *
 * Depending of their type, the GBA offers up to 4 of them.
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
 */

/**
 * @defgroup mosaic Mosaic
 *
 * It makes sprites and backgrounds look blocky.
 *
 * It divides a sprite or background into square blocks of pixels.
 * The top-left pixel of each block is used to fill the rest of that block, which makes it blocky.
 */

/**
 * @defgroup window Windows
 *
 * They allow to divide the screen into regions.
 *
 * For each one of these regions, they can show or hide sprites, backgrounds and blending between them.
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
 * Music for your ears, provided by maxmod (https://maxmod.devkitpro.org/).
 */

/**
 * @defgroup music Music
 *
 * Module files (files with *.mod, *.xm, *.s3m and *.it extensions) music.
 *
 * @ingroup audio
 */

/**
 * @defgroup sound Sound effects
 *
 * Waveform audio files (files with *.wav extension) sound effects.
 *
 * @ingroup audio
 */

/**
 * @defgroup keypad Keypad
 *
 * GBA keys handling.
 *
 * Keypad logging can be enabled or disabled by overloading the definition of @a BTN_CFG_KEYPAD_LOG_ENABLED @a .
 *
 * Recorded key presses can be replayed later by passing the log to @a btn::core::init() @a .
 */

/**
 * @defgroup text Text
 *
 * Butano's text system, based on sprites.
 *
 * Currently, it supports 8x8 and 8x16 fixed width AND variable width fonts of 16 colors (4 bits per pixel).
 *
 * Text can be printed in one sprite per character or multiple characters per sprite.
 *
 * Also, UTF-8 characters are supported.
 */

/**
 * @defgroup green_swap Green swap
 *
 * When it is enabled, green intensity of each two screen pixels are exchanged.
 *
 * It produces an interesting dirt effect.
 */

/**
 * @defgroup hblank_effect H-Blank effects
 *
 * They allow to change how things are drawn in each screen horizontal line,
 * since they transfer data to the GBA during each horizontal blanking (H-Blank) period.
 *
 * For example, with a H-Blank effect you can change the transparent color every screen horizontal line,
 * which is how games like Chrono Trigger fills their menus with a color gradient.
 */

/**
 * @defgroup memory Memory
 *
 * Memory management utilities.
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
 * @defgroup assert Asserts
 *
 * Butano's assert system.
 *
 * It can be enabled or disabled by overloading the definition of @a BTN_CFG_ASSERT_ENABLED @a .
 *
 * Note that these asserts can be used in constexpr contexts (is_constant_evaluated() returns <b>true</b>).
 */

/**
 * @defgroup log Logging
 *
 * Butano's logging system.
 *
 * It allows to print information on the console of multiple emulators.
 *
 * It can be enabled or disabled by overloading the definition of @a BTN_CFG_LOG_ENABLED @a .
 *
 * It supports printing on only one emulator at once.
 * The supported emulator can be changed by overloading the definition of @a BTN_CFG_LOG_BACKEND @a .
 */

/**
 * @defgroup profiler Profiler
 *
 * Butano's profiling system.
 *
 * It allows to measure elapsed time between code blocks defined by the user.
 *
 * It can be enabled or disabled by overloading the definition of @a BTN_CFG_PROFILER_ENABLED @a .
 */

/**
 * @defgroup std Standard library
 *
 * Butano's standard library replacement.
 *
 * The biggest difference against C++'s standard library is the usage of asserts instead of exceptions
 * and that it avoids the heap when possible.
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
 * A std::vector like container with the capacity defined at compile time.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup list List
 *
 * A std::list like container with the capacity defined at compile time.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup intrusive_list Intrusive list
 *
 * A std::list like container that doesn't contain values, it just references them.
 *
 * To be part of an intrusive list, values must inherit @a btn::intrusive_list_node_type @a class.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup intrusive_forward_list Intrusive forward list
 *
 * A std::forward_list like container that doesn't contain values, it just references them.
 *
 * To be part of an intrusive forward list, values must inherit @a btn::intrusive_forward_list_node_type @a class.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup unordered_map Unordered map
 *
 * A std::unordered_map like container with the capacity defined at compile time.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup unordered_set Unordered set
 *
 * A std::unordered_set like container with the capacity defined at compile time.
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
 * @defgroup span Span
 *
 * A std::span like container.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup optional Optional
 *
 * A std::optional like container.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
 *
 * @ingroup container
 */

/**
 * @defgroup unique_ptr Unique pointer
 *
 * A std::unique_ptr like container.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
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
 * @defgroup utility Utilities
 *
 * Standard library utilities.
 *
 * @ingroup std
 */

/**
 * @defgroup action Actions
 *
 * Actions allow the transformation of properties in time.
 *
 * Some of butano's classes and namespaces can have actions performed on it.
 *
 * For example, you can move a sprite from its current position to another and do it over a span of time.
 */

/**
 * @defgroup template_action Action templates
 *
 * Templates used to build butano's actions.
 *
 * You can use them to build your own actions too.
 *
 * @ingroup action
 */

/**
 * @defgroup tool Tools
 *
 * Stuff generated by the assets conversion tools to use your assets with butano.
 */

/**
 * @defgroup math Math
 *
 * Math related stuff.
 */

/**
 * @defgroup other Other
 *
 * Stuff which doesn't fit anywhere else.
 */


// PAGES

/**
 * @mainpage My Personal Index Page
 *
 * @section intro_sec Introduction
 *
 * This is the introduction.
 *
 * @section install_sec Installation
 *
 * @subsection step1 Step 1: Opening the box
 *
 * etc...
 */

/**
 * @page page1 A documentation page
 * @tableofcontents
 * Leading text.
 * @section sec An example section
 * This page contains the subsections @ref subsection1 and @ref subsection2.
 * For more info see page @ref page2.
 * @subsection subsection1 First subsection
 * Text.
 * @subsection subsection2 Second subsection
 * More text.
 */

/**
 * @page page2 Another page
 * Even more info.
 */

#endif
