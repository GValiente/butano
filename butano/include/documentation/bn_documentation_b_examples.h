/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DOCUMENTATION_EXAMPLES_H
#define BN_DOCUMENTATION_EXAMPLES_H

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
 * <td> @ref date_time
 *
 * @image html examples_date_time.png
 *
 * Real time clock (RTC) showcase (if your cart has one).
 *
 * <td> **Dynamic affine background**
 *
 * @image html examples_dynamic_affine_bg.png
 *
 * Basic dynamic affine background example.
 *
 * <tr><td> **Dynamic regular background**
 *
 * @image html examples_dynamic_regular_bg.png
 *
 * Basic dynamic regular background example.
 *
 * <td> **External tool**
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
 * <tr><td> @ref keypad
 *
 * @image html examples_keypad.png
 *
 * Simple keypad status demo.
 *
 * <td> @ref link
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
 * <tr><td> **Map collision**
 *
 * @image html examples_map_collision.png
 *
 * Sprite against map collision example.
 *
 * <td> **Mode 7**
 *
 * @image html examples_mode_7.png
 *
 * Basic SNES mode 7 like background example.
 *
 * <td> @ref mosaic
 *
 * @image html examples_mosaic.png
 *
 * Sprites and backgrounds mosaic effects.
 *
 * <tr><td> @ref palette
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
 * <td> @ref profiler
 *
 * @image html examples_profiler.png
 *
 * Basic code blocks profiling.
 *
 * <tr><td> @ref regular_bg
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
 * <td> @ref affine_mat "Sprite affine matrices"
 *
 * @image html examples_sprite_affine_mats.png
 *
 * Demonstrates usage of one affine matrix for multiple sprites.
 *
 * <tr><td> @ref sprite
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
 * <td> @ref text
 *
 * @image html examples_text.png
 *
 * Showcase of Butano sprite text rendering.
 *
 * <tr><td> **Textured polygons**
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
 * <td> **World map**
 *
 * @image html examples_world_map.png
 *
 * Showcase of a 4096x4096 world map with a perspective effect.
 *
 * </table>
 */

#endif
