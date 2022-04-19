GBT PLAYER v3.0.9
=================

Music player library and converter kit for Game Boy and Game Boy Color that can
be used with `RGBDS <https://github.com/bentley/rgbds>`__ (and, with
limitations, with GBDK). It is also available for Game Boy Advance as a C
library.

Licensed under the MIT license.

Copyright (C) 2009-2022 Antonio Niño Díaz.

All rights reserved.

Email: antonio_nd@outlook.com

Web:

- www.skylyrac.net/

Latest version:

- https://github.com/AntonioND/gbt-player

Introduction
------------

**GBT Player** is a music creation environment for GB, GBC and GBA. It is formed
by **mod2gbt**, which converts a mod file into a gbt (GameBoy Tracker) file, and
GBT Player, which will be used to play that song in the GB, GBC or GBA. It's the
same idea as the old Lemon player, but greatly improved. Because of this
workflow, there are a lot of limitations that the GB/GBA doesn't impose, but that
are needed so that the mod file and the result on the GB/GBA are similar.

``GBT Player`` is writen in 100% assembly for GB/GBC and in C for GBA. That
means that it is fast, it won't need a lot of CPU time (around 7%? in GB, and
barely noticeable in GBA), and you will have a lot of time for your game logic.

For GB/GBC, there is source code for **RGBDS**, the main option for Game Boy
development (in my opinion). There is also legacy code for an old version that
also supported **GBDK**, but it's mostly unmaintained.

For GBA, the code is just in C, and it doesn't depend on any library apart from
a few standard library headers.

``mod2gbt`` is writen in C, and should compile anywhere.

GBT Player is open source, and it is licensed under the MIT license. That means
that you can use and modify it but you have to give credit for the original
work. It would be nice to you tell me if you use it, anyway. :)

IMPORTANT NOTE: Version 1.x.x converted songs won't work with player version
2.0.0 or higher. The same happens with 2.x.x and version 3.0.0.  They have to be
converted again.

How to compile the example
--------------------------

Compile ``mod2gbt``. In Windows you can use a command line like:

``gcc -o mod2gbt.exe mod2gbt``

A Makefile for Linux has been included in case you are feeling lazy. Just do:

``make``

Put ``rgbasm``, ``rgbfix`` and ``rgblink`` in the ``gbt-player/gb`` folder and
run the bat/sh file in the ``rbgds_example`` folder. A compiled GB binary is
included.

Notes
-----

A nice tracker to modify the mod file is **OpenMPT**. You can download it here:

http://openmpt.org/

I don't use bat or sh files in my projects, I use makefiles, but I thought that
the examples would be too simple for a makefile to be useful.

``instr_test.gb`` is a sample of the default sounds.

``range_test.gb`` is just a test of what notes the GB can reach (C3 - B8).

``effects_test.gb`` tests arpeggio and "cut note" effects.

The mod file isn't 100% accurate. Sounds are a bit different from the real ones,
so you should make roms and test them in emulators or real GB often.

If you don't like the speed convertion done by ``mod2gbt`` (from 50 Hz to 60 Hz)
you can use the ``-speed`` argument for ``mod2gbt``. The speed will be higher
and it will probably have to be adjusted manually.

The initial speed of the song is set by the start function, and it will run at
that speed until it finds a change speed command in the song. If the first step
of your song takes forever, this is the reason.

**For GB/GBC**: When creating **really** big ROMs (more than 4 MiB), the define
``GBT_USE_MBC5_512BANKS`` in ``gbt_player.inc`` should be uncommented to allow
allocation of the music data in banks higher than 255. Also, songs must be
converted adding ``-512-banks`` to the ``mod2gbt`` command line.

GBDK notes
----------

GBDK: http://gbdk.sourceforge.net/

``GBDK`` default assembler (``as-gbz80``) is no longer supported. ``RGBDS`` must
be used, and I don't really know if ``GBDK`` will work with latest ``RGBDS``
version. Adding ``-W--asm=rgbds`` to the command line when compiling ``GBDK``
code should make it work, but it hasn't been tested.

