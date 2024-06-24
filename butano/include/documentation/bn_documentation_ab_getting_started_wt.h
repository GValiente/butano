/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DOCUMENTATION_GETTING_STARTED_WT_H
#define BN_DOCUMENTATION_GETTING_STARTED_WT_H

/**
 * @page getting_started_wt Getting started with Wonderful Toolchain
 *
 * Downloading Butano and building their games and examples is easy and doesn't take too much time, pinky promise.
 *
 * @tableofcontents
 *
 *
 * @section getting_started_wt_supported_platforms Supported platforms
 *
 * <a href="https://wonderful.asie.pl/">Wonderful Toolchain</a> supports Windows and Unix-like platforms.
 *
 * If you want to develop on macOS, you should try to use a Linux virtual machine or
 * @ref getting_started "devkitARM" instead.
 *
 *
 * @section getting_started_wt_emulator GBA emulator
 *
 * Before anything, it is convenient to have a GBA emulator at hand,
 * so you don't have to test in real hardware each change you make in your project.
 *
 * For developing GBA games, <a href="https://mgba.io">mGBA</a>,
 * <a href="https://github.com/nba-emu/NanoBoyAdvance">NanoBoyAdvance</a>,
 * <a href="https://github.com/SourMesen/Mesen2">Mesen</a> and the debug version of
 * <a href="https://problemkaputt.de/gba.htm">No$gba</a> are recommended.
 *
 *
 * @section getting_started_wt_wt Wonderful Toolchain
 *
 * The next step is to download and install <a href="https://wonderful.asie.pl/">Wonderful Toolchain</a>
 * using <a href="https://wonderful.asie.pl/docs/getting-started/">this tutorial</a>.
 *
 * From now on, this guide assumes a Windows environment, but the steps to take for Unix-like platforms
 * should be similar.
 *
 * @image html wt_installer.png
 *
 * Once installed, open the Wonderful Toolchain Shell and run the following commands,
 * accepting the default options when prompted:
 *
 * @code{.cpp}
 * pacman -S make
 * wf-pacman -Syu
 * wf-pacman -S toolchain-gcc-arm-none-eabi
 * wf-pacman -S thirdparty-blocksds-toolchain
 * wf-pacman -S target-gba
 * @endcode
 *
 * @image html wt_pacman.png
 *
 *
 * @section getting_started_wt_python Python
 *
 * To execute some of the tools needed to include image and audio files in your project,
 * you are going to need <a href="https://www.python.org/">Python</a>.
 *
 * If you are going to install Python on Windows,
 * remember to add it to the `PATH` and to close all system console windows before installing it:
 *
 * @image html python_windows.png
 *
 * Both Python 2 and Python 3 are supported for now, but Python 2 is not going to be supported for too long.
 * Make sure you can execute Python from the Wonderful Toolchain Shell:
 *
 * @image html wt_python.png
 *
 * If you have an Ubuntu/Debian based system with `python3` instead of `python`, you can:
 * * Specify `python3` as the Python interpreter path in the `Makefile` of the project you are trying to build.
 * * Install `python-is-python3`: `sudo apt-get install python-is-python3`.
 *
 *
 * @section getting_started_wt_butano Butano
 *
 * Download or clone the latest Butano release from <a href="https://github.com/GValiente/butano">GitHub</a>
 * and put it in a path without spaces or anything weird, please.
 *
 *
 * @section getting_started_wt_examples Butano examples
 *
 * Butano contains multiple @ref examples "examples" of most aspects of the engine.
 *
 * Make sure that you are able to build and run some of them:
 * * They are located in `/path/to/butano/examples/`.
 * * To build a Butano example, open the Wonderful Toolchain Shell, `cd` to the folder of the example
 *   you want to build and type `make -j8` if your CPU has 8 cores, `make -j16` if it has 16 and so on.
 *
 * @image html wt_example_1.png
 *
 * When trying to build the `sprites` example, if everything went as expected,
 * a `sprites.gba` file should have been generated in the `sprites` folder,
 * and the system console should show something like this:
 *
 * @image html wt_example_2.png
 *
 *
 * @section getting_started_wt_template Make your own GBA game!
 *
 * If you were able to build and run some examples, congrats! Now you can make your own GBA game with Butano.
 * To do that:
 * * Copy the template project located in `/path/to/butano/template/` to a path without spaces or weird characters.
 * * Change in the `Makefile` the path in which Butano library folder is located.
 *   For example, if Butano is located in `/path/to/butano/`,
 *   `LIBBUTANO` variable should be set to `/path/to/butano/butano`.
 * * If you want, you can also change ROM title, ROM code and more in the `Makefile`.
 * * `cd` to the new folder.
 * * Type `make -j#cpu_cores#` again.
 *
 * If everything went as expected, a `*.gba` file should have been generated in the project folder!
 *
 * Now learn how to @ref import "import your assets" in the game and
 * also take a look at the <a href="modules.html">modules</a> page.
 * It's always a good idea to play around with the examples as they cover most of what Butano offers.
 * The @ref faq "frequently asked questions (FAQ)" are worth a look too, as they answer most of the initial questions.
 */

#endif
