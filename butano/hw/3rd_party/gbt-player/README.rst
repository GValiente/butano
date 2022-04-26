GBT Player v4.0.1
=================

Music player library and converter kit for Game Boy Advance. It only uses the
PSG music channels, so the CPU usage is nearly 0%, but all sounds that can be
used are the ones you would expect from a Game Boy or Game Boy Color.

This is very different from other music players for the GBA like
`Maxmod <https://maxmod.devkitpro.org/>`__, which only uses the DMA music
channels. This causes it to use a lot of CPU, but it can play any sounds without
restrictions.

It is possible to use both libraries at the same time, if you want. GBT Player
only uses the hardware registers associated to the PSG channels, leaving the DMA
channels untouched and available for the user.

How to compile the example
--------------------------

First, install `devkitPro <https://devkitpro.org/wiki/Getting_Started>`__. This
is needed to build ROMs. Once it's installed you can continue.

The following will convert a S3M file called ``template.s3m`` into the right
format and build a sample ROM:

1. Open a terminal

2. Go to the ``example`` folder.

3. Run ``sh make_rom.sh`` on your terminal.

Legacy songs in MOD format
--------------------------

You can also use MOD files instead of S3M, but it is highly discouraged. This
should only be done if you are using songs that were already in MOD format and
don't want to convert them to S3M.

You will need to compile and use ``mod2gbt``:

1. Go to the ``mod2gbt`` folder in a terminal.

2. Type ``make``

This converter tries to imitate bugs present in v3.X (like incorrect volume
handling when a note is set but no volume is set).

Notes
-----

A nice tracker to modify the mod file is **OpenMPT**. You can download it here:

http://openmpt.org/

- ``instr_test.gb`` is a sample of the default sounds.

- ``range_test.gb`` is just a test of what notes the GB can reach (C3 - B8).

- ``effects_test.gb`` tests arpeggio and "cut note" effects.

- ``volume_test.gb`` tests the range of volumes supported by each channel.

The MOD and S3M files aren't 100% accurate. Sounds are a bit different from the
real ones, so you should make ROMs and test them in emulators or real GBAs
often.

To Do
-----

- Store channel 3 samples in RAM to be able to change them in execution time by
  the user?
- End song callback? Special effect for callback? To synchronize game events or
  things like that.

About
-----

Licensed under the MIT license.

Copyright (C) 2009-2022 Antonio Niño Díaz

All rights reserved.

Email: antonio_nd@outlook.com

Web: www.skylyrac.net/

Latest version: https://github.com/AntonioND/gbt-player
