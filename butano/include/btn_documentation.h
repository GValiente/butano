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
 * Used to apply 2D geometric transformations like rotating and/or scaling to sprites and backgrounds.
 */

/**
 * @defgroup sprite Sprites
 *
 * Small animated objects that can move freely from the background.
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
 * @defgroup hblank_effect H-Blank effects
 *
 * They allow to change how things are drawn in each screen horizontal line,
 * since they transfer data to the GBA during each horizontal blanking (H-Blank) period.
 *
 * For example, with a H-Blank effect you can change the transparent color every screen horizontal line,
 * which is how games like Chrono Trigger fills their menus with a color gradient.
 */

/**
 * @defgroup assert Asserts
 *
 * Butano's assert system.
 *
 * It can be disabled by defining \a BTN_CFG_ASSERT_ENABLED as \a <b>false</b>.
 *
 * Note that these asserts can be used in constexpr contexts (is_constant_evaluated() returns <b>true</b>).
 */

/**
 * @defgroup memory Memory
 *
 * Memory management utilities.
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
 * @defgroup other Other
 *
 * Stuff which doesn't fit anywhere else.
 */


// PAGES

/*! \mainpage My Personal Index Page
 *
 * \section intro_sec Introduction
 *
 * This is the introduction.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Opening the box
 *
 * etc...
 */

/*! \page page1 A documentation page
  \tableofcontents
  Leading text.
  \section sec An example section
  This page contains the subsections \ref subsection1 and \ref subsection2.
  For more info see page \ref page2.
  \subsection subsection1 First subsection
  Text.
  \subsection subsection2 Second subsection
  More text.
*/

/*! \page page2 Another page
  Even more info.
*/

#endif
