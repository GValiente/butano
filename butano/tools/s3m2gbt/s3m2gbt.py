#!/usr/bin/env python3

# s3m2gbt v4.2.0 (Part of GBT Player)
#
# SPDX-License-Identifier: MIT
#
# Copyright (c) 2022 Antonio Niño Díaz <antonio_nd@outlook.com>

from s3m2gbt import kaitaistruct

from s3m2gbt.s3m import S3m

class StepConversionError(Exception):
    def __init__(self, message, pattern = -1, step = -1, channel = -1):
        self.pattern = pattern
        self.step = step
        self.channel = channel + 1
        self.message = message

    def __str__(self):
        return f"Pattern {self.pattern} | Row {self.step} | Channel {self.channel} | {self.message}"

class S3MFormatError(Exception):
    pass

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

def s3m_note_to_gb(note):
    # Note cut with ^^
    if note == 0xFE:
        return 0xFE

    # Note off and ^^ note cut should be handled before reaching this point
    assert note <= 0x7F

    note -= 32
    if note < 0:
        raise StepConversionError("Note too low")
    elif note > 32 + 16 * 6:
        raise StepConversionError("Note too high")

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

EFFECT_PAN              = 0
EFFECT_ARPEGGIO         = 1
EFFECT_NOTE_CUT         = 2
EFFECT_VIBRATO          = 3
EFFECT_VOLUME_SLIDE     = 4
EFFECT_PATTERN_JUMP     = 8
EFFECT_BREAK_SET_STEP   = 9
EFFECT_SPEED            = 10

# Returns (converted_num, converted_params) if there was a valid effect. If
# there is none, it returns (None, None). Note that it is needed to pass the
# channel to this function because some effects behave differently depending on
# the channel (like panning).
def effect_s3m_to_gb(channel, effectnum, effectparams):

    if effectnum == 'A': # Set Speed
        if effectparams == 0:
            raise StepConversionError("Speed must not be zero")

        return (EFFECT_SPEED, effectparams)

    if effectnum == 'B': # Pattern jump
        # TODO: Fail if this jumps out of bounds
        return (EFFECT_PATTERN_JUMP, effectparams)

    elif effectnum == 'C': # Break + Set step
        # Effect value is BCD, convert to integer
        val = (((effectparams & 0xF0) >> 4) * 10) + (effectparams & 0x0F)
        return (EFFECT_BREAK_SET_STEP, val)

    elif effectnum == 'D': # Volume Slide
        if channel == 3:
            raise StepConversionError("Volume slide not supported in channel 3")

        if effectparams == 0:
            # Ignore volume slide commands that just continue the effect,
            # they are only needed for the S3M player.
            return (None, None)

        upper = (effectparams >> 4) & 0xF
        lower = effectparams & 0xF

        if upper == 0xF or lower == 0xF:
            raise StepConversionError("Fine volume slide not supported")

        elif lower == 0: # Volume goes up
            params = 1 << 3 # Increase
            delay = 7 - upper + 1
            if delay <= 0:
                raise StepConversionError("Volume slide too steep")
            params |= delay
            return (EFFECT_VOLUME_SLIDE, params)
        elif upper == 0: # Volume goes down
            params = 0 << 3 # Decrease
            delay = 7 - lower + 1
            if delay <= 0:
                raise StepConversionError("Volume slide too steep")
            params = delay
            return (EFFECT_VOLUME_SLIDE, params)
        else:
            raise StepConversionError("Invalid volume slide arguments")

        return (EFFECT_VOLUME_SLIDE, effectparams)

    elif effectnum == 'H': # Vibrato
        return (EFFECT_VIBRATO, effectparams)

    elif effectnum == 'J': # Arpeggio
        return (EFFECT_ARPEGGIO, effectparams)

    elif effectnum == 'S': # This effect is subdivided into many

        subeffectnum = (effectparams & 0xF0) >> 4
        subeffectparams = effectparams & 0x0F

        if subeffectnum == 0x8: # Pan position
            val = s3m_pan_to_gb(subeffectparams, channel)
            return (EFFECT_PAN, val)

        if subeffectnum == 0xC: # Notecut
            return (EFFECT_NOTE_CUT, subeffectparams)

    raise(f"Unsupported effect: {effectnum}{effectparams:02X}")

