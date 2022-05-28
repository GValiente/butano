mod2gbt v4.4.1
==============

This tool can convert a special template of MOD into a GBT converted song.

You can't use any MOD song as input. Please, use one of the templates provided
as an example. The templates have the right samples to simulate the PSG channels
of the GBA, and they also have the right tempo (that corresponds to one update
of the player each frame, so ~60 updates per second).

IMPORTANT: The usage of MOD files has been deprecated in GBT Player. This tool
will remain part of GBT Player to keep old music files working, but no new
features will be added. There were some bugs with old versions of ``mod2gbt``
that can't be fixed or old songs will stop working. The only purpose of
``mod2gbt`` is to keep such old music working. For any new songs, use S3M files
(which is a better format than MOD, anyway) and ``s3m2gbt``.

Initial status
--------------

There is no initial status extracted from a MOD file.

Frequencies
-----------

You can use notes from ``C3`` to ``B8``.

Volumes
-------

MOD supports values 0 to 64 as volume. However, in the GBA, it's different.
Channels 1, 2 and 4 have a range of 0 to 15, and channel 3 can have the
following levels: 0%, 25%, 50%, 75% and 100%.

When the conversion is done, the converter will get the closest value that is
valid for that specific channel.

Samples
-------

You can only use the samples provided with the MOD template. The samples of the
template are the corresponding ones to the default instruments of GBT Player.

You are not allowed to change the default volume of the samples. Also, don't
modify the tuning of the samples!

The valid samples for each channel are:

- Channel 1 and 2: Samples 1 to 4

- Channel 3: Samples 8 to 15.

- Channel 4: Samples 16 to 31. You should always use ``C5`` in your MOD song,
  the sound is always the same in the GB and changing the note in the MOD song
  won't have any effect.

The volume of channel 3 is always lower than other channels because the waveform
of channels 1, 2 and 4 use the full range of allowed values (0% or 100%) but
channel 3 can be set to any value in between. The area of any waveform of
channel 3 will always be smaller than the other channels (unless the wave of
channel 3 is a square wave).

Effects
-------

- ``0xy``: Arpeggio. Only channels 1, 2 and 3.

- ``Bnn``: Jump to pattern in order ``nn`` (in hexadecimal).

- ``Cnn``: Sets the volume to ``nn`` (in hexadecimal). Valid values from ``00h``
  to ``40h``. Channel 3 can only be set to 0%, 25%, 50%, 75% and 100%. Others
  can be set in a range of 0 to 15.

- ``Dnn``: Ends this pattern and jumps to position ``nn`` (in decimal) in next
  pattern.

- ``E8n``: Sets the panning to ``n`` (in hexadecimal).::

      Left --- Both --- Right
      0123   456789AB    CDEF

- ``ECn``: Cut Note after ``n`` ticks. If ``n > speed`` or ``n = 0``, it won't
  work.

- ``Fnn``: Sets speed to ``nn`` (in hexadecimal). Valid values are ``01h`` to
  ``1Fh``. The higher the value, the slower the song. BPM speed not supported.
