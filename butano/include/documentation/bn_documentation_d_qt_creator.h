/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DOCUMENTATION_QT_CREATOR_H
#define BN_DOCUMENTATION_QT_CREATOR_H

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
 * Before messing with Qt Creator, make sure that you are able to build and run Butano projects from the system console.
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

#endif