HAS_VOLUME      = 1 << 4
HAS_INSTRUMENT  = 1 << 5
HAS_EFFECT      = 1 << 6
HAS_NOTE        = 1 << 7
HAS_KIT         = 1 << 7

def convert_channel1(note_index, samplenum, volume, effectnum, effectparams):
    command = [ 0, 0, 0, 0 ] # NOP
    command_ptr = 1

    # Check if it's needed to add a note
    if note_index != -1:
        note_index = s3m_note_to_gb(note_index)
        command[0] |= HAS_NOTE
        command[command_ptr] = note_index
        command_ptr = command_ptr + 1

    # Check if there is a sample defined
    if samplenum > 0:
        instrument = samplenum & 3

        command[0] |= HAS_INSTRUMENT
        command[command_ptr] = (instrument << 4) & 0x30

    if effectnum is not None:
        [num, params] = effect_s3m_to_gb(1, effectnum, effectparams)

        if num is not None:
            command[0] |= HAS_EFFECT
            command[command_ptr] |= num & 0x0F
            command_ptr += 1
            command[command_ptr] = params & 0xFF

    # Check if it's needed to add a volume
    if volume > -1:
        command[0] |= HAS_VOLUME
        command[0] |= s3m_volume_to_gb(volume) & 0x0F

    # Note: The volume bit doesn't affect the final size.
    sizes = [ 1, 2, 3, 3, 2, 3, 4, 4 ]
    command_size = sizes[command[0] >> 5]

    return command[:command_size]

def convert_channel2(note_index, samplenum, volume, effectnum, effectparams):
    command = [ 0, 0, 0, 0 ] # NOP
    command_ptr = 1

    # Check if it's needed to add a note
    if note_index != -1:
        note_index = s3m_note_to_gb(note_index)
        command[0] |= HAS_NOTE
        command[command_ptr] = note_index
        command_ptr = command_ptr + 1

    # Check if there is a sample defined
    if samplenum > 0:
        instrument = samplenum & 3

        command[0] |= HAS_INSTRUMENT
        command[command_ptr] = (instrument << 4) & 0x30

    if effectnum is not None:
        [num, params] = effect_s3m_to_gb(2, effectnum, effectparams)

        if num is not None:
            command[0] |= HAS_EFFECT
            command[command_ptr] |= num & 0x0F
            command_ptr += 1
            command[command_ptr] = params & 0xFF

    # Check if it's needed to add a volume
    if volume > -1:
        command[0] |= HAS_VOLUME
        command[0] |= s3m_volume_to_gb(volume) & 0x0F

    # Note: The volume bit doesn't affect the final size.
    sizes = [ 1, 2, 3, 3, 2, 3, 4, 4 ]
    command_size = sizes[command[0] >> 5]

    return command[:command_size]

def convert_channel3(note_index, samplenum, volume, effectnum, effectparams):
    command = [ 0, 0, 0, 0 ] # NOP
    command_ptr = 1

    # Check if it's needed to add a note
    if note_index != -1:
        note_index = s3m_note_to_gb(note_index)
        command[0] |= HAS_NOTE
        command[command_ptr] = note_index
        command_ptr = command_ptr + 1

    # Check if there is a sample defined
    if samplenum > 0:
        instrument = samplenum & 7

        command[0] |= HAS_INSTRUMENT
        command[command_ptr] = (instrument << 4) & 0xF0

    if effectnum is not None:
        [num, params] = effect_s3m_to_gb(3, effectnum, effectparams)

        if num is not None:
            command[0] |= HAS_EFFECT
            command[command_ptr] |= num & 0x0F
            command_ptr += 1
            command[command_ptr] = params & 0xFF

    # Check if it's needed to add a volume
    if volume > -1:
        command[0] |= HAS_VOLUME
        command[0] |= s3m_volume_to_gb_ch3(volume) & 0x0F

    # Note: The volume bit doesn't affect the final size.
    sizes = [ 1, 2, 3, 3, 2, 3, 4, 4 ]
    command_size = sizes[command[0] >> 5]

    return command[:command_size]

