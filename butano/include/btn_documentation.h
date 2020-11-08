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
 * Butano's color palettes system allows to manage individual 4 bits per pixel palettes for sprites and backgrounds,
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
 * @ingroup display
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
 * Note that these asserts can be used in constexpr contexts (is_constant_evaluated() returns `true`).
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
 * @defgroup deque Deque
 *
 * A std::deque like container with the capacity defined at compile time.
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
 * @defgroup forward_list Forward list
 *
 * A std::forward_list like container with the capacity defined at compile time.
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
 * @defgroup array Array
 *
 * A std::array like container.
 *
 * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
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
 * @defgroup any Any
 *
 * A std::any like container with the capacity defined at compile time.
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
 * @mainpage Butano
 *
 * Modern C++ high level GBA engine.
 *
 * This is the documentation for the Butano engine. To see a high-level overview,
 * go to the <a href="https://github.com/GValiente/butano">GitHub main page</a>.
 *
 * Thanks to the <a href="https://magnum.graphics/">Magnum Project</a> for its awesome documentation!
 * This documentation is heavily based on it.
 *
 *
 * @section main_whats_new What's new?
 *
 * Check out the \ref changelog "changelog".
 *
 *
 * @section main_getting_started Getting started
 *
 * The best way to get started is to read the
 * \ref getting_started "guide to download, install and start using Butano".
 * After that, there are various \ref examples "examples" explaining most aspects of the engine.
 * At last, check how to \ref import "import" your own assets in a game and
 * take a look at the <a href="modules.html">modules page</a>.
 *
 *
 * @section main_contact_support Contact and support
 *
 * If you spotted a bug, need a feature or have an awesome idea,
 * the best you can do for now is to head over to <a href="https://github.com/GValiente/butano">GitHub</a>.
 *
 *
 * @section main_license License
 *
 * Butano is licensed under the zlib license:
 *
 * > (C) 2020 Gustavo Valiente (gustavo.valiente@prontonmail.com)
 * >
 * > This software is provided 'as-is', without any express or implied warranty.
 * > In no event will the authors be held liable for any damages arising from the use of this software.
 * >
 * > Permission is granted to anyone to use this software for any purpose, including commercial applications,
 * > and to alter it and redistribute it freely, subject to the following restrictions:
 * >
 * > 1. The origin of this software must not be misrepresented;
 * >    you must not claim that you wrote the original software.
 * >    If you use this software in a product, an acknowledgment in the product documentation would be
 * >    appreciated but is not required.
 * >
 * > 2. Altered source versions must be plainly marked as such,
 * >    and must not be misrepresented as being the original software.
 * >
 * > 3. This notice may not be removed or altered from any source distribution.
 */


/**
 * @page getting_started Getting started
 *
 * Downloading, building and making your own games with Butano is easy and doesn't take too much time, pinky promise.
 *
 * @tableofcontents
 *
 * @section getting_started_emulator GBA emulator
 *
 * Before anything, it is convenient to have a GBA emulator at hand,
 * so you don't have to test in real hardware each change you make in your project.
 *
 * For developing GBA games, <a href="https://mgba.io/">mGBA</a>
 * and the debug version of <a href="https://problemkaputt.de/gba.htm">No$gba</a> are recommended.
 *
 *
 * @section getting_started_devkitARM devkitARM
 *
 * The next step is to download and install devkitARM
 * from <a href="https://devkitpro.org/wiki/Getting_Started">devkitPro</a>.
 *
 * Install the `GBA Development` component and make sure that you are able to build and run
 * some of the examples located in `path/to/devkitpro/examples/gba`.
 *
 * @image html devkitpro.png
 *
 *
 * @section getting_started_python Python
 *
 * To execute some of the tools needed to include image and audio files in your project,
 * you are going to need <a href="https://www.python.org/">Python</a>.
 *
 * Both Python 2 and Python 3 are supported for now, but Python 2 is not going to be supported for too long.
 * Make sure you can execute Python from the command line.
 *
 * @image html python.png
 *
 *
 * @section getting_started_butano Butano
 *
 * Download or clone the latest Butano release from <a href="https://github.com/GValiente/butano">GitHub</a>
 * and put it in a path without spaces or anything weird like that, please.
 *
 *
 * @section getting_started_examples Butano examples
 *
 * Butano contains multiple \ref examples "examples" explaining most aspects of the engine.
 *
 * Make sure that you are able to build and run some of them. They are located in `path/to/butano/examples/`.
 * To build an example, open the console, `cd` to the folder of the example you want to build and type
 * `make -j8` if your CPU has 8 cores, `make -j16` if it has 16 and so on.
 *
 * When trying to build the `sprites` example, if everything went as expected,
 * a `sprites.gba` file should have been generated in the `sprites` folder,
 * and the console should show something like this:
 *
 * @image html example_build.png
 *
 *
 * @section getting_started_template Make your own GBA game!
 *
 * If you were able to build and run some examples, congrats! Now you can make your own GBA game with Butano.
 * To do that:
 * * Copy the template project located in `path/to/butano/template/` to a path without spaces nor weird characters.
 * * Change the ROM title, the path in which Butano is located and more in the `Makefile` file.
 * * `cd` to the new folder.
 * * Type `make -j#cpu_cores#` again.
 *
 * If everything went as expected, a `*.gba` file should have been generated in the project folder!
 * Now learn how to \ref import "import your assets" in the game and
 * also take a look at the <a href="modules.html">modules page</a>.
 */


/**
 * @page examples Examples
 * @tableofcontents
 * @section examples_0_1_0 0.1.0
 * First release.
 */


/**
 * @page import Importing assets
 * @tableofcontents
 * @section import_0_1_0 0.1.0
 * First release.
 */


/**
 * @page changelog Changelog
 * @tableofcontents
 * @section changelog_0_1_0 0.1.0
 * First release.
 */


#ifdef BTN_DOXYGEN
    #ifndef BTN_CFG_ASSERT_ENABLED
        #define BTN_CFG_ASSERT_ENABLED true
    #endif

    #ifndef BTN_CFG_LOG_ENABLED
        #define BTN_CFG_LOG_ENABLED true
    #endif

    #ifndef BTN_CFG_PROFILER_ENABLED
        #define BTN_CFG_PROFILER_ENABLED true
    #endif
#endif

#endif
