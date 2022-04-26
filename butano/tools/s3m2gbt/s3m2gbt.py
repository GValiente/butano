#!/usr/bin/env python3

# s3m2gbt v4.0.1 (Part of GBT Player)
#
# SPDX-License-Identifier: MIT
#
# Copyright (c) 2022 Antonio Niño Díaz <antonio_nd@outlook.com>

import argparse
from s3m2gbt import kaitaistruct
import sys

from s3m2gbt.s3m import S3m

# Channels 1, 2, 4
def s3m_volume_to_gb(s3m_vol):
    if s3m_vol >= 64:
        return 15
    else:
        return s3m_vol >> 2;

# Channel 3
def s3m_volume_to_gb_ch3(s3m_vol):
    vol = s3m_volume_to_gb(s3m_vol)

    if vol >= 0 and vol <= 3:
        return 0 # 0%
    elif vol >= 4 and vol <= 6:
        return 3 # 25%
    elif vol >= 7 and vol <= 9:
        return 2 # 50%
    elif vol >= 10 and vol <= 12:
        return 4 # 75%
    elif vol >= 13 and vol <= 15:
        return 1 # 100%
    else:
        return 0

def s3m_note_to_gb(note, pattern, step, channel):

    # This function shouldn't be called for channel 4
    assert channel != 4

    # TODO: s3mNoteOff = 0xFE || s3mNoteNone = 0xFF ?
    assert note <= 0x7F

    note -= 32
    if note < 0:
        print("ERROR: Note too low!")
        print(f"  {pattern}-{step}:Ch {channel}")
        sys.exit(1)
    elif note > 32 + 16 * 6:
        print("ERROR: Note too hig!")
        print(f"  {pattern}-{step}:Ch {channel}")
        sys.exit(1)

    note = (note & 0xF) + ((note & 0xF0) >> 4) * 12
    return note

def s3m_pan_to_gb(pan, channel):
    left = False
    right = False

    if pan >= 0 and pan <= 3:
        left = True
    elif pan >= 4 and pan <= 11:
        left = True
        right = True
    elif pan >= 12 and pan <= 15:
        right = True

    val = 0
    if left:
        val |= 1 << (3 + channel)
    if right:
        val |= 1 << (channel - 1)

    return val

# Returns (converted_num, converted_params) if there was a valid effect. On
# error, it returns (None, None).
def effect_mod_to_gb(pattern_number, step_number, channel,
                     effectnum, effectparams):

    if effectnum == 'A': # Set Speed
        if effectparams == 0:
            print("WARN: Speed must not be zero.")
            print(f"  {pattern_number}-{step_number}:Ch {channel}")
            return (None, None)

        return (10, effectparams)

    if effectnum == 'B': # Pattern jump
        # TODO: Fail if this jumps out of bounds
        return (8, effectparams)

    elif effectnum == 'C': # Break + Set step
        # Effect value is BCD, convert to integer
        val = (((effectparams & 0xF0) >> 4) * 10) + (effectparams & 0x0F)
        return (9, val)

    elif effectnum == 'J': # Arpeggio
        return (1, effectparams)

    elif effectnum == 'S': # This effect is subdivided into many

        subeffectnum = (effectparams & 0xF0) >> 4
        subeffectparams = effectparams & 0x0F

        if subeffectnum == 0x8: # Pan position
            val = s3m_pan_to_gb(subeffectparams, channel)
            return (0, val)

        if subeffectnum == 0xC: # Notecut
            return (2, subeffectparams)

    print(f"WARN: Unsupported effect: {effectnum}{effectparams:02X}")
    print(f"  {pattern_number}-{step_number}:Ch {channel}")

    return (None, None)

HAS_VOLUME      = 1 << 4
HAS_INSTRUMENT  = 1 << 5
HAS_EFFECT      = 1 << 6
HAS_NOTE        = 1 << 7
HAS_KIT         = 1 << 7

