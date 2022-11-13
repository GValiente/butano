/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DOCUMENTATION_H
#define BN_DOCUMENTATION_H

// NAMESPACE

/**
 * @namespace bn
 * @brief Main Butano namespace.
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
 * thanks to <a href="https://github.com/AntonioND/gbt-player">GBT Player</a>.
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
 * Keypad logging can be enabled or disabled by overloading the definition of @a BN_CFG_KEYPAD_LOG_ENABLED @a .
 *
 * Recorded key presses can be replayed later by passing the log to @a bn::core::init() @a .
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
 * It can be enabled or disabled by overloading the definition of @a BN_CFG_ASSERT_ENABLED @a .
 *
 * Note that these asserts can be used in constexpr contexts (is_constant_evaluated() returns `true`).
 */

/**
 * @defgroup log Logging
 *
 * Butano logging system.
 *
 * It allows to print information on the console of multiple emulators.
 *
 * It can be enabled or disabled by overloading the definition of @a BN_CFG_LOG_ENABLED @a .
 *
 * It supports printing on only one emulator at once.
 * The supported emulator can be changed by overloading the definition of @a BN_CFG_LOG_BACKEND @a .
 */

/**
 * @defgroup profiler Profiler
 *
 * Butano profiling system.
 *
 * It allows to measure elapsed time between code blocks defined by the user.
 *
 * It can be enabled or disabled by overloading the definition of @a BN_CFG_PROFILER_ENABLED @a .
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
 * To be part of an intrusive list, values must inherit @a bn::intrusive_list_node_type @a class.
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
 * To be part of an intrusive forward list, values must inherit @a bn::intrusive_forward_list_node_type @a class.
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
 * A generic allocator where allocation and release are not O(1) operations.
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
 * @defgroup other Other
 *
 * Stuff which doesn't fit anywhere else.
 */


// PAGES

/**
 * @mainpage Butano
 *
 * Butano is a modern C++ high level engine for the
 * <a href="https://en.wikipedia.org/wiki/Game_Boy_Advance">Game Boy Advance</a>.
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
 * Check out the @ref changelog "changelog".
 *
 *
 * @section main_getting_started Getting started
 *
 * The best way to get started is to read the
 * @ref getting_started "guide to download, install and start using Butano".
 * After that, there are various @ref examples "examples" explaining most aspects of the engine.
 * At last, check how to @ref import "import" your own assets in a game and
 * take a look at the <a href="modules.html">modules</a> page.
 * The @ref faq "frequently asked questions (FAQ)" are worth a look too.
 *
 *
 * @section main_contact_support Contact and support
 *
 * If you spotted a bug, need a feature or have an awesome idea,
 * the best you can do is to head over to <a href="https://github.com/GValiente/butano">GitHub</a>
 * and to the <a href="https://discord.io/gbadev">gbadev Discord server</a>.
 *
 *
 * @section main_licenses Licenses
 *
 * Butano is licensed under the zlib license,
 * see the <a href="https://github.com/GValiente/butano/blob/master/LICENSE">LICENSE</a> file for details.
 *
 * Third party libraries are licensed under other licenses,
 * please check the <a href="https://github.com/GValiente/butano/blob/master/licenses">licenses</a> folder for details.
 *
 * Assets used in games, examples and other projects are licensed under other licenses,
 * please check the `credits` folders for details.
 */


/**
 * @page getting_started Getting started
 *
 * Downloading, building and making your own games with Butano is easy and doesn't take too much time, pinky promise.
 *
 * @tableofcontents
 *
 * @section getting_started_supported_platforms Supported platforms
 *
 * Butano is built on top of the <a href="https://devkitpro.org/">devkitARM</a> toolchain,
 * so it supports Windows, macOS and Unix-like platforms.
 *
 * @section getting_started_emulator GBA emulator
 *
 * Before anything, it is convenient to have a GBA emulator at hand,
 * so you don't have to test in real hardware each change you make in your project.
 *
 * For developing GBA games, <a href="https://mgba.io">mGBA</a>
 * and the debug version of <a href="https://problemkaputt.de/gba.htm">No$gba</a> are recommended.
 *
 *
 * @section getting_started_devkitARM devkitARM
 *
 * The next step is to download and install devkitARM
 * from <a href="https://devkitpro.org/wiki/Getting_Started">devkitPro</a>:
 * * Install the `GBA Development` component.
 * * Install **all members** in the `gba-dev` group if the installer asks for which members to install.
 * * Make sure that **with the console** you are able to build and run some of the examples
 *   located in `/path/to/devkitpro/examples/gba`:
 *   * By default, devkitARM is installed in `C:\devkitPro\` on Windows and in `/opt/devkitpro/` on Ubuntu.
 *   * To build a devkitARM example open the console, `cd` to the folder of the example you want to build
 *     and type `make -j8` if your CPU has 8 cores, `make -j16` if it has 16 and so on.
 *   * In particular, you should build and run the Maxmod example located in
 *     `/path/to/devkitpro/examples/gba/audio/maxmod` before going on.
 *
 * @image html devkitpro.png
 *
 * @image html devkitpro_ubuntu.png
 *
 *
 * @section getting_started_python Python
 *
 * To execute some of the tools needed to include image and audio files in your project,
 * you are going to need <a href="https://www.python.org/">Python</a>.
 *
 * If you are going to install Python on Windows,
 * remember to add it to the PATH and to close all command line windows before installing it:
 *
 * @image html python_windows.png
 *
 * Both Python 2 and Python 3 are supported for now, but Python 2 is not going to be supported for too long.
 * Make sure you can execute Python from the command line:
 *
 * @image html python.png
 *
 * @image html python_ubuntu.png
 *
 * If you have an Ubuntu/Debian based system with `python3` instead of `python`, you can:
 * * Specify `python3` as the Python interpreter path in the `Makefile` of the project you are trying to build.
 * * Install `python-is-python3`: `sudo apt-get install python-is-python3`.
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
 * Butano contains multiple @ref examples "examples" of most aspects of the engine.
 *
 * Make sure that you are able to build and run some of them:
 * * They are located in `/path/to/butano/examples/`.
 * * As with <a href="https://devkitpro.org/">devkitARM</a> examples, to build a Butano example open the console,
 *   `cd` to the folder of the example you want to build and type `make -j8` if your CPU has 8 cores,
 *   `make -j16` if it has 16 and so on.
 *
 * When trying to build the `sprites` example, if everything went as expected,
 * a `sprites.gba` file should have been generated in the `sprites` folder,
 * and the console should show something like this:
 *
 * @image html example_build.png
 *
 * @image html example_build_ubuntu.png
 *
 *
 * @section getting_started_template Make your own GBA game!
 *
 * If you were able to build and run some examples, congrats! Now you can make your own GBA game with Butano.
 * To do that:
 * * Copy the template project located in `/path/to/butano/template/` to a path without spaces nor weird characters.
 * * Change in the `Makefile` the path in which Butano library folder is located.
 *   For example, if Butano is located in `/path/to/butano/`,
 *   `LIBBUTANO` variable should be set to `/path/to/butano/butano`.
 * * If you want, you can also change ROM title, ROM code and more in the `Makefile`.
 * * `cd` to the new folder.
 * * Type `make -j#cpu_cores#` again.
 *
 * If everything went as expected, a `*.gba` file should have been generated in the project folder!
 * Now learn how to @ref import "import your assets" in the game and
 * also take a look at the <a href="modules.html">modules</a> page.
 * The @ref faq "frequently asked questions (FAQ)" are worth a look too.
 */


/**
 * @page examples Examples
 *
 * Learn by example to get quickly up to speed.
 *
 * The examples assume that you have at least basic knowledge about:
 * * Modern C++ (Butano is based on modern C++ concepts like shared ownership, RAII, standard containers...).
 * * Old school 2D graphics (what are sprites, backgrounds, color palettes...).
 * * Basics of retro game development (at least to know that floating point usage is a no-no and things like that).
 *
 * Deep knowledge of GBA hardware is not required, as Butano wraps most of the functionality in its own classes.
 *
 * As always, to understand how the little machine works, <a href="https://www.coranac.com/tonc/text/toc.htm">Tonc</a>
 * and <a href="http://problemkaputt.de/gbatek.htm">GBATEK</a> are your friends.
 *
 * Butano brings multiple examples of most aspects of the engine in `/path/to/butano/examples/`.
 * Each example project contains multiple mini examples showcasing how easy is to work with Butano.
 *
 * The included example projects are the following:
 *
 * <table>
 *
 * <tr><td> @ref affine_bg
 *
 * @image html examples_affine_bgs.png
 *
 * Demonstrates usage of backgrounds with affine transformations.
 *
 * <td> @ref audio
 *
 * @image html examples_audio.png
 *
 * Shows how to play Direct Sound music and sound effects with Butano.
 *
 * <td> **DMG audio**
 *
 * @image html examples_dmg_audio.png
 *
 * Simple DMG music playback example.
 *
 * <tr><td> **Audio player**
 *
 * @image html examples_audio_player.png
 *
 * Basic audio player example.
 *
 * <td> @ref blending
 *
 * @image html examples_blending.png
 *
 * Demonstrates usage of transparency, intensity and fade blending.
 *
 * <td> @ref camera
 *
 * @image html examples_cameras.png
 *
 * Shows how a camera affects the displayed elements.
 *
 * <tr><td> @ref core
 *
 * @image html examples_core.png
 *
 * Shows how to sleep and reset the GBA among other things.
 *
 * <td> **Dynamic affine background**
 *
 * @image html examples_dynamic_affine_bg.png
 *
 * Basic dynamic affine background example.
 *
 * <td> **Dynamic regular background**
 *
 * @image html examples_dynamic_regular_bg.png
 *
 * Basic dynamic regular background example.
 *
 * <tr><td> **External tool**
 *
 * @image html examples_external_tool.png
 *
 * Shows how to call an external tool from the project's `Makefile`.
 *
 * <td> @ref green_swap
 *
 * @image html examples_green_swap.png
 *
 * See how it looks this undocumented graphic effect.
 *
 * <td> @ref keypad
 *
 * @image html examples_keypad.png
 *
 * Simple keypad status demo.
 *
 * <tr><td> @ref link
 *
 * @image html examples_link.png
 *
 * Multiplayer communication through a link cable.
 *
 * <td> @ref log
 *
 * @image html examples_log.png
 *
 * Integration of emulator logging into Butano.
 *
 * <td> **Mode 7**
 *
 * @image html examples_mode_7.png
 *
 * Basic SNES mode 7 like background example.
 *
 * <tr><td> @ref mosaic
 *
 * @image html examples_mosaic.png
 *
 * Sprites and backgrounds mosaic effects.
 *
 * <td> @ref palette
 *
 * @image html examples_palettes.png
 *
 * Showcases multiple color palette effects.
 *
 * <td> **Flat polygons**
 *
 * @image html examples_polygons.png
 *
 * Showcases flat polygons rendering with sprites.
 *
 * <tr><td> @ref profiler
 *
 * @image html examples_profiler.png
 *
 * Basic code blocks profiling.
 *
 * <td> @ref regular_bg
 *
 * @image html examples_regular_bgs.png
 *
 * Shows how to use regular backgrounds with Butano.
 *
 * <td> @ref rumble
 *
 * @image html examples_rumble.png
 *
 * Vibration showcase (if your cart supports it).
 *
 * <tr><td> @ref affine_mat "Sprite affine matrices"
 *
 * @image html examples_sprite_affine_mats.png
 *
 * Demonstrates usage of one affine matrix for multiple sprites.
 *
 * <td> @ref sprite
 *
 * @image html examples_sprites.png
 *
 * Simple sprites examples.
 *
 * <td> @ref sram
 *
 * @image html examples_sram.png
 *
 * SRAM reads and writes with Butano.
 *
 * <tr><td> @ref text
 *
 * @image html examples_text.png
 *
 * Showcase of Butano sprite text rendering.
 *
 * <td> **Textured polygons**
 *
 * @image html examples_texture_polygons.png
 *
 * Showcases textured polygons rendering with sprites.
 *
 * <td> @ref window
 *
 * @image html examples_windows.png
 *
 * Demonstrates usage of rectangle and sprite windows.
 *
 * <tr><td> **World map**
 *
 * @image html examples_world_map.png
 *
 * Showcase of a 4096x4096 world map with a perspective effect.
 *
 * </table>
 */


