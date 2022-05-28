s3m2gbt v4.4.1
==============

This tool can convert a special template of S3M into a GBT converted song.

You can't use any S3M song as input. Please, use one of the templates provided
as an example. The templates have the right samples to simulate the PSG channels
of the GBA, and they also have the right tempo (that corresponds to one update
of the player each frame, so ~60 updates per second).

Initial status
--------------

You can set the global song speed in the song settings, as well as the initial
panning for each channel. It isn't needed to set them by using them as effects.

Don't change the BPM of the song, it's set so that it matches the refresh
frequency of the GB/GBC/GBA.

The converter ignores global volumes. Sample default volumes are used when a
note an an instrument are provided, but no volume (as expected from a S3M file).

Frequencies
-----------

You can use notes from ``C3`` to ``B8``.

``^^`` note cuts are supported.

Volumes
-------

S3M supports values 0 to 64 as volume. However, in the GBA, it's different.
Channels 1, 2 and 4 have a range of 0 to 15, and channel 3 can have the
following levels: 0%, 25%, 50%, 75% and 100%.

When the conversion is done, the converter will get the closest value that is
valid for that specific channel.

Samples
-------

You are allowed to replace the samples of the channel 3. If the resulting sample
is exactly 32 or 64 samples long, you can pass the ``--instruments`` flag to
``s3m2gbt`` and it will export all valid instruments along the patterns of your
song. They will replace the default instruments of GBT Player for the duration
of the song.

You are allowed to change the default volume of the samples, but don't modify
the tuning of the samples!

The samples of the template are the corresponding ones to the default
instruments of GBT Player. If you export the instruments of the template, the
resulting instruments are the same.

The valid samples for each channel are:

- Channel 1 and 2: Samples 1 to 4

- Channel 3: Samples 8 to 15. They can be modified.

- Channel 4: Samples 16 to 31. You should always use ``C5`` in your S3M song,
  the sound is always the same in the GB and changing the note in the S3M song
  won't have any effect.

The volume of channel 3 is always lower than other channels because the waveform
of channels 1, 2 and 4 use the full range of allowed values (0% or 100%) but
channel 3 can be set to any value in between. The area of any waveform of
channel 3 will always be smaller than the other channels (unless the wave of
channel 3 is a square wave).

Effects
-------

- ``Ann``: Sets speed to ``nn`` (in hexadecimal). Valid values are ``01`` and
  higher. The higher the value, the slower the song.

- ``Bnn``: Jump to pattern in order ``nn`` (in hexadecimal).

- ``Cnn``: Ends this pattern and jumps to position ``nn`` (in decimal) in next
  pattern.

- ``Dxy``: Volume slide. Only channels 1, 2, and 4. Notes:

  - The PSG channels can't stop the volume slide after it has started. You need
    to either set a new volume manually, or wait until it goes to the minimum or
    maximum volume.

  - This means that after the first ``Dxy`` effect you should add ``D00``
    effects in your pattern until the note is silent. The effects are ignored on
    the GBA, but they are needed so that the S3M playback matches the GBA
    playback. Don't repeat the original Dxy effect! It would sound ok in the S3M
    player, but in the GBA it wouldn't work.::

             OK       |      Not OK
        ------------------------------
        C3 01 v64 D06 |  C3 01 v64 D06
                  D00 |            D06
                  D00 |            D06
        C4 01 v64 --- |  C4 01 v64 ---

  - Only values 1-7 are allowed. (``D10`` to ``D70`` and ``D01`` to ``D07``).
    This is because the hardware volume slide of the PSG channels only has 7
    different speeds.  Software volume slides create a lot of clicks, so they
    sound terrible, and can't be used as an alternative.

  - Fine volume slide is not allowed (``DFy`` or ``DxF``).

- ``Hxy``: Vibrato. Only channels 1, 2 and 3.

- ``Jxy``: Arpeggio. Only channels 1, 2 and 3.

- ``S8n``: Sets the panning to ``n`` (in hexadecimal).::

      Left --- Both --- Right
      0123   456789AB    CDEF

- ``SCn`` - Cut Note after ``n`` ticks. If ``n > speed`` or ``n = 0``, it won't
  work.