def convert_channel1(pattern_number, step_number,
                     note_index, samplenum, volume, effectnum, effectparams):
    command = [ 0, 0, 0, 0 ] # NOP
    command_ptr = 1

    # Check if it's needed to add a note
    if note_index != -1:
        note_index = s3m_note_to_gb(note_index, pattern_number, step_number, 1)
        command[0] |= HAS_NOTE
        command[command_ptr] = note_index & 0x7F
        command_ptr = command_ptr + 1

        # If a note is set with no volume, set volume to the max
        # TODO: This should take the volume from the sample volume
        if volume == -1:
            volume = 64

    # Check if there is a sample defined
    if samplenum > 0:
        instrument = samplenum & 3

        command[0] |= HAS_INSTRUMENT
        command[command_ptr] = (instrument << 4) & 0x30

    if effectnum is not None:
        [converted_num, converted_params] = effect_mod_to_gb(
                pattern_number, step_number, 1, effectnum, effectparams)

        if converted_num is not None:
            command[0] |= HAS_EFFECT
            command[command_ptr] |= converted_num & 0x0F
            command_ptr += 1
            command[command_ptr] = converted_params & 0xFF

    # Check if it's needed to add a volume
    if volume > -1:
        command[0] |= HAS_VOLUME
        command[0] |= s3m_volume_to_gb(volume) & 0x0F

    # Note: The volume bit doesn't affect the final size.
    sizes = [ 1, 2, 3, 3, 2, 3, 4, 4 ]
    command_size = sizes[command[0] >> 5]

    return command[:command_size]

def convert_channel2(pattern_number, step_number,
                     note_index, samplenum, volume, effectnum, effectparams):
    command = [ 0, 0, 0, 0 ] # NOP
    command_ptr = 1

    # Check if it's needed to add a note
    if note_index != -1:
        note_index = s3m_note_to_gb(note_index, pattern_number, step_number, 2)
        command[0] |= HAS_NOTE
        command[command_ptr] = note_index & 0x7F
        command_ptr = command_ptr + 1

        # If a note is set with no volume, set volume to the max
        # TODO: This should take the volume from the sample volume
        if volume == -1:
            volume = 64

    # Check if there is a sample defined
    if samplenum > 0:
        instrument = samplenum & 3

        command[0] |= HAS_INSTRUMENT
        command[command_ptr] = (instrument << 4) & 0x30

    if effectnum is not None:
        [converted_num, converted_params] = effect_mod_to_gb(
                pattern_number, step_number, 2, effectnum, effectparams)

        if converted_num is not None:
            command[0] |= HAS_EFFECT
            command[command_ptr] |= converted_num & 0x0F
            command_ptr += 1
            command[command_ptr] = converted_params & 0xFF

    # Check if it's needed to add a volume
    if volume > -1:
        command[0] |= HAS_VOLUME
        command[0] |= s3m_volume_to_gb(volume) & 0x0F

    # Note: The volume bit doesn't affect the final size.
    sizes = [ 1, 2, 3, 3, 2, 3, 4, 4 ]
    command_size = sizes[command[0] >> 5]

    return command[:command_size]

def convert_channel3(pattern_number, step_number,
                     note_index, samplenum, volume, effectnum, effectparams):
    command = [ 0, 0, 0, 0 ] # NOP
    command_ptr = 1

    # Check if it's needed to add a note
    if note_index != -1:
        note_index = s3m_note_to_gb(note_index, pattern_number, step_number, 3)
        command[0] |= HAS_NOTE
        command[command_ptr] = note_index & 0x7F
        command_ptr = command_ptr + 1

        # If a note is set with no volume, set volume to the max
        # TODO: This should take the volume from the sample volume
        if volume == -1:
            volume = 64

    # Check if there is a sample defined
    if samplenum > 0:
        instrument = samplenum & 7

        command[0] |= HAS_INSTRUMENT
        command[command_ptr] = (instrument << 4) & 0xF0

    if effectnum is not None:
        [converted_num, converted_params] = effect_mod_to_gb(
                pattern_number, step_number, 3, effectnum, effectparams)

        if converted_num is not None:
            command[0] |= HAS_EFFECT
            command[command_ptr] |= converted_num & 0x0F
            command_ptr += 1
            command[command_ptr] = converted_params & 0xFF

    # Check if it's needed to add a volume
    if volume > -1:
        command[0] |= HAS_VOLUME
        command[0] |= s3m_volume_to_gb_ch3(volume) & 0x0F

    # Note: The volume bit doesn't affect the final size.
    sizes = [ 1, 2, 3, 3, 2, 3, 4, 4 ]
    command_size = sizes[command[0] >> 5]

    return command[:command_size]

