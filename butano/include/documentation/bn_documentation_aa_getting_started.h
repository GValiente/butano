/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DOCUMENTATION_GETTING_STARTED_H
#define BN_DOCUMENTATION_GETTING_STARTED_H

/**
 * @page getting_started Getting started
 *
 * Downloading Butano and building their games and examples is easy and doesn't take too much time, pinky promise.
 *
 * @tableofcontents
 *
 *
 * @section getting_started_emulator GBA emulator
 *
 * Before anything, it is convenient to have a GBA emulator at hand,
 * so you don't have to test each change you make to your project on real hardware.
 *
 * For developing GBA games, <a href="https://mgba.io">mGBA</a>,
 * <a href="https://github.com/nba-emu/NanoBoyAdvance">NanoBoyAdvance</a>,
 * <a href="https://github.com/SourMesen/Mesen2">Mesen</a> and the debug version of
 * <a href="https://problemkaputt.de/gba.htm">No$gba</a> are recommended.
 *
 *
 * @section getting_started_toolchain Toolchain
 *
 * Butano can be built on top of <a href="https://devkitpro.org/">devkitARM</a>
 * or <a href="https://wonderful.asie.pl/">Wonderful Toolchain</a>. Both are compatible with the same operating systems
 * (Windows, macOS and Unix-like platforms), so the differences between them aren't that big:
 * * <a href="https://problemkaputt.de/gba.htm">No$gba</a>'s @ref nocashgba_exception "exception system"
 *   with `*.elf` files only works with <a href="https://devkitpro.org/">devkitARM</a>.
 * * <a href="https://blocksds.skylyrac.net/maxmod/index.html">Maxmod</a>'s conversion tool provided by
 *   <a href="https://blocksds.skylyrac.net">BlocksDS</a> is more robust, so using Butano with
 *   <a href="https://wonderful.asie.pl/">Wonderful Toolchain</a> could improve the playback of your songs.
 *
 * So:
 * * If you want to use <a href="https://devkitpro.org/">devkitARM</a>, go to @ref getting_started_dka.
 * * If you want to use <a href="https://wonderful.asie.pl/">Wonderful Toolchain</a>, go to @ref getting_started_wt.
 */

#endif
