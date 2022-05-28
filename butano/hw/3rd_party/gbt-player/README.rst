GBT Player
==========

Music player library and converter kit for the Game Boy family of consoles. It's
available for the Game Boy Advance as a C library. It's also avaliable for Game
Boy and Game Boy Color as a `RGBDS <https://github.com/gbdev/rgbds>`_ library.
There is also a legacy version that uses GBDK. If you're interested in the
changes introduced with each version, check the `changelog <./changelog.rst>`_.

On GBA it is possible to use GBT Player at the same time as other players like
Maxmod, check this `example <./gba/examples/combined_maxmod>`_.

Introduction
------------

**GBT Player** is a music creation environment for GB, GBC and GBA. It is formed
by:

- **GBT Player**: Library to play GBT format on GB, GBC or GBA.
- **s3m2gbt**: Converts S3M files into GBT (GameBoy Tracker) format.
- **mod2gbt**: Converts legacy MOD files into GBT (GameBoy Tracker) format.

It's the same idea as the old Lemon player for Game Boy, but greatly improved:
You take a MOD file that you can edit in any tracker, convert it with a special
converter to something that the Game Boy can understand, and play it there.

Because of this workflow, there are some limitations that are needed so that the
MOD/S3M file and the result on the GB/GBA are similar.

As of version 4.0.0 there is a big split between the GB/GBC version and the GBA
version. The GB version is stuck in version 3.X, and all the new changes are
added to the GBA because it's easier to prototype new features there.

- For GBA, the code is just in C, and it doesn't depend on any library apart from
  a few standard library headers.

- For GB/GBC, there is source code for **RGBDS**, the main option for Game Boy
  development (in my opinion). There is also legacy code for an old version that
  also supported **GBDK**, but it's mostly unmaintained. Note that there is no
  S3M support for GB/GBC, only for GBA.

GBT Player is open source, and it is licensed under the MIT license. That means
that you can use and modify it but you have to give credit for the original
work. It would be nice to you tell me if you use it, anyway. :)

About
-----

Licensed under the MIT license.

Copyright (C) 2009-2022 Antonio Niño Díaz

All rights reserved.

Email: antonio [underscore] nd [at] outlook [dot] com

Web: www.skylyrac.net

Latest version: https://github.com/AntonioND/gbt-player