def convert_channel4(pattern_number, step_number,
                     note_index, samplenum, volume, effectnum, effectparams):
    command = [ 0, 0, 0, 0 ] # NOP
    command_ptr = 1

    # Check if there is a sample defined
    if samplenum > 0:
        kit = samplenum & 0xF;
        command[0] |= HAS_KIT
        command[command_ptr] = kit & 0xF
        command_ptr += 1

        # If a note is set with no volume, set volume to the max
        # TODO: This should take the volume from the sample volume
        if volume == -1:
            volume = 64

    if effectnum is not None:
        [converted_num, converted_params] = effect_mod_to_gb(
                pattern_number, step_number, 4, effectnum, effectparams)

        if converted_num is not None:
            command[0] |= HAS_EFFECT
            command[command_ptr] |= converted_num & 0x0F
            command_ptr += 1
            command[command_ptr] = converted_params & 0xFF

    # Check if it's needed to add a volume
    if volume > -1:
        command[0] |= HAS_VOLUME
        command[0] |= s3m_volume_to_gb(volume) & 0x0F

    # Note: The volume bit doesn't affect the final size.
    sizes = [ 1, 2, 3, 3, 2, 3, 4, 4 ]
    command_size = sizes[command[0] >> 5]

    return command[:command_size]

def print_step(fileout, cmd1, cmd2, cmd3, cmd4):
    fileout.write("    ")

    for b in cmd1:
        fileout.write(f"0x{b:02X},")
    for b in cmd2:
        fileout.write(f"0x{b:02X},")
    for b in cmd3:
        fileout.write(f"0x{b:02X},")
    for b in cmd4:
        fileout.write(f"0x{b:02X},")

    fileout.write("\n")

def initial_state_array(speed, panning_array):
    array = []

    array.extend([1, speed])

    array.extend([2])
    array.extend(panning_array)

    array.extend([0]) # End

    return array