def convert_channel4(note_index, samplenum, volume, effectnum, effectparams):
    command = [ 0, 0, 0, 0 ] # NOP
    command_ptr = 1

    # Note cut using ^^ as note
    if note_index == 0xFE:
        if samplenum > 0:
            # This limitation is only for channel 4. It should never happen in a
            # regular song.
            raise("Note cut + Sample in same step: Not supported in channel 4")
        samplenum = 0xFE

    # Check if there is a sample defined
    if samplenum > 0:
        if samplenum == 0xFE:
            kit = 0xFE;
        else:
            kit = samplenum & 0xF;
        command[0] |= HAS_KIT
        command[command_ptr] = kit
        command_ptr += 1

    if effectnum is not None:
        [num, params] = effect_s3m_to_gb(4, effectnum, effectparams)

        if num is not None:
            command[0] |= HAS_EFFECT
            command[command_ptr] |= num & 0x0F
            command_ptr += 1
            command[command_ptr] = params & 0xFF

    # Check if it's needed to add a volume
    if volume > -1:
        command[0] |= HAS_VOLUME
        command[0] |= s3m_volume_to_gb(volume) & 0x0F

    # Note: The volume bit doesn't affect the final size.
    sizes = [ 1, 2, 3, 3, 2, 3, 4, 4 ]
    command_size = sizes[command[0] >> 5]

    return command[:command_size]

SAMPLE_64_ENTRIES = 1 << 7

def initial_state_array(speed, panning_array, instruments):
    array = []

    # Initial speed
    # -------------

    array.extend([1, speed])

    # Initial panning
    # ---------------

    array.extend([2])
    array.extend(panning_array)

    # Channel 3 instruments
    # ---------------------

    if instruments is not None:
        print("Exporting instruments...")
        count = 0
        for i in instruments:
            # In the tracker, instruments start at index 1, but they start at
            # index 0 in the S3M file.
            count += 1

            # Only handle instruments assigned to channel 3
            if count < 8 or count > 15:
                continue

            try:
                body = i.body
                name = body.sample_name.decode("utf-8")

                if body.type != S3m.Instrument.InstTypes.sample:
                    raise S3MFormatError(f"Sample '{name}': Unsupported instrument type!")

                size = len(body.body.sample)
                if size != 32 and size != 64:
                    raise S3MFormatError(f"Sample '{name}': Invalid sample length: {size}")
                else:
                    flags = count - 8
                    if size == 64:
                        flags |= SAMPLE_64_ENTRIES

                    array.extend([3, flags])

                    # Convert from 8 bit to 4 bit
                    for i in range(0, size, 2):
                        sample_hi = body.body.sample[i + 0] >> 4
                        sample_lo = body.body.sample[i + 1] >> 4
                        value = (sample_hi << 4) | sample_lo
                        array.extend([value])

            except kaitaistruct.ValidationNotEqualError as e:
                if e.src_path == u"/types/instrument/seq/6":
                    # This may be caused by an empty instrument, don't crash!
                    pass
                else:
                    raise S3MFormatError("Error while decoding instruments")
            except Exception as e:
                raise e

    # End commands
    # ------------

    array.extend([0])

    return array

