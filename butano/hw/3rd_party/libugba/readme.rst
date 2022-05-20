Universal GBA Library
=====================

1. Introduction
---------------

This is a library for development of GBA games. It can be used to build actual
GBA game ROMs, but it can also target a regular PC so that it is easier to debug
the logic in the game. It is written in C.

This means you can do things like:

- Build a GBA ROM and run it in an emulator.
- Build a Linux executable file and debug it using GDB.
- Easily implement unit tests for your game and run them on your PC.

It comes with several examples of how to use each one of the subsystems of the
library. All examples are also used as unit tests, and they are part of the
automated regresion testing system of the library.

Also note that most of the library is optional and is included for conveniency.
The only interfaces that you must use in your project are described in `this
document <docs/interfaces.rst>`_.

Note that this is still an early version of the library. The public interface is
likely to change a bit, so expect breaking changes at some point. Hopefully,
they will be minimal, and the interfaces described in the link above will most
likely stay the same even if the internal code changes. The SDL2 port needs a
pretty big cleanup.

The GBA version of the library is a static library licensed under the MIT
license. The PC version of the library is a dynamic library licensed under the
LGPL-3.0 license. The other license files of the repository correspond to some
resources used in the examples (like music and sound effects), but don't affect
the library itself.

This repository only holds the library code. For more information about testing
check the following repository: https://github.com/AntonioND/ugba-testing

The following repository contains an example of a project that uses **libugba**:
https://github.com/AntonioND/ugba-template

Another example is my game, **µCity Advance**:
https://github.com/AntonioND/ucity-advance

2. Limitations
--------------

The PC version of the library behaves in a different way as the GBA version of
it. Take a look at `here <docs/limitations.rst>`_ for more details.

For a list of planned features, check `this link <docs/to-do.rst>`_.

3. Dependencies
---------------

To generate PC executables:

**Required**

- **GCC**, **Clang** or another compiler supported by CMake.
- **CMake 3.15** or later
- **SDL2 2.0.7** or later (AudioStream support needed)

**Optional**

For the SDL port:

- **libpng 1.6** or later (Simplified API support needed): Needed to save PNGs
  like screenshots and dumps from VRAM debugger windows.
- **liblua 5.2** or later (Integer support needed): Needed for the Lua
  interpreter used for the unit tests.

To generate the GBA library file you need one of:

- A ``gcc-arm-none-eabi`` toolchain: You can get it from your package manager,
  or from `Arm's GNU toolchain downloads website`_. In Ubuntu you can just do:

.. code:: bash

    sudo apt install gcc-arm-none-eabi

- `devkitPro`_: You need to install devkitPro following the instructions in the
  `devkitPro Getting Started guide`_, then follow the instructions in this
  readme. It comes with its own build of ``gcc-arm-none-eabi``.

4. Build library
----------------

Building PC library
^^^^^^^^^^^^^^^^^^^

If you're on Linux or any Linux-like environment (like MinGW or Cygwin), install
the dependencies using your package manager. For example, in Debian or Ubuntu:

.. code:: bash

    sudo apt install libsdl2-dev liblua5.4-dev libpng-dev

Clone this repository:

.. code:: bash

    git clone https://github.com/AntonioND/libugba
    cd libugba

Run this to build the library:

.. code:: bash

    mkdir build
    cd build
    cmake .. -DBUILD_GBA=OFF
    make -j`nproc`

Building GBA library with devkitPro
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Clone this repository and build it with just **make** like this:

.. code:: bash

    git clone https://github.com/AntonioND/libugba
    cd libugba
    make -j`nproc`

You can also build it with **CMake** (at the sime time as the PC version):

.. code:: bash

    git clone https://github.com/AntonioND/libugba
    cd libugba
    mkdir build
    cd build
    cmake ..
    make -j`nproc`

Building GBA library without devkitPro
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Run the following commands to build the GBA and PC versions at the same time:

.. code:: bash

    git clone https://github.com/AntonioND/libugba
    cd libugba
    mkdir build
    cd build
    cmake .. -DUSE_DEVKITARM=OFF
    make -j`nproc`

To override the autodetected location of the cross compiler, you can add
``-DARM_GCC_PATH=/path/to/folder/`` to the ``cmake`` command.

5. Regenerating font
--------------------

If you want to replace the font used for the console, go to ``source/graphics``
and run the ``convert.sh`` script.

6. Acknowledgements
-------------------

- Dave Murphy (WinterMute) (and others) for devkitPro and devkitARM.
- Jasper Vijn (cearn) for Tonc.
- Martin Korth (Nocash) for no$gba and GBATEK.
- Vicki Pfau (endrift) for mGBA.

.. _Arm's GNU toolchain downloads website: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads
.. _devkitPro Getting Started guide: https://devkitpro.org/wiki/Getting_Started
.. _devkitPro: https://devkitpro.org/