def convert_file(module_path, song_name, output_path):

    data = S3m.from_file(module_path)

    if output_path == None:
        output_path = song_name + ".c"

    fileout = open(output_path, "w")

    name = data.song_name.decode("utf-8")
    print(f"Song Name: '{name}'")
    print(f"Num. Orders: {data.num_orders}")
    print(f"Num. Patterns: {data.num_patterns}")

    #for i in data.instruments:
    #    try:
    #        print(vars(i.body))
    #    except kaitaistruct.ValidationNotEqualError as e:
    #        if e.src_path == u"/types/instrument/seq/6":
    #            print("Invalid magic in instrument")
    #        else:
    #            print(vars(e))

    fileout.write("// File created by s3m2gbt\n\n"
                  "#include <stddef.h>\n#include <stdint.h>\n\n")

    # Export patterns
    # ---------------

    print(f"Exporting patterns...")

    pattern = 0
    for p in data.patterns:

        fileout.write(f"static const uint8_t {song_name}_{pattern}[] = ")
        fileout.write("{\n")

        step = 0
        try:
            cmd1 = [0]
            cmd2 = [0]
            cmd3 = [0]
            cmd4 = [0]

            for c in p.body.body.cells:

                if c.channel_num == 0 and (not c.has_volume) and \
                    (not c.has_fx) and (not c.has_note_and_instrument):

                    print_step(fileout, cmd1, cmd2, cmd3, cmd4)

                    step = step + 1

                    # Clear commands
                    cmd1 = [0]
                    cmd2 = [0]
                    cmd3 = [0]
                    cmd4 = [0]

                volume = -1
                if c.has_volume:
                    volume = c.volume

                note = -1
                instrument = 0
                if c.has_note_and_instrument:
                    note = c.note
                    instrument = c.instrument

                effectnum = None
                effectparams = None
                if c.has_fx:
                    # Convert type to ASCII to match the documentation
                    effectnum = chr(c.fx_type + ord('A') - 1)
                    effectparams = c.fx_value

                if c.channel_num == 0:
                    cmd1 = convert_channel1(pattern, step,
                                            note, instrument, volume,
                                            effectnum, effectparams)
                elif c.channel_num == 1:
                    cmd2 = convert_channel2(pattern, step,
                                            note, instrument, volume,
                                            effectnum, effectparams)
                elif c.channel_num == 2:
                    cmd3 = convert_channel3(pattern, step,
                                            note, instrument, volume,
                                            effectnum, effectparams)
                elif c.channel_num == 3:
                    cmd4 = convert_channel4(pattern, step,
                                            note, instrument, volume,
                                            effectnum, effectparams)
                else:
                    print(f"Too many channels: {c.channel_num}")

        except kaitaistruct.ValidationNotEqualError as e:
            print(vars(e))

        fileout.write("};\n")
        fileout.write("\n")

        pattern += 1

    # Export initial state
    # --------------------

    print(f"Exporting initial state...")

    fileout.write(f"const uint8_t {song_name}_init_state[] = ")
    fileout.write("{\n")

    default_pan = [8, 8, 8, 8]
    if data.has_custom_pan == 252:
        print("Song contains custom panning values")
        for i in range(0, 4):
            if data.channel_pans[i].has_custom_pan:
                default_pan[i] = data.channel_pans[i].pan

    gb_default_pan = [
        s3m_pan_to_gb(default_pan[0], 1),
        s3m_pan_to_gb(default_pan[1], 2),
        s3m_pan_to_gb(default_pan[2], 3),
        s3m_pan_to_gb(default_pan[3], 4)
    ]

    state_array = initial_state_array(data.initial_speed, gb_default_pan)
    fileout.write("    ")
    for s in state_array:
        fileout.write(f"0x{s:02X},")
    fileout.write("\n")
    fileout.write("};\n")
    fileout.write("\n")

    # Export orders
    # -------------

    print(f"Exporting orders...")

    fileout.write(f"const uint8_t *{song_name}[] = ")
    fileout.write("{\n")

    fileout.write(f"    {song_name}_init_state,")
    fileout.write("\n")

    for o in data.orders:
        pattern = int(o)
        if pattern >= data.num_patterns:
            # TODO: Warn if the pattern goes over the limit?
            continue
        fileout.write(f"    {song_name}_{pattern},")
        fileout.write("\n")

    fileout.write("    NULL\n")
    fileout.write("};\n")

    fileout.close()

    # All done!
    # ---------

    print(f"Done!")

if __name__ == "__main__":

    print("s3m2gbt v4.0.1 (part of GBT Player)")
    print("Copyright (c) 2022 Antonio Niño Díaz <antonio_nd@outlook.com>")
    print("Copyright (c) 2015-2022 Kaitai Project")
    print("All rights reserved")
    print("")

    parser = argparse.ArgumentParser(description='Convert S3M files into GBT format.')
    parser.add_argument("--input", default=None, required=True, help="input file")
    parser.add_argument("--name", default=None, required=True, help="output song name")
    parser.add_argument("--output", default=None, required=False, help="output file")

    args = parser.parse_args()

    convert_file(args.input, args.name, args.output)