def convert_file(module_path, song_name, output_path, export_instruments):

    data = S3m.from_file(module_path)

    if output_path == None:
        output_path = song_name + ".c"

    fileout = open(output_path, "w")

    name = data.song_name.decode("utf-8")
    print(f"Song Name: '{name}'")
    print(f"Num. Orders: {data.num_orders}")
    print(f"Num. Patterns: {data.num_patterns}")

    fileout.write("// File created by s3m2gbt\n\n"
                  "#include <stddef.h>\n#include <stdint.h>\n\n")

    # Export patterns
    # ---------------

    print(f"Exporting patterns...")

    pattern = -1
    for p in data.patterns:
        pattern += 1

        # Check if pattern is actually used in the order list. If it isn't used,
        # don't export it.
        if pattern not in data.orders:
            print(f"Pattern {pattern} not exported: Not in the order list")
            continue

        fileout.write(f"static const uint8_t {song_name}_{pattern}[] = ")
        fileout.write("{\n")

        step = 0
        try:
            cmd1 = [0]
            cmd2 = [0]
            cmd3 = [0]
            cmd4 = [0]

            for c in p.body.body.cells:

                # If an end of row marker is reached, print the previous row.
                # Trust that the S3M file is generated in a valid way and it
                # doesn't have markers at weird positions, and that there is one
                # marker right at the end of each pattern.
                if c.channel_num == 0 and (not c.has_volume) and \
                    (not c.has_fx) and (not c.has_note_and_instrument):

                    # Write step
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

                    # Rows with note and instrument but no volume use the
                    # default volume of the sample.
                    if instrument > 0 and volume == -1:
                        this_instrument = data.instruments[instrument - 1].body
                        volume = this_instrument.body.default_volume

                effectnum = None
                effectparams = None
                if c.has_fx:
                    # Convert type to ASCII to match the documentation
                    effectnum = chr(c.fx_type + ord('A') - 1)
                    effectparams = c.fx_value

                channel = c.channel_num + 1

                try:
                    if channel == 1:
                        cmd1 = convert_channel1(note, instrument, volume,
                                                effectnum, effectparams)
                    elif channel == 2:
                        cmd2 = convert_channel2(note, instrument, volume,
                                                effectnum, effectparams)
                    elif channel == 3:
                        cmd3 = convert_channel3(note, instrument, volume,
                                                effectnum, effectparams)
                    elif channel == 4:
                        cmd4 = convert_channel4(note, instrument, volume,
                                                effectnum, effectparams)
                    else:
                        raise S3MFormatError(f"Too many channels: {channel}")
                except StepConversionError as e:
                    e.step = step
                    e.pattern = pattern
                    e.channel = channel
                    raise e

        except kaitaistruct.ValidationNotEqualError as e:
            info = str(vars(e))
            raise S3MFormatError(f"Unknown error: {info}")

        fileout.write("};\n")
        fileout.write("\n")

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

    instr = None
    if export_instruments:
        instr = data.instruments

    state_array = initial_state_array(data.initial_speed, gb_default_pan, instr)

    # Write rows of 8 bytes until the end of the array
    while True:
        left = len(state_array)

        write = []
        if left == 0:
            break
        elif left <= 8:
            write = state_array
            state_array = []
        else:
            write = state_array[0:8]
            state_array = state_array[8:]

        fileout.write("    ")
        for s in write:
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

if __name__ == "__main__":

    import argparse
    import sys

    print("s3m2gbt v4.2.0 (part of GBT Player)")
    print("Copyright (c) 2022 Antonio Niño Díaz <antonio_nd@outlook.com>")
    print("Copyright (c) 2015-2022 Kaitai Project")
    print("All rights reserved")
    print("")

    parser = argparse.ArgumentParser(description='Convert S3M files into GBT format.')
    parser.add_argument("--input", default=None, required=True,
                        help="input file")
    parser.add_argument("--name", default=None, required=True,
                        help="output song name")
    parser.add_argument("--output", default=None, required=False,
                        help="output file")
    parser.add_argument("--instruments", default=False, required=False,
                        action='store_true', help="export channel 3 instruments")

    args = parser.parse_args()

    try:
        convert_file(args.input, args.name, args.output, args.instruments)
    except StepConversionError as e:
        print("ERROR: " + str(e))
        sys.exit(1)
    except S3MFormatError as e:
        print("ERROR: Invalid S3M file: " + str(e))
        sys.exit(1)

    print("Done!")

    sys.exit(0)