Since new functionality needs advanced macros like ``BANK()``, which are only
supported by ``RGBDS``, the ``GBDK`` version is discontinued. The latest version
that can be used with ``GBDK`` default assembler (2.1.2) is in the folder
``legacy_gbdk``, as well as the corresponding converter. I don't really know if
the same effect can be obtained with ``as-gbz80``. If it can be done, open an
issue and I may update ``GBDK`` version again. For now, this old legacy version
is mostly unmaintained.

This is an article I wrote related to bank switching:

http://www.skylyrac.net/2015-04-08-gbt-player-usage.html

To compile the ``GBDK`` example: Open the bat file, change it to set the correct
path to your ``lcc`` binary and double clic the bat.

Changelog
---------

- Version 3.0.9 (2022/4/18)

  - First version for GBA.
  - Directory structure reorganized.

- Version 3.0.8 and 2.1.3 (2020/4/21)

  - Fix effect ``Dnn`` in the last pattern of a song (Nolan Baker). Now it jumps
    to pattern 0 instead of crashing.
  - Relicense under the MIT license (rather than BSD-2-Clause).

- Version 3.0.7 (2019/6/3)

  - Fix panning in channel 4.

- Version 3.0.6 (2018/1/20)

  - Code cleanup.
  - Fix example code.
  - Increase version of ``mod2gbt`` to v3.1.
  - Some C fixes (Zalo)

- Version 2.1.2 (2018/1/20)

  - Cleanup of legacy version.
  - ``mod2gbt`` can no longer output assembly files. Set version number to v2.2.
  - Added license headers to all files so that it is easier to bundle code with
    other projects.

- Version 3.0.5 (2016/5/2)

  - Bit shift optimizations.

- Version 3.0.4 (2016/4/5)

  - Code reorganized and added license notices to source files.

- Version 3.0.3 (2016/2/6)

  - Code reorganized to fit in 80 columns.

- Version 3.0.2 (2015/5/3)

  - Corrected tabulations in RGBDS code.

- Version 3.0.1 (2015/4/27)
  - Replaced tabs by spaces in asm code.

- Version 3.0.0 (2015/4/22)

  - Added support for multiple bank songs.
  - ``GBDK`` default assembler (``as-gbz80``) version discontinued. Version
    2.1.1 will be kept in case someone wants to use it.
  - Previously converted songs must be converted again.
  - ``gbt_play()``: registers used for arguments have changed a bit.

- Version 2.1.1 (2015/4/7)

  - Simplified ``GBDK`` example because it was confusing a lot of people...

- Version 2.1.0 (2014/5/24)

  - Fixed arpeggio effect. Now it keeps looping until tick = 0 (previously it
    only looped once). It loops 3 steps, not 4.
  - Added "Cut Note" effect.
  - Effects optimizations.

- Version 2.0.1 (2014/5/23)

  - Fixed effects in channels 1, 2 and 3 in GBDK version.

- Version 2.0.0 (2014/5/22)

  - Rewritten library and converter.
  - Arpeggio effect added.
  - Song size should be reduced to about 60-75% (but it can go as high as 150%
    if it uses effects all time in all channels).
  - Old converted song data won't work, songs have to be converted again.

- Version 1.2.1 (2014/5/15)

  - Fixed ``Bnn`` command.

- Version 1.2 (2014/5/1)

  - Fixed lots of things regarding the mod file template and ``mod2gbt``, the
    converter. Old songs won't be converted right with this new version. You
    should copy pattern data into the new mod template and transpose it 17
    semitones to make it work again.
  - Fixed a typo in a variable name.

- Version 1.1 (2013)

  - Fixed definitions for enabling and disabling channels.
  - Changed email address.

- Version 1.0 (2009)

  - Initial release

To Do
-----

- Store channel 3 samples in RAM to be able to change them in execution time by
  the user?
- End song callback? Special effect for callback? To synchronize game events or
  things like that.
- ``WLA-DX`` version.

Known bugs
----------

- Effect ``Dxx``, when used the last step of a pattern, will jump 2 patterns
  instead of 1.

License
-------

```
Copyright (c) 2009-2022 Antonio Niño Díaz <antonio_nd@outlook.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