/**
 * @page import Importing assets
 *
 * @tableofcontents
 *
 * Importing your graphic and audio files into a GBA game can be annoying,
 * but with Butano and this guide you will be good to go.
 *
 * GBA ROMs by themselves don't include a file system, so you can't put a couple of `*.bmp` files into a folder
 * and expect to read them directly from the GBA side.
 *
 * This means that all the game's data has to be added directly to the binary.
 * Don't worry, because Butano build system does this for you.
 * When you drop a file into an assets folder, Butano:
 * * Generates a GBA-friendly version of it.
 * * Inserts it into the ROM.
 * * Creates a C++ header into the `build` folder containing the required information to use the assets with ease.
 *
 * Let's see how to import image and audio files.
 *
 *
 * @section import_image Images
 *
 * By default image files go into the `graphics` folder of your project.
 *
 * Butano for now is a little finicky about the images it likes, sorry.
 *
 * The required image format is the following:
 * * BMP without compression nor color space information.
 * * 16 or 256 colors only.
 * * The first color in the color palette is the transparent one, so in most cases it will not be shown on screen.
 *
 * If you are using <a href="https://www.gimp.org/">GIMP</a> for your images,
 * remember to disable color space information:
 *
 * @image html import_gimp.png
 *
 * However, the recommended tool to ensure that your images are compatible
 * with Butano is <a href="https://www.coranac.com/projects/usenti/">Usenti</a>:
 *
 * @image html import_usenti.png
 *
 * Usenti is a simple bitmap editor for paletted images,
 * it is like the good old Paint but with various palette/color manipulation tools.
 *
 * Any program that can generate indexed `*.bmp` files without weird headers should work. However:
 * * Usenti works with 15BPP (bits per pixel) colors, like the GBA and unlike most other popular image editors,
 *   which work with 24BPP or 32BPP colors. BPP reduction destroys some color gradients,
 *   so the images generated by Usenti are closer to what a GBA can display.
 * * Usenti is powered by <a href="https://www.coranac.com/projects/grit/">grit</a>, the tool used by Butano
 *   to convert images to GBA friendly data. grit doesn't support some `*.bmp` features like
 *   compression or some weird headers, so if Usenti displays a `*.bmp` file without issues,
 *   grit should be able to process it successfully.
 *
 * If you are not going to use Usenti for your images, at least try to check them with it
 * when they are not displayed as expected.
 *
 * A single `*.bmp` file is not enough to display graphics on the GBA.
 * You must accompany it with a `*.json` file with the same name specifying if it is a sprite or a background
 * and some more info.
 *
 * Let's see how to do it.
 *
 *
 * @subsection import_sprite Sprites
 *
 * An image file can contain multiple sprite images.
 * If it only contains one sprite image, its size must be one of the specified by @ref bn::sprite_shape_size.
 *
 * Multiple sprite images are allowed by layering them down on the vertical axis:
 *
 * @image html import_sprite.png
 *
 * An example of the `*.json` files required for sprites is the following:
 *
 * @code{.json}
 * {
 *     "type": "sprite",
 *     "height": 64
 * }
 * @endcode
 *
 * The fields for sprites are the following:
 * * `"type"`: must be `"sprite"` for sprites.
 * * `"height"`: height of each sprite image in pixels.
 * For example, if the specified height is 32, an image with 128 pixels of height contains 4 sprite images.
 * * `"tiles_compression"`: optional field which specifies the compression of the tiles data:
 *   * `"none"`: uncompressed data (this is the default option).
 *   * `"lz77"`: LZ77 compressed data.
 *   * `"run_length"`: run-length compressed data.
 *   * `"huffman"`: Huffman compressed data.
 *   * `"auto"`: uses the option which gives the smallest data size.
 * * `"palette_compression"`: optional field which specifies the compression of the colors data:
 *   * `"none"`: uncompressed data (this is the default option).
 *   * `"lz77"`: LZ77 compressed data.
 *   * `"run_length"`: run-length compressed data.
 *   * `"huffman"`: Huffman compressed data.
 *   * `"auto"`: uses the option which gives the smallest data size.
 * * `"compression"`: optional field which specifies the compression of the tiles and the colors data:
 *   * `"none"`: uncompressed data (this is the default option).
 *   * `"lz77"`: LZ77 compressed data.
 *   * `"run_length"`: run-length compressed data.
 *   * `"huffman"`: Huffman compressed data.
 *   * `"auto"`: uses the option which gives the smallest data size.
 *
 * If the conversion process has finished successfully,
 * a bn::sprite_item should have been generated in the `build` folder.
 *
 * For example, from two files named `image.bmp` and `image.json`,
 * a header file named `bn_sprite_items_image.h` is generated in the `build` folder.
 *
 * You can use this header to create a sprite with only one line of C++ code:
 *
 * @code{.cpp}
 * #include "bn_sprite_items_image.h"
 *
 * bn::sprite_ptr sprite = bn::sprite_items::image.create_sprite(0, 0);
 * @endcode
 *
 *
 * @subsection import_sprite_tiles Sprite tiles
 *
 * An image file can contain multiple sprite tiles sets.
 * If it only contains one sprite tiles set,
 * the size of the image must be one of the specified by @ref bn::sprite_shape_size.
 *
 * Multiple sprite tiles sets are allowed by layering them down on the vertical axis:
 *
 * @image html import_sprite.png
 *
 * An example of the `*.json` files required for sprite tiles is the following:
 *
 * @code{.json}
 * {
 *     "type": "sprite_tiles",
 *     "height": 64
 * }
 * @endcode
 *
 * The fields for sprite tiles are the following:
 * * `"type"`: must be `"sprite_tiles"` for sprite tiles.
 * * `"height"`: height of each sprite tiles set in pixels.
 * For example, if the specified height is 32, an image with 128 pixels of height contains 4 sprite tiles sets.
 * * `"bpp_mode"`: optional field which specifies the bits per pixel of the sprite tiles:
 *   * `"bpp_8"`: up to 256 colors per @ref tile "tile".
 *   * `"bpp_4"`: up to 16 colors per @ref tile "tile".
 * * `"compression"`: optional field which specifies the compression of the tiles data:
 *   * `"none"`: uncompressed data (this is the default option).
 *   * `"lz77"`: LZ77 compressed data.
 *   * `"run_length"`: run-length compressed data.
 *   * `"huffman"`: Huffman compressed data.
 *   * `"auto"`: uses the option which gives the smallest data size.
 *
 * If the conversion process has finished successfully,
 * a bn::sprite_tiles_item should have been generated in the `build` folder.
 *
 * For example, from two files named `image.bmp` and `image.json`,
 * a header file named `bn_sprite_tiles_items_image.h` is generated in the `build` folder.
 *
 * You can use this header to create sprite tiles with only one line of C++ code:
 *
 * @code{.cpp}
 * #include "bn_sprite_tiles_items_image.h"
 *
 * bn::sprite_tiles_ptr sprite_tiles = bn::sprite_tiles_items::image.create_tiles();
 * @endcode
 *
 *
 * @subsection import_sprite_palette Sprite palettes
 *
 * An example of the `*.json` files required for sprite palettes is the following:
 *
 * @code{.json}
 * {
 *     "type": "sprite_palette"
 * }
 * @endcode
 *
 * The fields for sprite palettes are the following:
 * * `"type"`: must be `"sprite_palette"` for sprites.
 * * `"colors_count"`: optional field which specifies the palette size [1..256].
 * * `"compression"`: optional field which specifies the compression of the colors data:
 *   * `"none"`: uncompressed data (this is the default option).
 *   * `"lz77"`: LZ77 compressed data.
 *   * `"run_length"`: run-length compressed data.
 *   * `"huffman"`: Huffman compressed data.
 *   * `"auto"`: uses the option which gives the smallest data size.
 *
 * If the conversion process has finished successfully,
 * a bn::sprite_palette_item should have been generated in the `build` folder.
 *
 * For example, from two files named `image.bmp` and `image.json`,
 * a header file named `bn_sprite_palette_items_image.h` is generated in the `build` folder.
 *
 * You can use this header to create a sprite palette with only one line of C++ code:
 *
 * @code{.cpp}
 * #include "bn_sprite_palette_items_image.h"
 *
 * bn::sprite_palette_ptr sprite_palette = bn::sprite_palette_items::image.create_palette();
 * @endcode
 *
 *
 * @subsection import_regular_bg Regular backgrounds
 *
 * An image file can contain only one regular background.
 * The size of a small regular background (which are faster) must be 256x256, 256x512, 512x256 or 512x512 pixels.
 * Big regular backgrounds are slower CPU wise and don't support wrapping (they can't be moved beyond their boundaries),
 * but can have any width or height multiple of 256 pixels.
 *
 * An example of the `*.json` files required for regular backgrounds is the following:
 *
 * @code{.json}
 * {
 *     "type": "regular_bg"
 * }
 * @endcode
 *
 * The fields for regular backgrounds are the following:
 * * `"type"`: must be `"regular_bg"` for regular backgrounds.
 * * `"palette_item"`: optional field which specifies the name of the bn::bg_palette_item to use for this background.
 * * `"bpp_mode"`: optional field which specifies the bits per pixel of the regular background.
 * This field is required if an external bn::bg_palette_item is referenced with `"palette_item"`:
 *   * `"bpp_8"`: up to 256 colors per @ref tile "tile".
 *   * `"bpp_4_auto"`: up to 16 colors per @ref tile "tile".
 * Butano tries to quantize the image to fit the color palette into the required one.
 * It is not supported if an external bn::bg_palette_item is referenced with `"palette_item"`.
 *   * `"bpp_4_manual"`: up to 16 colors per @ref tile "tile".
 * Butano expects that the image color palette is already valid for this mode.
 *   * `"bpp_4"`: `"bpp_4_manual"` alias.
 *
 * The default is `"bpp_4_manual"` for 16 color images and `"bpp_8"` for 256 color images.
 * * `"repeated_tiles_reduction"`: optional field which specifies if repeated tiles must be reduced or not
 * (`true` by default).
 * * `"flipped_tiles_reduction"`: optional field which specifies if flipped tiles must be reduced or not
 * (`true` by default).
 * * `"tiles_compression"`: optional field which specifies the compression of the tiles data:
 *   * `"none"`: uncompressed data (this is the default option).
 *   * `"lz77"`: LZ77 compressed data.
 *   * `"run_length"`: run-length compressed data.
 *   * `"huffman"`: Huffman compressed data.
 *   * `"auto"`: uses the option which gives the smallest data size.
 * * `"palette_compression"`: optional field which specifies the compression of the colors data:
 *   * `"none"`: uncompressed data (this is the default option).
 *   * `"lz77"`: LZ77 compressed data.
 *   * `"run_length"`: run-length compressed data.
 *   * `"huffman"`: Huffman compressed data.
 *   * `"auto"`: uses the option which gives the smallest data size.
 * * `"map_compression"`: optional field which specifies the compression of the map data:
 *   * `"none"`: uncompressed data (this is the default option).
 *   * `"lz77"`: LZ77 compressed data.
 *   * `"run_length"`: run-length compressed data.
 *   * `"huffman"`: Huffman compressed data.
 *   * `"auto"`: uses the option which gives the smallest data size.
 * * `"compression"`: optional field which specifies the compression of the tiles, the colors and the map data:
 *   * `"none"`: uncompressed data (this is the default option).
 *   * `"lz77"`: LZ77 compressed data.
 *   * `"run_length"`: run-length compressed data.
 *   * `"huffman"`: Huffman compressed data.
 *   * `"auto"`: uses the option which gives the smallest data size.
 *
 * If the conversion process has finished successfully,
 * a bn::regular_bg_item should have been generated in the `build` folder.
 *
 * For example, from two files named `image.bmp` and `image.json`,
 * a header file named `bn_regular_bg_items_image.h` is generated in the `build` folder.
 *
 * You can use this header to create a regular background with only one line of C++ code:
 *
 * @code{.cpp}
 * #include "bn_regular_bg_items_image.h"
 *
 * bn::regular_bg_ptr regular_bg = bn::regular_bg_items::image.create_bg(0, 0);
 * @endcode
 *
 *
 * @subsection import_regular_bg_tiles Regular background tiles
 *
 * An image file can contain up to 1024 regular background tiles.
 *
 * An example of the `*.json` files required for regular background tiles is the following:
 *
 * @code{.json}
 * {
 *     "type": "regular_bg_tiles",
 *     "bpp_mode": "bpp_8"
 * }
 * @endcode
 *
 * The fields for regular background tiles are the following:
 * * `"type"`: must be `"regular_bg_tiles"` for regular background tiles.
 * * `"bpp_mode"`: specifies the bits per pixel of the regular background tiles:
 *   * `"bpp_8"`: up to 256 colors per @ref tile "tile".
 *   * `"bpp_4"`: up to 16 colors per @ref tile "tile".
 * Butano expects that the image color palette is already valid for this mode.
 * * `"compression"`: optional field which specifies the compression of the tiles data:
 *   * `"none"`: uncompressed data (this is the default option).
 *   * `"lz77"`: LZ77 compressed data.
 *   * `"run_length"`: run-length compressed data.
 *   * `"huffman"`: Huffman compressed data.
 *   * `"auto"`: uses the option which gives the smallest data size.
 *
 * If the conversion process has finished successfully,
 * a bn::regular_bg_tiles_item should have been generated in the `build` folder.
 *
 * For example, from two files named `image.bmp` and `image.json`,
 * a header file named `bn_regular_bg_tiles_items_image.h` is generated in the `build` folder.
 *
 * You can use this header to create regular background tiles with only one line of C++ code:
 *
 * @code{.cpp}
 * #include "bn_regular_bg_tiles_items_image.h"
 *
 * bn::regular_bg_tiles_ptr regular_bg_tiles = bn::regular_bg_tiles_items::image.create_tiles();
 * @endcode
 *
 *
 * @subsection import_affine_bg Affine backgrounds
 *
 * An image file can contain only one affine background.
 * The size of a small affine background (which are faster) must be 128x128, 256x256, 512x512 or 1024x1024 pixels.
 * Big affine backgrounds are slower CPU wise and don't support wrapping (they can't be moved beyond their boundaries),
 * but can have any width or height multiple of 256 pixels.
 *
 * An example of the `*.json` files required for affine backgrounds is the following:
 *
 * @code{.json}
 * {
 *     "type": "affine_bg"
 * }
 * @endcode
 *
 * The fields for affine backgrounds are the following:
 * * `"type"`: must be `"affine_bg"` for affine backgrounds.
 * * `"palette_item"`: optional field which specifies the name of the bn::bg_palette_item to use for this background.
 * * `"repeated_tiles_reduction"`: optional field which specifies if repeated tiles must be reduced or not
 * (`true` by default).
 * * `"tiles_compression"`: optional field which specifies the compression of the tiles data:
 *   * `"none"`: uncompressed data (this is the default option).
 *   * `"lz77"`: LZ77 compressed data.
 *   * `"run_length"`: run-length compressed data.
 *   * `"huffman"`: Huffman compressed data.
 *   * `"auto"`: uses the option which gives the smallest data size.
 * * `"palette_compression"`: optional field which specifies the compression of the colors data:
 *   * `"none"`: uncompressed data (this is the default option).
 *   * `"lz77"`: LZ77 compressed data.
 *   * `"run_length"`: run-length compressed data.
 *   * `"huffman"`: Huffman compressed data.
 *   * `"auto"`: uses the option which gives the smallest data size.
 * * `"map_compression"`: optional field which specifies the compression of the map data:
 *   * `"none"`: uncompressed data (this is the default option).
 *   * `"lz77"`: LZ77 compressed data.
 *   * `"run_length"`: run-length compressed data.
 *   * `"huffman"`: Huffman compressed data.
 *   * `"auto"`: uses the option which gives the smallest data size.
 * * `"compression"`: optional field which specifies the compression of the tiles, the colors and the map data:
 *   * `"none"`: uncompressed data (this is the default option).
 *   * `"lz77"`: LZ77 compressed data.
 *   * `"run_length"`: run-length compressed data.
 *   * `"huffman"`: Huffman compressed data.
 *   * `"auto"`: uses the option which gives the smallest data size.
 *
 * If the conversion process has finished successfully,
 * a bn::affine_bg_item should have been generated in the `build` folder.
 *
 * For example, from two files named `image.bmp` and `image.json`,
 * a header file named `bn_affine_bg_items_image.h` is generated in the `build` folder.
 *
 * You can use this header to create an affine background with only one line of C++ code:
 *
 * @code{.cpp}
 * #include "bn_affine_bg_items_image.h"
 *
 * bn::affine_bg_ptr affine_bg = bn::affine_bg_items::image.create_bg(0, 0);
 * @endcode
 *
 *
 * @subsection import_affine_bg_tiles Affine background tiles
 *
 * An image file can contain up to 256 affine background tiles.
 *
 * An example of the `*.json` files required for affine background tiles is the following:
 *
 * @code{.json}
 * {
 *     "type": "affine_bg_tiles"
 * }
 * @endcode
 *
 * The fields for affine background tiles are the following:
 * * `"type"`: must be `"affine_bg_tiles"` for affine background tiles.
 * * `"compression"`: optional field which specifies the compression of the tiles data:
 *   * `"none"`: uncompressed data (this is the default option).
 *   * `"lz77"`: LZ77 compressed data.
 *   * `"run_length"`: run-length compressed data.
 *   * `"huffman"`: Huffman compressed data.
 *   * `"auto"`: uses the option which gives the smallest data size.
 *
 * If the conversion process has finished successfully,
 * a bn::affine_bg_tiles_item should have been generated in the `build` folder.
 *
 * For example, from two files named `image.bmp` and `image.json`,
 * a header file named `bn_affine_bg_tiles_items_image.h` is generated in the `build` folder.
 *
 * You can use this header to create affine background tiles with only one line of C++ code:
 *
 * @code{.cpp}
 * #include "bn_affine_bg_tiles_items_image.h"
 *
 * bn::affine_bg_tiles_ptr affine_bg_tiles = bn::affine_bg_tiles_items::image.create_tiles();
 * @endcode
 *
 *
 * @subsection import_bg_palette Background palettes
 *
 * An example of the `*.json` files required for background palettes is the following:
 *
 * @code{.json}
 * {
 *     "type": "bg_palette",
 *     "bpp_mode": "bpp_8"
 * }
 * @endcode
 *
 * The fields for background palettes are the following:
 * * `"type"`: must be `"bg_palette"` for background palettes.
 * * `"bpp_mode"`: specifies the bits per pixel of the background palette:
 *   * `"bpp_8"`: up to 256 colors per @ref tile "tile".
 *   * `"bpp_4"`: up to 16 colors per @ref tile "tile".
 * * `"colors_count"`: optional field which specifies the palette size [1..256].
 * * `"compression"`: optional field which specifies the compression of the tiles and the colors data:
 *   * `"none"`: uncompressed data (this is the default option).
 *   * `"lz77"`: LZ77 compressed data.
 *   * `"run_length"`: run-length compressed data.
 *   * `"huffman"`: Huffman compressed data.
 *   * `"auto"`: uses the option which gives the smallest data size.
 *
 * If the conversion process has finished successfully,
 * a bn::bg_palette_item should have been generated in the `build` folder.
 *
 * For example, from two files named `image.bmp` and `image.json`,
 * a header file named `bn_bg_palette_items_image.h` is generated in the `build` folder.
 *
 * You can use this header to create a background palette with only one line of C++ code:
 *
 * @code{.cpp}
 * #include "bn_bg_palette_items_image.h"
 *
 * bn::bg_palette_ptr bg_palette = bn::bg_palette_items::image.create_palette();
 * @endcode
 *
 *
 * @section import_audio Audio
 *
 * By default audio files played with Direct Sound channels go into the `audio` folder of your project,
 * and music files played with DMG channels go into the `dmg_audio` folder.
 *
 * Direct Sound stuff is managed by the awesome <a href="https://maxmod.devkitpro.org/">Maxmod</a>,
 * so if you have an issue with Direct Sound music or with sound effects, well, you know.
 *
 * DMG channels are handled by the also impressive <a href="https://github.com/AntonioND/gbt-player">GBT Player</a>,
 * so if you have issues with DMG music, you know where to go.
 *
 * A really nice application for editing audio files
 * before importing them into your game is <a href="https://openmpt.org/">OpenMPT</a>.
 *
 *
 * @subsection import_direct_sound_music Direct Sound music
 *
 * The required format for Direct Sound music is module files with `*.mod`, `*.xm`, `*.s3m` and `*.it` extensions.
 *
 * By default Butano supports up to 16 music channels,
 * but this limit can be increased by overloading the definition of @ref BN_CFG_AUDIO_MAX_MUSIC_CHANNELS.
 *
 * However, if it is possible don't do this, don't make the poor GBA suffer.
 *
 * If the conversion process has finished successfully,
 * a bunch of bn::music_item objects under the `bn::music_items` namespace
 * should have been generated in the `build` folder for all music files.
 * You can use these items to play music with only one line of C++ code:
 *
 * @code{.cpp}
 * #include "bn_music_items.h"
 *
 * bn::music_items::song.play();
 * @endcode
 *
 *
 * @subsection import_dmg_music DMG music
 *
 * The required format for DMG music is module files with `*.mod` and `*.s3m` extensions.
 *
 * If the conversion process has finished successfully,
 * a bn::dmg_music_item should have been generated in the `build` folder.
 *
 * For example, from a file named `module.mod`,
 * a header file named `bn_dmg_music_items_module.h` is generated in the `build` folder.
 *
 * You can use this header to play the module file with only one line of C++ code:
 *
 * @code{.cpp}
 * #include "bn_dmg_music_items_module.h"
 *
 * bn::dmg_music_items::module.play();
 * @endcode
 *
 *
 * @subsection import_sound Sound effects
 *
 * The required format for sound effects is waveform audio files (files with `*.wav` extension)
 * without compression or anything weird. Besides, *I think* stereo files are not allowed.
 *
 * The recommended quality for sound effects is 8-bits 22050 Hz.
 *
 * If the conversion process has finished successfully,
 * a bunch of bn::sound_item objects under the `bn::sound_items` namespace
 * should have been generated in the `build` folder for all sound files.
 * You can use these items to play sound effects with only one line of C++ code:
 *
 * @code{.cpp}
 * #include "bn_sound_items.h"
 *
 * bn::sound_items::sfx.play();
 * @endcode
 */


