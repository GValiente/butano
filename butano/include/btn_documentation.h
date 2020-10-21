/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_DOCUMENTATION_H
#define BTN_DOCUMENTATION_H

// NAMESPACE

/**
 * @namespace btn
 * @brief Modern C++ high level GBA engine
 */


// GROUPS

/**
 * @defgroup container Containers
 *
 * STL like containers with the capacity defined at compile time.
 *
 * They don't throw exceptions. Instead, asserts are used to ensure valid usage.
 */

/**
 * @defgroup vector Vector
 *
 * A vector with the capacity defined at compile time.
 *
 * @ingroup container
 */

/**
 * @defgroup sprite Sprites
 *
 * Small animated objects that can move freely from the background.
 *
 * In GBA's jargon they are called OBJs.
 */

/**
 * @defgroup assert Asserts
 *
 * Butano's assert system.
 *
 * It can be disabled by defining \a BTN_CFG_ASSERT_ENABLED as \a false.
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
  \subsection subsection1 The first subsection
  Text.
  \subsection subsection2 The second subsection
  More text.
*/

/*! \page page2 Another page
  Even more info.
*/

#endif
