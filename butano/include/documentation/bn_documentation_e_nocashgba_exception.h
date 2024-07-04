/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DOCUMENTATION_NOCASHGBA_EXCEPTION_H
#define BN_DOCUMENTATION_NOCASHGBA_EXCEPTION_H

/**
 * @page nocashgba_exception No$gba exception setup guide
 *
 * @tableofcontents
 *
 * <a href="https://problemkaputt.de/gba.htm">No$gba</a> exception system allows to catch common programming errors.
 *
 * When an exception is triggered, No$gba stops the execution of the ROM and shows the assembler instruction
 * that has triggered the exception.
 *
 * If you load an `*.elf` file instead of a `*.gba` file, it also shows the high level code that has triggered the exception.
 * Please remember that `*.elf` support only works with <a href="https://devkitpro.org/">devkitARM</a>,
 * it doesn't work if you're using <a href="https://wonderful.asie.pl/">Wonderful Toolchain</a>.
 *
 *
 * @section nocashgba_exception_getting_started Getting started
 *
 * Download the "no$gba Windows debug version" from <a href="https://problemkaputt.de/gba.htm">here</a>.
 * The gaming versions can't show the code that has triggered an exception, they only show the number of exceptions detected.
 *
 *
 * @section nocashgba_exception_enable Enable exceptions
 *
 * Exceptions are disabled by default. You need to enable them in the `Xcept` tab of the setup menu:
 *
 * @image html nocashgba/nocashgba_0.png
 *
 *
 * @section nocashgba_exception_example Exception example
 *
 * Let's trigger an exception by dereferencing a null pointer:
 *
 * @code{.cpp}
 * #include "bn_core.h"
 *
 * int main()
 * {
 *     bn::core::init();
 *     int skip_frames = 1;
 *     int* ptr = nullptr;
 *     skip_frames += *ptr;
 *     bn::core::set_skip_frames(skip_frames);
 *
 *     while(true)
 *     {
 *         bn::core::update();
 *     }
 * }
 * @endcode
 *
 * If we build the project and we run the `*.elf` file with No$gba, it should stop the execution
 * and display the following warning:
 *
 * @image html nocashgba/nocashgba_1.png
 *
 * As we can see, it also shows the C++ code that has triggered the exception:
 *
 * @image html nocashgba/nocashgba_2.png
 */

#endif