/**
 * @page qt_creator Qt Creator setup guide
 *
 * @tableofcontents
 *
 * Qt Creator is a cross-platform integrated development environment (IDE) built for the maximum developer experience.
 * Qt Creator runs on Windows, Linux, and macOS desktop operating systems,
 * and allows developers to create applications across desktop, mobile, and embedded platforms.
 *
 * Since it has lots of C++ advanced features for a free IDE, it is a good fit for Butano projects.
 *
 * This is a small tutorial for setting up Butano template project in Qt Creator 4.14.
 * Although the guide has been made in Windows, the steps for other operating systems are similar if not the same.
 *
 *
 * @section qt_creator_getting_started Getting started
 *
 * Before messing with Qt Creator, make sure that you are able to build and run Butano projects from the command line.
 * The best way to achieve it is to read the
 * @ref getting_started "guide to download, install and start using Butano".
 *
 *
 * @section qt_creator_install Install Qt Creator
 *
 * If you don't have Qt Creator installed yet, you can get it from <a href="https://www.qt.io/offline-installers">here</a>.
 *
 * @image html qt_creator/qt_0.png
 *
 *
 * @section qt_creator_compilers Compilers
 *
 * Once you have Qt Creator installed, add <a href="https://devkitpro.org/">devkitARM</a>
 * GCC and G++ compilers from `Tools -> Options -> Kits -> Compilers`:
 *
 * @image html qt_creator/qt_1.png
 *
 * @image html qt_creator/qt_2.png
 *
 *
 * @section qt_creator_kit Kit
 *
 * Add a new kit with the previous <a href="https://devkitpro.org/">devkitARM</a> compilers from
 * `Tools -> Options -> Kits -> Kits`:
 *
 * @image html qt_creator/qt_3.png
 *
 * You can ignore the yellow triangle: it is a warning, not an error.
 *
 *
 * @section qt_creator_import Import template project
 *
 * Now that you have the kit setup, import Butano template project from `File -> New File or Project...`:
 *
 * @image html qt_creator/qt_4.png
 *
 * @image html qt_creator/qt_5.png
 *
 * @image html qt_creator/qt_6.png
 *
 * @image html qt_creator/qt_7.png
 *
 *
 * @section qt_creator_cpp_20 C++20
 *
 * Enable C++20 support in the `template.cxxflags` file with the following line:
 *
 * @code{.cpp}
 * -std=c++20
 * @endcode
 *
 *
 * @section qt_creator_include_paths Include paths
 *
 * Add the following paths to the `template.includes` file:
 *
 * @code{.cpp}
 * include
 * build
 * ../butano/include
 * @endcode
 *
 *
 * @section qt_creator_build_settings Build settings
 *
 * Disable `all` build target from `Projects -> Build Settings -> Build Steps`:
 *
 * @image html qt_creator/qt_8.png
 *
 *
 * @section qt_creator_run_settings Run settings
 *
 * Add your favorite GBA emulator from `Projects -> Run Settings -> Run`:
 *
 * @image html qt_creator/qt_9.png
 *
 *
 * @section qt_creator_build_and_run Build and run
 *
 * Finally, build and run the template project by clicking at the bottom left arrow (the one without the roach please).
 *
 * If everything went as expected, your GBA emulator of choice should show a nice black screen. Congrats!
 */


/**
 * @page faq Frequently asked questions (FAQ)
 *
 * @tableofcontents
 *
 *
 * @section faq_general General
 *
 *
 * @subsection faq_how_do_i How do I-
 *
 * The best way to get started is to read the
 * @ref getting_started "guide to download, install and start using Butano".
 *
 * After that, there are various @ref examples "examples" explaining most aspects of the engine.
 *
 * At last, check how to @ref import "import" your own assets in a game and
 * take a look at the <a href="modules.html">modules</a> page.
 *
 * This page is worth a look too.
 *
 *
 * @subsection faq_sell_money Would I be able to sell my game made with Butano for money?
 *
 * Sure!
 *
 * If you comply with <a href="https://github.com/GValiente/butano/blob/master/LICENSE">Butano license</a>
 * and <a href="https://github.com/GValiente/butano/tree/master/3rd_party_licenses">third party libraries licenses</a>
 * used by Butano you can sell your game without issues.
 *
 *
 * @section faq_programming Programming
 *
 *
 * @subsection faq_cpp Can I use Butano without knowing C++?
 *
 * While Butano is easy to use if you have some C++ skills,
 * it is going to be hard to work with it if you don't know nothing about C++,
 * so you should learn a bit of C++ before diving into Butano.
 *
 * After that, maybe you can learn both C++ and Butano at the same time while you make a game,
 * but if you don't want to have a hard time with Butano you should know the basics of C++ before messing with it.
 *
 *
 * @subsection faq_ide Is there any good free IDE for Butano projects out there?
 *
 * Take a look at the @ref qt_creator.
 *
 *
 * @subsection faq_incomplete_type Why I get an incomplete type error when trying to use a Butano class?
 *
 * If you get an error message like these:
 *
 * @code{.cpp}
 * error: variable 'bn::regular_bg_ptr bg' has initializer but incomplete type
 * error: invalid use of incomplete type 'class bn::regular_bg_ptr'
 * error: aggregate 'bn::fixed' has incomplete type and cannot be defined
 * error: static assertion failed: template argument must be a complete class or an unbounded array
 * @endcode
 *
 * It is almost always because of lack of included headers.
 *
 * You can find the header of each Butano class in its documentation page.
 *
 *
 * @subsection faq_common_folder Why can't I use the content of the common folder in a new project?
 *
 * The `common` folder contains code and assets used by most Butano examples and tests.
 *
 * It is not part of Butano (the library), so it is not included by default in new projects.
 *
 * As the content of the `common` folder could change in future Butano versions,
 * you should copy its content to your project if you want to use it.
 *
 *
 * @subsection faq_error_after_update Why I get undefined reference errors after updating Butano?
 *
 * If after updating Butano you get error messages like this:
 *
 * @code{.cpp}
 * error: undefined reference to 'bn::reciprocal_lut_16'
 * @endcode
 *
 * They usually go away with a full rebuild of your project (`make clean` before `make`).
 *
 * Also make sure you update <a href="https://devkitpro.org/">devkitARM</a> when you update Butano,
 * since some Butano features don't work with older devkitARM releases.
 *
 *
 * @subsection faq_containers Why there's std like containers included with Butano?
 *
 * Butano containers differ from the standard library ones in two important points:
 * * They don't use the heap, their content is always on the stack.
 * * They don't throw exceptions. @ref assert are used instead.
 *
 * Since avoiding heap usage and exceptions is usually good for GBA development,
 * use Butano containers whenever possible.
 *
 * Keep in mind that unlike most Butano containers, bn::unique_ptr uses the heap instead of the stack.
 *
 *
 * @subsection faq_heap So I shouldn't use the heap?
 *
 * Since heap usage is slow and the heap allocator included with Butano is very limited,
 * avoid heap usage whenever possible.
 *
 * Also, remember to call bn::core::init before using the heap.
 *
 *
 * @subsection faq_memory_types Why I run out of memory so often?
 *
 * Besides VRAM and such, the GBA provides two memory banks:
 * * IWRAM: 32KB fast RAM.
 * * EWRAM: 256KB slow RAM.
 *
 * Data is allocated in IWRAM by default, so it is common to run out of memory if you don't use EWRAM.
 *
 * To place data in EWRAM, you can:
 * * Allocate memory in the heap, since it is placed in EWRAM.
 * * Declare static data with the `BN_DATA_EWRAM` macro:
 * @code{.cpp}
 * BN_DATA_EWRAM static_data data;
 * @endcode
 *
 * However, if the data is read only, you can avoid wasting RAM by placing it in ROM with the `const` qualifier:
 * @code{.cpp}
 * const const_data data;
 * @endcode
 *
 *
 * @subsection faq_destroy_ptr How to destroy sprites and backgrounds?
 *
 * bn::sprite_ptr, bn::regular_bg_ptr and all Butano classes that end with the `_ptr` suffix are
 * `std::shared_ptr` like smart pointers that retains shared ownership of a hardware resource.
 *
 * In the case of bn::sprite_ptr, several bn::sprite_ptr objects may own the same sprite.
 * The sprite is released when the last remaining bn::sprite_ptr owning it is destroyed.
 *
 * If you want to learn more about `std::shared_ptr`, you can read:
 * * <a href="https://www.geeksforgeeks.org/auto_ptr-unique_ptr-shared_ptr-weak_ptr-2">A basic introduction of standard smart pointers</a>.
 * * <a href="https://docs.microsoft.com/en-us/cpp/cpp/how-to-create-and-use-shared-ptr-instances?view=msvc-160">A std::shared_ptr usage guide</a>.
 *
 *
 * @subsection faq_destroy_optional That's cool, but how can I destroy and reload sprites and backgrounds as easy as possible?
 *
 * bn::optional helps to unload and reload hardware resources easily:
 *
 * @code{.cpp}
 * // bg_optional is empty:
 * bn::optional<bn::regular_bg_ptr> bg_optional;
 *
 * // bg_optional now contains the background specified by bn::regular_bg_items::bg1:
 * bg_optional = bn::regular_bg_items::bg1.create_bg(0, 0);
 *
 * // bg_optional is empty again:
 * bg_optional.reset();
 *
 * // bg_optional now contains the background specified by bn::regular_bg_items::bg2:
 * bg_optional = bn::regular_bg_items::bg2.create_bg(0, 0);
 * @endcode
 *
 *
 * @subsection faq_float_double Can I use float or double when coding for the GBA?
 *
 * Normally CPUs have dedicated hardware for processing `float`s and `double`s, called an FPU.
 * The GBA doesn't have one, so you're left with 2 choices:
 *
 * * Using soft float, which is floating point emulated in software (*very* slow).
 * * Using fixed point arithmetic, which is just integer arithmetic with a few extra steps.
 *
 * Butano provides bn::fixed, which tries to be as close as possible to a `float` with fixed point arithmetic.
 * Use it when you want to work with numbers with decimal points.
 *
 *
 * @subsection faq_sprintf How do I print a number in a string?
 *
 * *Don't* use `sprintf`, `std::ostring_stream` nor anything that comes with the standard library, because:
 *
 * * Butano functions for printing numbers in strings can be more than 5 times faster than the standard functions.
 * * Standard functions can take more than 4KB of IWRAM.
 * * Standard functions sometimes can't be used because they give link errors.
 *
 * If you want to print a number in a string with Butano, bn::to_string provides the easiest way to do it:
 *
 * @code{.cpp}
 * bn::string<16> string = bn::to_string<16>(number);
 * @endcode
 *
 * If you want to compose a string with multiple numbers, you can use bn::ostringstream too:
 *
 * @code{.cpp}
 * bn::string<64> string;
 * bn::ostringstream string_stream(string);
 * string_stream << "Text: ";
 * string_stream << 123;
 * string_stream << " - ";
 * string_stream << number;
 * @endcode
 *
 *
 * @subsection faq_global_objects Does Butano allow to declare bn::sprite_ptr or bn::regular_bg_ptr objects globally?
 *
 * In general, you should not do anything with Butano before calling bn::core::init,
 * including creating global Butano objects and allocating memory in the heap.
 *
 * If you want to declare global Butano objects, you can do something like this instead:
 *
 * @code{.cpp}
 * struct global_data
 * {
 *     bn::sprite_ptr sprite;
 *     bn::regular_bg_ptr bg;
 * };
 *
 * global_data* global_ptr;
 *
 * int main()
 * {
 *     bn::core::init();
 *
 *     global_data global_instance = {
 *         bn::sprite_items::sprite_item.create_sprite(0, 0),
 *         bn::regular_bg_items::bg_item.create_bg(0, 0)
 *     };
 *
 *     global_ptr = &global_instance;
 *
 *     // ...
 * }
 * @endcode
 *
 * And then you can access global Butano objects from anywhere in your code with something like this:
 *
 * @code{.cpp}
 * global_ptr->sprite.set_position(50, 50);
 * @endcode
 *
 *
 * @subsection faq_wait_updates Is there a way to stop running my code for a certain amount of time?
 *
 * Since you can usually assume than your game is running at 60FPS, you can wait a second for example with this:
 *
 * @code{.cpp}
 * for(int index = 0; index < 60; ++index)
 * {
 *     bn::core::update();
 * }
 * @endcode
 *
 *
 * @subsection faq_utf8_characters How can I print UTF-8 characters like japanese or chinese ones?
 *
 * bn::sprite_text_generator already supports UTF-8 characters rendering,
 * but the bn::sprite_font instances used in the examples don't provide japanese nor chinese characters,
 * so you will have to make a new one with them.
 *
 *
 * @subsection faq_tonc_general_notes Are there some more general notes on GBA programming out there?
 *
 * <a href="https://www.coranac.com/tonc/text/first.htm#sec-notes">I'm glad you asked</a>.
 *
 *
 * @section faq_color Colors
 *
 *
 * @subsection faq_transparent_color Which color is the transparent one?
 *
 * Butano supports 16 or 256 color images only, so they must have a color palette.
 *
 * The transparent color is the first one in the color palette,
 * so in order to change it you should use a bitmap editor with color palette manipulation tools,
 * like <a href="https://www.coranac.com/projects/usenti/">Usenti</a>:
 *
 * @image html import_usenti.png
 *
 *
 * @subsection faq_backdrop_color How can I set the backdrop color?
 *
 * The transparent or the backdrop color (displayed color when nothing else is)
 * is the first one in the backgrounds palette.
 *
 * You can override its default value with bn::bg_palettes::set_transparent_color.
 *
 *
 * @subsection faq_share_palettes How to share the same color palette between sprites or backgrounds?
 *
 * If two sprites or backgrounds have the same colors, by default they share the same color palette.
 *
 * Keep in mind that unused colors are also taken into account when deciding if two color palettes are equal or not.
 *
 *
 * @subsection faq_multiple_8bpp_objects Why everything looks weird when I show two or more backgrounds or sprites with more than 16 colors?
 *
 * Since the GBA has only two 256 color palettes (one for sprites and the other for backgrounds),
 * if you use for example two backgrounds with more than 16 colors,
 * Butano assumes that they have the same color palette (same colors in the same order).
 *
 * So if you are going to show at the same time multiple backgrounds with more than 16 colors,
 * use the same color palette with all of them (in the same scene of course,
 * different backgrounds shown in different scenes can have different color palettes).
 *
 *
 * @section faq_backgrounds Backgrounds
 *
 *
 * @subsection faq_bg_wrapping Why some backgrounds don't allow wrapping?
 *
 * If you try to move a big background beyond its boundaries, an error message like this one should be displayed:
 *
 * @image html faq_bg_wrapping.png
 *
 * That's because big backgrounds don't allow wrapping, so if you are using a big background,
 * avoid moving it beyond its boundaries (or avoid using big backgrounds).
 *
 *
 * @subsection faq_big_background What's a big background?
 *
 * The GBA only supports some fixed sizes for background maps.
 *
 * However, Butano allows to manage background maps with any size multiple of 256 pixels.
 * These special background maps and the backgrounds that display them are called big maps/backgrounds.
 *
 * Try to avoid big backgrounds whenever possible, because they are slower CPU wise and
 * don't support wrapping (they can't be moved beyond their boundaries).
 *
 *
 * @subsection faq_regular_affine_background Why there are two types of backgrounds (regular and affine)?
 *
 * It seems it is always better to use affine backgrounds, since they can be rotated, scaled, etc.
 * and its size can be up to 1024x1024 pixels without becoming big backgrounds.
 *
 * However, compared to regular backgrounds, affine backgrounds have these limitations:
 * * Only two of them can be displayed at the same time, instead of four.
 * * They don't support 16 color tiles, only 256 color ones.
 * * They only support up to 256 different tiles, instead of 1024.
 *
 * Because of these limitations, you should avoid affine backgrounds whenever possible.
 *
 *
 * @subsection faq_background_error_grit Why can't I import a regular background with 1024 or less tiles?
 *
 * If you get this error when trying to import a regular background with 1024 or less tiles:
 *
 * `error: Regular BGs with more than 1024 tiles not supported: 1025`
 *
 * Or you get this error when importing an affine background with 256 or less tiles:
 *
 * `error: Affine BGs with more than 256 tiles not supported: 257`
 *
 * Your image is fine, but <a href="https://www.coranac.com/projects/grit/">grit</a>
 * (the tool used by Butano to import images) is generating unneeded extra tiles.
 *
 * The only workaround that I know of is reducing detail in your input image until the tiles count of
 * the generated background is valid.
 *
 *
 * @section faq_audio Audio
 *
 *
 * @subsection faq_music_crash Why the game crashes when some Direct Sound songs are played?
 *
 * Butano uses the excellent <a href="https://maxmod.devkitpro.org/">Maxmod</a> library
 * for Direct Sound audio support.
 *
 * It provides impressive performance and support for lots of module music formats,
 * but unfortunately it crashes with some songs.
 *
 * Sometimes it helps to change song's file format (for example, from `*.xm` to `*.it`).
 * You can use <a href="https://openmpt.org/">OpenMPT</a> to do that.
 *
 * You can also try to create a new issue in its
 * <a href="https://github.com/devkitPro/maxmod/issues">GitHub issues</a> page,
 * but since it seems the library was abandoned long time ago, don't hold your hopes up too much.
 *
 *
 * @subsection faq_music_missing_notes Why there are missing notes when playing some Direct Sound songs?
 *
 * If a song doesn't have more channels than the maximum number of active Direct Sound music channels
 * specified by @ref BN_CFG_AUDIO_MAX_MUSIC_CHANNELS, as before,
 * sometimes it helps to change its file format (for example, from `*.xm` to `*.it`).
 * You can use <a href="https://openmpt.org/">OpenMPT</a> to do that.
 *
 *
 * @subsection faq_audio_quality How can I improve audio quality?
 *
 * If you have some free CPU left, you can increase audio mixing rate to improve its quality.
 *
 * The easiest way to specify the audio mixing rate for a specific project is
 * to define it in the `USERFLAGS` of its `Makefile`.
 *
 * For example, to set the audio mixing rate to 21KHz:
 *
 * `USERFLAGS := -DBN_CFG_AUDIO_MIXING_RATE=BN_AUDIO_MIXING_RATE_21_KHZ`
 *
 * Remember to rebuild your project from scratch after modifying a `Makefile`.
 *
 * Available mixing rates are @ref audio "here".
 *
 *
 * @section faq_flash_carts Flash carts
 *
 *
 * @subsection faq_flash_carts_start Why my game runs fine on emulators but doesn't work on a real GBA with a flash cart?
 *
 * Some flash carts allow to improve commercial games with patches like `saver patch`, `enable restart`,
 * `enable real time save`, etc.
 *
 * These patches can break homebrew games, so try to disable some or all of them if you run into any issues.
 *
 *
 * @subsection faq_flash_carts_sram Why SRAM works on emulators but doesn't work with this old flash cart?
 *
 * While SRAM works out-of-the-box with most modern flash carts, it can fail with some older ones.
 *
 * To fix it you can try to:
 * * Set save type as SRAM.
 * * Disable or enable save patches.
 * * Update the firmware of the flash cart.
 */


/**
 * @page changelog Changelog
 *
 * @tableofcontents
 *
 *
 * @section changelog_13_1_0 13.1.0 (next release)
 *
 * * bn::list::remove, bn::list::remove_if, bn::forward_list::remove, bn::forward_list::remove_if,
 *   bn::intrusive_list::remove, bn::intrusive_list::remove_if, bn::intrusive_forward_list::remove and
 *   bn::intrusive_forward_list::remove_if added.
 * * Hash support for bn::istring and bn::string added.
 * * <a href="https://www.coranac.com/projects/#tonc">Tonclib</a> unused components removed
 *   (EWRAM and ROM usage reduced).
 * * Flipped tiles reduction disabled in `dynamic_regular_bg` example.
 *
 *
 * @section changelog_13_0_0 13.0.0
 *
 * * <b>(Breaking change)</b> bn::best_fit_allocator (used by the heap manager) supports unlimited allocations.
 * * bn::best_fit_allocator::log_status and bn::memory::log_alloc_ewram_status added.
 * * New games made with Butano added to `README.md`.
 *
 *
 * @section changelog_12_5_0 12.5.0
 *
 * @ref BN_CFG_ASSERT_SHOW_DIAGNOSTIC added.
 *
 *
 * @section changelog_12_4_0 12.4.0
 *
 * * SRAM magic string fixed.
 * * `BN_CODE_IWRAM` macro can be used for thumb code too.
 * * Destructors of classes with `_ptr` suffix ROM usage reduced.
 * * 4BPP regular backgrounds quantization improved.
 * * Some error messages improved.
 * * @ref import_image "Import images guide" explains why <a href="https://www.coranac.com/projects/usenti/">Usenti</a>
 *   is recommended over other image editors.
 *
 *
 * @section changelog_12_3_0 12.3.0
 *
 * * bn::sprite_text_generator variable width fonts rendering fixed and optimized.
 * * Sprite shape and size update ROM usage reduced.
 * * Some Cppcheck warnings removed.
 *
 *
 * @section changelog_12_2_0 12.2.0
 *
 * * Automatic sprite double size detection fixed.
 * * Sprite tiles management optimized (EWRAM and CPU usage reduced).
 * * Background tiles and maps management optimized (EWRAM and CPU usage reduced).
 * * Color palettes management CPU usage reduced.
 * * Sprites and sprite affine mats management CPU usage reduced.
 * * Backgrounds management CPU usage reduced.
 * * Cameras management CPU usage reduced.
 *
 *
 * @section changelog_12_1_0 12.1.0
 *
 * * Sprite tiles delayed commit fixed.
 * * V-Blank interrupt management optimized (EWRAM and CPU usage reduced).
 * * Missed link messages frequency reduced.
 * * bn::timer::elapsed_ticks optimized.
 *
 *
 * @section changelog_12_0_0 12.0.0
 *
 * * <b>(Breaking change)</b> bn::aligned fixed.
 * * Color effects optimized.
 *
 *
 * @section changelog_11_0_1 11.0.1
 *
 * bn::string::starts_with and bn::string_view::starts_with fixed.
 *
 *
 * @section changelog_11_0_0 11.0.0
 *
 * Huffman images compression support
 * (make sure to update <a href="https://devkitpro.org/">devkitARM</a> before using it).
 *
 *
 * @section changelog_10_7_0 10.7.0
 *
 * * bn::unique_ptr move constructor, move assignment operator and bn::unique_ptr::reset
 *   allow to manage pointers and deleters with different type.
 * * bn::unique_ptr documentation improved.
 * * More questions added to @ref faq page.
 * * @ref faq page improved.
 *
 *
 * @section changelog_10_6_1 10.6.1
 *
 * bn::bitset AND, OR and XOR operators fixed.
 *
 *
 * @section changelog_10_6_0 10.6.0
 *
 * * bn::sprite_animate_action::set_wait_updates and bn::sprite_cached_animate_action::set_wait_updates added.
 * * bn::sprite_animate_action::reset and bn::sprite_cached_animate_action::reset added.
 * * Backgrounds management optimized (EWRAM and CPU usage reduced).
 * * Sprites, sprite tiles, palettes and HDMA commit CPU usage reduced.
 * * Missed link messages frequency reduced.
 * * <a href="https://mgba.io">mGBA</a> logging CPU usage reduced.
 * * Keypad logging EWRAM and maximum CPU usage reduced.
 * * CPU usage profiler entries removed.
 * * Some assert messages improved.
 *
 *
 * @section changelog_10_5_0 10.5.0
 *
 * * bn::core::vblank_callback and bn::core::set_vblank_callback added.
 * * bn::random::update added.
 * * H-Blank effects input data validation fixed.
 *
 *
 * @section changelog_10_4_0 10.4.0
 *
 * * Avoid data decompression in V-Blank if possible.
 * * Audio updates can no longer be interrupted.
 *
 *
 * @section changelog_10_3_0 10.3.0
 *
 * * bn::bg_tiles::allow_offset and bn::bg_tiles::set_allow_offset added.
 * * Import tool now can generate regular and affine background tiles without maps nor palettes.
 * See the @ref import_regular_bg_tiles and the @ref import_affine_bg_tiles import guides to learn how to import them.
 * * BPP mode can be specified when importing sprite tiles.
 * See the @ref import_sprite_tiles import guide to learn how to specify it.
 * * Audio updates can be interrupted.
 * * SRAM reads and writes optimized.
 * * BG blocks manager ROM usage reduced.
 * * <a href="https://github.com/felixjones/agbabi">agbabi</a> memory routines updated.
 * * Missing `__FILE_NAME__` macro check added.
 * * Missing license files added.
 *
 *
 * @section changelog_10_2_0 10.2.0
 *
 * * bn::core_lock added.
 * * bn::reciprocal_16_lut added.
 * * Memory operations like `memcpy` and `memset` optimized thanks to
 * <a href="https://github.com/felixjones/agbabi">agbabi</a>.
 * * bn::affine_mat_attributes scale setup optimized (CPU and ROM usage reduced).
 * * bn::atan2 and bn::degrees_atan2 optimized thanks to <a href="https://github.com/felixjones/agbabi">agbabi</a>.
 * * Unused interrupts removed from the interrupt manager.
 * * More questions added to @ref faq page.
 *
 *
 * @section changelog_10_1_0 10.1.0
 *
 * * bn::audio::dmg_sync_enabled and bn::audio::set_dmg_sync_enabled added.
 * * Custom assembler flags can be specified in the project's `Makefile`.
 * * <a href="https://github.com/AntonioND/gbt-player">GBT Player</a> updated to 4.4.1.
 * * bn::fixed, bn::optional, bn::string_view and bn::span are no longer forward declared in the public API.
 * * Text color in assert and profile screens improved.
 * * Audio volume setup optimized.
 * * Clouds restored in `world_map` example.
 *
 *
 * @section changelog_10_0_0 10.0.0
 *
 * * Custom linker flags can be specified in the project's `Makefile`.
 * * Asserts ROM usage reduced.
 * * Audio startup fixed when link-time optimization is enabled.
 * * Audio stop fixed when link communication is active.
 * * Parallel link-time optimization temporal files ignored and added to the list of files to clean.
 *
 *
 * @section changelog_9_5_0 9.5.0
 *
 * * bn::dmg_music::left_volume, bn::dmg_music::right_volume, bn::dmg_music::set_left_volume,
 * bn::dmg_music::set_right_volume and bn::dmg_music::set_volume added.
 * * bn::dmg_music::position and bn::dmg_music::set_position added.
 * * bn::dmg_music_volume_to_action, bn::dmg_music_volume_loop_action and bn::dmg_music_volume_toggle_action added.
 * * bn::link_state::player_count added.
 * * An external `mod2gbt` executable is no longer necessary for importing DMG music with `*.mod` extension.
 * * <a href="https://github.com/AntonioND/gbt-player">GBT Player</a> updated to 4.4.0.
 * * New interrupt manager implemented by <a href="https://github.com/AntonioND/libugba">Universal GBA Library</a>.
 * * Audio commands list management optimized (ROM, EWRAM and CPU usage reduced).
 * * Missed link messages frequency reduced.
 * * Link data sending race condition fixed.
 * * Link race condition on V-Blank fixed.
 * * Audio pause/resume <a href="https://problemkaputt.de/gba.htm">No$gba</a> error removed.
 * * Palette rotate range documentation fixed.
 * * @ref faq page improved.
 *
 *
 * @section changelog_9_4_1 9.4.1
 *
 * BMP files colors count calculation fixed (again).
 *
 *
 * @section changelog_9_4_0 9.4.0
 *
 * * bn::dmg_music and bn::dmg_music_item added (yes, music playback using DMG channels is now supported
 * thanks to <a href="https://github.com/AntonioND/gbt-player">GBT Player</a>.
 * See the @ref import_dmg_music import guide and the `dmg_audio` example to learn how to import and play it.
 * * BMP files colors count calculation fixed.
 * * C++ keywords are not valid asset names anymore.
 * * @ref getting_started Python setup improved.
 * * <a href="https://cinemint.itch.io/tremblay-island">Tremblay Island</a> added to `README.md`.
 *
 *
 * @section changelog_9_3_0 9.3.0
 *
 * * bn::sram::clear and bn::sram::set_bytes added.
 * * bn::music::playing_item added.
 * * SRAM code moved back from ROM to EWRAM again, since it doesn't seem to crash
 * <a href="https://problemkaputt.de/gba.htm">No$gba</a> anymore.
 * * bn::power_of_two optimized.
 * * Keypad with frame skipping fixed.
 *
 *
 * @section changelog_9_2_0 9.2.0
 *
 * * bn::regular_bg_ptr::hw_id and bn::affine_bg_ptr::hw_id added.
 * * bn::sin_and_cos, bn::degrees_sin_and_cos, bn::lut_sin_and_cos and bn::degrees_lut_sin_and_cos added.
 * * More bn::regular_bg_ptr and bn::affine_bg_ptr static contructors added.
 * * bn::regular_bg_map_item::cells_ptr and bn::affine_bg_map_item::cells_ptr added.
 * * bn::affine_mat_attributes rotation angle processing optimized.
 * * bn::unordered_map and bn::unordered_set lack of pointer stability documented.
 *
 *
 * @section changelog_9_1_1 9.1.1
 *
 * bn::calculate_sin_lut_value sine and cosine symmetry with 135 and 315 degrees fixed.
 *
 *
 * @section changelog_9_1_0 9.1.0
 *
 * * bn::system_font, bn::core::system_font and bn::core::set_system_font added.
 * * bn::core::assert_tag and bn::core::set_assert_tag added.
 * * @ref BN_VERSION_STRING added.
 * * bn::rect_window::restore_boundaries added.
 * * bn::window::set_show_all and bn::window::set_show_nothing fixed.
 * * bn::utf8_characters_map_ref::index assert shows UTF8-character data.
 * * <a href="https://github.com/laqieer/gba-free-fonts">gba-free-fonts</a> added to `README.md`.
 *
 *
 * @section changelog_9_0_0 9.0.0
 *
 * * bn::utf8_characters_map and bn::utf8_characters_map_ref added:
 *   they allow to build an UTF-8 characters map in ROM, without size limit.
 * * <b>(Breaking change)</b> bn::sprite_font now manages a bn::utf8_characters_map_ref
 *   instead of a span of UTF-8 characters.
 * * <b>(Breaking change)</b> `BN_CFG_SPRITE_TEXT_MAX_UTF8_CHARACTERS` removed.
 * * bn::blending_transparency_attributes::set_transparency_and_intensity_alphas added.
 * * More questions added to @ref faq page.
 *
 *
 * @section changelog_8_10_0 8.10.0
 *
 * * Huffman decompression support added.
 * * bn::best_fit_allocator added.
 * * bn::istring::resize and bn::istring::shrink added.
 * * bn::string and bn::string_view compatibility improved.
 * * bn::string construction, assignment and append optimized.
 * * Integer to string conversion optimized.
 * * Slot index added to palettes manager status log.
 * * bn::calloc and bn::memory::ewram_calloc interface fixed.
 * * `.elf` files <a href="https://problemkaputt.de/gba.htm">No$gba</a> support restored.
 * * `-O0` builds support restored.
 * * More questions added to @ref faq page.
 *
 *
 * @section changelog_8_9_0 8.9.0
 *
 * * Hue shift palette effect added.
 * See the `palettes` example to learn how to use it.
 * * Brightness effect ROM usage reduced.
 * * bn::sprite_item and bn::sprite_tiles_item size reduced.
 * * Automatic sprite double size detection fixed.
 * * bn::hbe_visible_toggle_action removed, since it was not possible to use it.
 * * Clang build fixes.
 * * Clang-Tidy warnings fixed.
 * * New games made with Butano added to `README.md`.
 *
 *
 * @section changelog_8_8_0 8.8.0
 *
 * * Modified assets detection optimized.
 * * Audio and graphics tools unified in order to reduce build time.
 * * bn::newton_raphson_sqrt removed, since it doesn't work for bn::fixed input values.
 *
 *
 * @section changelog_8_7_0 8.7.0
 *
 * * An external bn::bg_palette_item can be specified when importing regular and affine backgrounds.
 * * Colors count can be specified when importing sprite and background palettes.
 * * Standard containers equality comparison optimized.
 * * Asserts file name hack fixed.
 * * <a href="https://github.com/AntonioND/libugba">Universal GBA Library</a> LGPL license files removed.
 * * Standard containers documentation improved.
 * * @ref faq page improved.
 *
 *
 * @section changelog_8_6_0 8.6.0
 *
 * * bn::random::get_int and bn::random::get_fixed added.
 * * bn::calloc, bn::realloc, bn::memory::ewram_calloc and bn::memory::ewram_realloc added.
 * * bn::unique_ptr moved to its own header file.
 * * bn::pool and bn::generic_pool ROM usage reduced.
 * * Automatic 4BPP regular backgrounds generation fixed.
 * * Asserts disabled build warnings fixed.
 * * @ref faq page improved.
 *
 *
 * @section changelog_8_5_0 8.5.0
 *
 * * bn::optional::get added.
 * * bn::bitset and bn::bitset_ref optimized (managed bits count now must be a multiple of 8).
 * * ROM usage reduced due to unnecessary internal asserts removed.
 * * Blending hardware weights fixed.
 * * bn::regular_bg_map_cell_info documentation fixed.
 *
 *
 * @section changelog_8_4_0 8.4.0
 *
 * * bn::bitset_ref added.
 * * bn::diamond_angle and bn::degrees_diamond_angle added.
 * * Asserts ROM usage reduced.
 * * Unnecessary `inline` keyword usages removed.
 *
 *
 * @section changelog_8_3_0 8.3.0
 *
 * * Maps commit optimized by more than 50% in some cases.
 * * Integer alignment at minimum enforced in all containers.
 * * BG blocks manager and sprite tiles manager commit fixed.
 *
 *
 * @section changelog_8_2_0 8.2.0
 *
 * * <b>(Security issue)</b> Source code files full path is not stored in output ROMs anymore.
 * * Some Clang-Tidy and Cppcheck warnings removed.
 *
 *
 * @section changelog_8_1_0 8.1.0
 *
 * * bn::color_effect added.
 * * bn::memory::decompress added.
 * * All `uncompress` methods renamed to `decompress`.
 * * All color effects optimized.
 * * Palettes update and search optimized.
 * * @ref getting_started page improved.
 *
 *
 * @section changelog_8_0_0 8.0.0
 *
 * * Blending transparency weights can be setup separately with bn::blending::set_transparency_top_weight and
 *   bn::blending::set_transparency_bottom_weight.
 * * <b>(Breaking change)</b> bn::blending_transparency_attributes interface has changed to allow to setup
 *   transparency weights separately.
 * * Actions for blending transparency weights added.
 * * Internal graphics suffix changed from `bn_graphics` to `bn_gfx`. Remember to rebuild your project after updating.
 * * @ref faq page improved.
 *
 *
 * @section changelog_7_8_0 7.8.0
 *
 * * Generic slow game pak detection replaces SuperCard SD specific one.
 * * @ref BN_GAME_PAK_WAIT_STATE_FIRST_AUTO and @ref BN_GAME_PAK_WAIT_STATE_SECOND_AUTO enabled by default.
 * * Affine matrices update optimized.
 * * Tiles and maps managers optimized.
 * * Background windows optimized.
 * * bn::audio::update_on_vblank and bn::audio::set_update_on_vblank documentation improved.
 * * @ref faq page improved.
 *
 *
 * @section changelog_7_7_0 7.7.0
 *
 * * bn::audio::update_on_vblank and bn::audio::set_update_on_vblank added.
 * * Sprite tiles update and commit optimized.
 * * Background tiles and maps update and commit optimized.
 * * Big maps commit optimized.
 * * Asserts ROM usage reduced.
 * * H-Blank effects ROM usage reduced.
 * * bn::sprite_text_generator ROM usage reduced.
 *
 *
 * @section changelog_7_6_0 7.6.0
 *
 * * Display and audio hardware commit optimized.
 * * bn::unique_ptr size reduced.
 * * bn::regular_bg_ptr::set_item and bn::affine_bg_ptr::set_item fixed.
 * * @ref faq page improved.
 * * @ref main_licenses section improved.
 *
 *
 * @section changelog_7_5_1 7.5.1
 *
 * Audio and HDMA issues when updating at less than 60FPS fixed.
 *
 *
 * @section changelog_7_5_0 7.5.0
 *
 * * bn::sprites::reserved_handles_count and bn::sprites::set_reserved_handles_count added.
 * * @ref BN_CFG_PROFILER_LOG_ENGINE_DETAILED added.
 * * Sprites and backgrounds decompression optimized thanks to
 * <a href="https://github.com/Cult-of-GBA/BIOS">Cult-of-GBA BIOS</a>.
 * * Sprites and backgrounds hardware setup optimized.
 * * Sprites automatic double size mode optimized.
 * * Affine sprites hiding fixed.
 *
 *
 * @section changelog_7_4_0 7.4.0
 *
 * * Run-length is preferred to LZ77 when choosing best compression mode.
 * * Sprites automatic double size mode and affine matrix management fixed.
 * * @ref faq page improved.
 *
 *
 * @section changelog_7_3_0 7.3.0
 *
 * * bn::sprite_text_generator now can plot multiple 16x16 variable width characters in a single sprite.
 * * bn::sprite_text_generator IWRAM usage reduced.
 * * Third party libraries to link can be specified in the project's `Makefile`.
 * * Project's `Makefile` allows to build source files generated with an external tool.
 * See the `external_tool` example to learn how to do it.
 * * `butano-audio-tool.py` generates a list of all available music and sound items with their name.
 * * `audio_player` example added.
 * * <a href="https://gvaliente.itch.io/varooom-3d">Varooom 3D</a> improved
 * (check its `readme.txt` file for details).
 *
 *
 * @section changelog_7_2_0 7.2.0
 *
 * * bn::regular_bg_map_cell_info and bn::affine_bg_map_cell_info added.
 * * bn::regular_bg_map_item::cell_index and bn::regular_bg_map_item::cell added.
 * * bn::affine_bg_map_item::cell_index and bn::affine_bg_map_item::cell added.
 * * Maps commit optimized by more than 200% in some cases.
 * * `dynamic_regular_bg` example added.
 * * `dynamic_bg` example renamed to `dynamic_affine_bg` and improved.
 *
 *
 * @section changelog_7_1_1 7.1.1
 *
 * bn::vector, bn::unordered_map and bn::unordered_set move constructors and assignment operators fixed.
 *
 *
 * @section changelog_7_1_0 7.1.0
 *
 * * HDMA commit fixed.
 * * @ref faq page improved.
 * * Games made with Butano added to `README.md`.
 *
 *
 * @section changelog_7_0_1 7.0.1
 *
 * bn::format build fix.
 *
 *
 * @section changelog_7_0_0 7.0.0
 *
 * * <a href="https://gvaliente.itch.io/varooom-3d">Varooom 3D</a> source code and assets added.
 * * Maximum number of active H-Blank effects reduced to 6.
 * * Common files moved to `common` namespace.
 *
 *
 * @section changelog_6_18_0 6.18.0
 *
 * * Generated items are now `constexpr inline` to avoid being copied to every translation unit.
 * * Sprites EWRAM usage reduced.
 * * Different type backgrounds sorting fixed.
 *
 *
 * @section changelog_6_17_1 6.17.1
 *
 * * bn::sort with 128bit sort keys fixed.
 * * IRQs setup during startup improved.
 *
 *
 * @section changelog_6_17_0 6.17.0
 *
 * * bn::memory::used_stack_iwram added.
 * * bn::sort optimized.
 * * VRAM allocation error messages improved.
 * * bn::optional::emplace copy avoided.
 * * @ref import guide improved.
 * * More questions added to @ref faq page.
 *
 *
 * @section changelog_6_16_0 6.16.0
 *
 * * bn::sin_lut is not copied to every translation unit.
 * * bn::reciprocal_lut is not copied to every translation unit.
 * * GBA keypad is read after playing all keypad commands.
 * * bn::degrees_lut_cos assert added.
 *
 *
 * @section changelog_6_15_0 6.15.0
 *
 * * bn::rect::top_right, bn::rect::bottom_left, bn::fixed_rect::top_right and bn::fixed_rect::bottom_left added.
 * * GPIO register is not setup unless needed (avoids <a href="https://mgba.io">mGBA</a> warning at startup).
 * * SRAM code moved from EWRAM to ROM in order to avoid a
 * <a href="https://problemkaputt.de/gba.htm">No$gba</a> error.
 * * bn::abs for unsigned arguments improved.
 * * `*.elfbin` output files renamed to `*.elf` as it should.
 * * bn::unordered_map::erase and bn::unordered_set::erase fixed.
 *
 *
 * @section changelog_6_14_0 6.14.0
 *
 * * Asserts ROM usage reduced.
 * * bn::deque::push_front, bn::deque::emplace_front and bn::deque::insert fixed.
 *
 *
 * @section changelog_6_13_1 6.13.1
 *
 * * bn::to_array fixed.
 * * bn::sprite_text_generator documentation fixed.
 *
 *
 * @section changelog_6_13_0 6.13.0
 *
 * * EWRAM wait states count can be specified with @ref BN_CFG_EWRAM_WAIT_STATE.
 * * bn::fixed_t::floor_integer and bn::fixed_t::ceil_integer added.
 * * bn::keypad::any_held, bn::keypad::any_pressed and bn::keypad::any_released added.
 * * Unnecessary GPIO register writes avoided.
 * * bn::type_id marked as `constexpr`.
 * * Background move actions fixed.
 * * `constexpr const` replaced with `constexpr` where it makes sense.
 *
 *
 * @section changelog_6_12_0 6.12.0
 *
 * * Multiplication and division methods added to bn::fixed_point, bn::fixed_size and bn::fixed_rect.
 * * Colors count added to palettes manager status log.
 * * Invalid bn::fixed_t unsigned operator overloads removed.
 *
 *
 * @section changelog_6_11_0 6.11.0
 *
 * * An external tool can be called from the project's `Makefile`.
 * See the `external_tool` example to learn how to call it.
 * * Affine background tiles offset optimized.
 * * Big numbers to string conversion optimized.
 * * 32bit integer division optimized.
 * * 64bit integer division by 0 assert added.
 * * Big maps maximum valid position fixed.
 * * Compressed tiles warning fixed.
 * * GCC11 warning fixed.
 * * Some 8x8 sprite text characters fixed.
 * * @ref qt_creator added.
 * * More questions added to @ref faq page.
 *
 *
 * @section changelog_6_10_0 6.10.0
 *
 * * `dynamic_bg` example added.
 * * Affine background tiles offset fixed.
 *
 *
 * @section changelog_6_9_0 6.9.0
 *
 * * bn::sprite_font supports all sprite shapes and sizes.
 * * Trivial type containers destruction optimized.
 * * Container destructors are always `noexcept`.
 * * Redundant container asserts removed.
 * * GPIO direction register setup.
 *
 *
 * @section changelog_6_8_0 6.8.0
 *
 * * Sprites optimized.
 * * Blending optimized.
 * * Mosaic optimized.
 * * Internal memory transfers optimized.
 * * `texture_polygons` example affine parameters fixed.
 * * Clouds removed from `world_map` example.
 *
 *
 * @section changelog_6_7_0 6.7.0
 *
 * * Automatic sprite double size detection improved.
 * * Butano Fighter allows to disable rumble in the title menu.
 *
 *
 * @section changelog_6_6_2 6.6.2
 *
 * Automatic sprite double size detection fixed.
 *
 *
 * @section changelog_6_6_1 6.6.1
 *
 * BG blocks manager use after move fixed.
 *
 *
 * @section changelog_6_6_0 6.6.0
 *
 * * SRAM memory usage reduced.
 * * Sprite animate actions memory usage reduced.
 * * Missing asserts added to some actions.
 * * bn::random is now a trivial class.
 * * BG blocks manager search fixed.
 * * `world_map` example frame rate back to 60FPS.
 *
 *
 * @section changelog_6_5_0 6.5.0
 *
 * * bn::bitset and bn::ibitset added.
 * * BG blocks manager and sprite tiles manager allocation fixed.
 *
 *
 * @section changelog_6_4_0 6.4.0
 *
 * * Sprites and affine backgrounds support horizontal and vertical shear.
 * * bn::affine_mat_attributes::unsafe_set_register_values added.
 * * `texture_polygons` example added.
 * * More questions added to @ref faq page.
 *
 *
 * @section changelog_6_3_0 6.3.0
 *
 * * @ref BN_VERSION_MAJOR, @ref BN_VERSION_MINOR and @ref BN_VERSION_PATCH added.
 * * bn::core::current_cpu_usage, bn::core::last_cpu_usage and bn::core::last_vblank_usage accuracy improved.
 * * Signed division fixed and optimized.
 * * BG blocks manager and sprite tiles manager allocation fixed.
 * * BG blocks manager prints requested blocks count when an allocation fails.
 *
 *
 * @section changelog_6_2_0 6.2.0
 *
 * * bn::core::skip_frames and bn::core::set_skip_frames added.
 * * bn::sprite_font usability improved.
 * * bn::reciprocal_lut and bn::lut_reciprocal fixed.
 *
 *
 * @section changelog_6_1_0 6.1.0
 *
 * * Sprite tiles generation outputs sprite shape and size.
 * * @ref BN_ASSERT and @ref BN_ERROR can be modified or disabled for specific code sections.
 * * Sprites IWRAM usage reduced.
 * * Some internal asserts removed.
 * * Profiler screen fixed.
 * * Documentation explains how to place data in ROM.
 *
 *
 * @section changelog_6_0_0 6.0.0
 *
 * * Rumble support (if your cart supports it). See @ref rumble and the `rumble` example.
 * * bn::sprites::reload added.
 * * bn::hdma::high_priority_start and bn::hdma::high_priority_stop added.
 * * bn::hdma::running and bn::hdma::high_priority_running added.
 * * Sprite tiles can be generated from images alone, without palettes.
 * * Graphics and audio subfolders supported.
 * * bn::core::reset fixed.
 * * HDMA disable when an assert or the profiler are shown fixed.
 * * HDMA first screen horizontal line fixed.
 * * bn::newton_raphson_sqrt recursion avoided.
 * * Python install guide improved.
 *
 *
 * @section changelog_5_3_1 5.3.1
 *
 * bn::atan2 and bn::degrees_atan2 fixed.
 *
 *
 * @section changelog_5_3_0 5.3.0
 *
 * * bn::sin, bn::cos, bn::atan2 and bn::degrees_atan2 added.
 * * H-Blank effects optimized (thanks JoaoBaptMG).
 * * bn::calculate_sin_lut_value optimized.
 * * bn::lut_reciprocal optimized.
 * * @ref faq page improved.
 *
 *
 * @section changelog_5_2_0 5.2.0
 *
 * * bn::sprite_ptr::put_below, bn::regular_bg_ptr::put_below and bn::affine_bg_ptr::put_below added.
 * * H-Blank effects IWRAM usage reduced.
 * * `butano-graphics-tool.py` errors reporting improved.
 *
 *
 * @section changelog_5_1_0 5.1.0
 *
 * * Automatic game pak wait states setup (see @ref BN_GAME_PAK_WAIT_STATE_FIRST_AUTO and
 *   @ref BN_GAME_PAK_WAIT_STATE_SECOND_AUTO for more information).
 * * bn::format and bn::format_ref functions added.
 * * Some methods to indicate if a background or a background map are big or not have been added.
 * * bn::calculate_sin_lut_value precision improved.
 * * @ref faq page added.
 * * `butano-graphics-tool.py` now uses all CPU cores.
 * * Missing third party license files included.
 *
 *
 * @section changelog_5_0_0 5.0.0
 *
 * * Compressed sprites and backgrounds support (see @ref import to learn how to generate them).
 * * bn::degrees_sin and bn::degrees_cos don't use a LUT anymore,
 *   they are replaced by bn::degrees_lut_sin and bn::degrees_lut_cos.
 * * bn::lut_sin and bn::lut_cos input angle range increased to [0, 2048].
 * * Enum classes size reduced.
 * * Required fields when importing assets are not defaulted anymore.
 * * bn::rule_of_three_approximation overflow fixed.
 *
 *
 * @section changelog_4_4_0 4.4.0
 *
 * * bn::sprite_font allows to specify space between characters.
 * * Background palettes bits per pixel mode can be specified by the user.
 * * Palettes change optimized.
 * * Unused palette colors are not exported anymore.
 * * bn::sprite_ptr::set_tiles() validation fixed.
 *
 *
 * @section changelog_4_3_0 4.3.0
 *
 * * H-Blank effects EWRAM usage reduced (more than 2KB by default).
 * * Optimization level changed to `-O2` to avoid another
 * <a href="https://problemkaputt.de/gba.htm">No$gba</a> crash.
 * * SRAM code moved back from ROM to EWRAM, since doing that doesn't make
 * <a href="https://problemkaputt.de/gba.htm">No$gba</a> crash anymore.
 * * Redundant reset code removed.
 *
 *
 * @section changelog_4_2_0 4.2.0
 *
 * * Sprite and background palettes can be generated from images alone, without tiles nor maps.
 * * `<cstddef>` is always included (`bn_cstddef.h` header file removed).
 * * bn::unordered_map and bn::unordered_set assignment fixed.
 *
 *
 * @section changelog_4_1_0 4.1.0
 *
 * * H-Blank effects optimized (it fixes `world_map` example flickering).
 * * SRAM code moved from EWRAM to ROM to avoid a <a href="https://problemkaputt.de/gba.htm">No$gba</a> crash.
 *
 *
 * @section changelog_4_0_0 4.0.0
 *
 * * Affine backgrounds support.
 *   See @ref affine_bg and the `affine_bgs`, `mode_7` and `world_map` examples for more.
 * * bn::hblank_effect_ptr renamed to bn::hbe_ptr (all child classes have been renamed too).
 * * bn::sprite_affine_mat_attributes renamed to bn::affine_mat_attributes.
 * * bn::bg_tiles_ptr renamed to bn::regular_bg_tiles_ptr.
 * * bn::bg_tiles_item renamed to bn::regular_bg_tiles_item.
 * * bn::affine_mat_scale_lut replaced with bn::reciprocal_lut.
 * * bn::core::cpu_usage() and bn::core::vblank_usage() renamed to
 *   bn::core::last_cpu_usage() and bn::core::last_vblank_usage().
 * * bn::lut_reciprocal() added.
 * * bn::core::current_cpu_usage() added.
 * * <a href="https://github.com/rodri042/gba-link-connection">gba-link-connection</a> updated to v2.1.4.
 * * HDMA is disabled when needed, like before sleeping or before an assert screen.
 * * bn::regular_bg_tiles_item::valid_tiles_count() fixed.
 * * bn::nullopt documentation links fixed.
 *
 *
 * @section changelog_3_3_0 3.3.0
 *
 * * HDMA properly supported (now it works at less than 60fps). See bn::hdma and the `hdma_polygons` example for more.
 * * <a href="https://github.com/rodri042/gba-link-connection">gba-link-connection</a> remote timeout detection fixed.
 *
 *
 * @section changelog_3_2_1 3.2.1
 *
 * bn::optional build fix.
 *
 *
 * @section changelog_3_2_0 3.2.0
 *
 * bn::optional is now constexpr.
 *
 *
 * @section changelog_3_1_0 3.1.0
 *
 * * bn::regular_bg_map_ptr::tiles_offset() and bn::regular_bg_map_ptr::palette_banks_offset() methods added.
 * * Allocated background tiles and maps commit fixed.
 *
 *
 * @section changelog_3_0_0 3.0.0
 *
 * Thanks to the awesome <a href="https://github.com/rodri042/gba-link-connection">gba-link-connection</a>,
 * multiplayer support has been implemented! See bn::link and the `link` example for more.
 *
 *
 * @section changelog_2_0_0 2.0.0
 *
 * * By removing some method overloads, lots of runtime asserts when creating resources have been removed.
 * * bn::palette_bpp_mode has been renamed to bn::bpp_mode and `bpp_mode()` methods have been renamed to `bpp()`.
 * * 8 bits per pixel background tiles allocation fixed.
 *
 *
 * @section changelog_1_0_0 1.0.0
 *
 * * Regular maps bigger than 512x512 supported.
 * * 32bit integer division by 0 assert added.
 * * bn::regular_bg_item maximum tiles check added.
 * * bn::regular_bg_item generation options fixed.
 *
 *
 * @section changelog_0_4_0 0.4.0
 *
 * * `btn` renamed to `bn`. No more API breaks will be made between minor releases after 1.0.0, promise.
 * * Background tiles manager status can be printed in the log with bn::bg_tiles::log_status().
 *   This is done automatically when a non-optional background tiles allocation fails too.
 * * Background regular maps manager status can be printed in the log with bn::bg_maps::log_status().
 *   This is done automatically when a non-optional regular background map allocation fails too.
 * * Sprite tiles manager status can be printed in the log with bn::sprite_tiles::log_status().
 *   This is done automatically when a non-optional sprite tiles allocation fails too.
 * * Color palettes managers status can be printed in the log with
 *   bn::bg_palettes::log_status() and bn::sprite_palettes::log_status().
 *   This is done automatically when a non-optional color palette allocation fails too.
 * * Sprites destruction optimized.
 * * Setters with an optional parameter added to some classes.
 * * Optional components documentation fixed.
 * * Other documentation improvements.
 *
 *
 * @section changelog_0_3_0 0.3.0
 *
 * * Sprites update performance improved up to 30% in Butano Fighter
 *   thanks to avoid rebuilding sprites list as much as possible.
 * * Profiler can show the maximum measured ticks per entry.
 * * Assets tools print output binaries size.
 *
 *
 * @section changelog_0_2_0 0.2.0
 *
 * * Performance improved up to 12% in Butano Fighter without `-flto` thanks to using less build translation units.
 * * Documentation improved.
 *
 *
 * @section changelog_0_1_0 0.1.0
 *
 * First release.
 */


#ifdef BN_DOXYGEN
    #ifndef BN_CFG_ASSERT_ENABLED
        #define BN_CFG_ASSERT_ENABLED true
    #endif

    #ifndef BN_CFG_LOG_ENABLED
        #define BN_CFG_LOG_ENABLED true
    #endif

    #ifndef BN_CFG_PROFILER_ENABLED
        #define BN_CFG_PROFILER_ENABLED true
    #endif
#endif

#endif
