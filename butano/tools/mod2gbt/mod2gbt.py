#!/usr/bin/env python3

# mod2gbt v4.4.1 (Part of GBT Player)
#
# SPDX-License-Identifier: MIT
#
# Copyright (c) 2009-2022 Antonio Niño Díaz <antonio_nd@outlook.com>

class RowConversionError(Exception):
    def __init__(self, message, pattern = -1, row = -1, channel = -1):
        self.pattern = pattern
        self.row = row
        self.channel = channel + 1
        self.message = message

    def __str__(self):
        return f"Pattern {self.pattern} | Row {self.row} | Channel {self.channel} | {self.message}"

class MODFormatError(Exception):
    pass

class ModFile:

    def pattern_get_unpacked_step(self, pattern, row, channel):
        p = self.patterns[pattern]
        index = (row * 4 * 4) + (channel * 4)
        info = p[index:index + 4]

        sample_num    = (info[0] & 0xF0) | ((info[2] & 0xF0) >> 4)
        sample_period =  info[1]         | ((info[0] & 0x0F) << 8)
        effect_num    =  info[2] & 0x0F
        effect_param  =  info[3]

        note_index = mod_get_index_from_period(sample_period)
        return (sample_num, note_index, effect_num, effect_param)

    def __init__(self, data):
        self.name = data[0:20].decode("utf-8")
        print(f"Song name: '{self.name}'")

        # Skip name and sample data (31 samples)
        read_ptr = 20 + 30 * 31

        self.song_length = data[read_ptr]
        read_ptr += 1
        print(f"Song length: {self.song_length}")

        read_ptr += 1 # Skip unused byte

        self.pattern_table = data[read_ptr:read_ptr + 128]
        read_ptr += 128

        self.identifier = data[read_ptr:read_ptr + 4]
        if self.identifier != b'M.K.':
            raise MODFormatError("Only 4 channel mod files with 31 samples are supported")

        read_ptr += 4

        self.patterns = []
        for i in range(0, 64):
            self.patterns.append(data[read_ptr:read_ptr + 64 * 4 * 4])
            read_ptr += 64 * 4 * 4

# Channels 1, 2, 4
def mod_volume_to_gb(mod_vol):
    if mod_vol >= 64:
        return 15
    else:
        return mod_vol >> 2

# Channel 3
def mod_volume_to_gb_ch3(mod_vol):
    vol = mod_volume_to_gb(mod_vol)

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

def mod_pan_to_gb(pan, channel):
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

# SAMPLE PERIOD LUT - MOD values
#
#           C    C#   D    D#   E    F    F#   G    G#   A    A#   B
# Octave 0:1712,1616,1525,1440,1357,1281,1209,1141,1077,1017, 961, 907 | C3 to B3
# Octave 1: 856, 808, 762, 720, 678, 640, 604, 570, 538, 508, 480, 453 | C4 to B4
# Octave 2: 428, 404, 381, 360, 339, 320, 302, 285, 269, 254, 240, 226 | C5 to B5
# Octave 3: 214, 202, 190, 180, 170, 160, 151, 143, 135, 127, 120, 113 | C6 to B6
# Octave 4: 107, 101,  95,  90,  85,  80,  76,  71,  67,  64,  60,  57 | C7 to B7
# Octave 5:  53,  50,  47,  45,  42,  40,  37,  35,  33,  31,  30,  28 | C8 to B8
#
# From C3 to B8  |  A5 = 1750 = 440.00Hz  |  C5 = 1546
#
# const UWORD GB_frequencies[] = {
#       44, 156, 262, 363, 457, 547, 631, 710, 786, 854, 923, 986, | C3 to B3
#     1046,1102,1155,1205,1253,1297,1339,1379,1417,1452,1486,1517, | C4 to B4
#     1546,1575,1602,1627,1650,1673,1694,1714,1732,1750,1767,1783, | C5 to B5
#     1798,1812,1825,1837,1849,1860,1871,1881,1890,1899,1907,1915, | C6 to B6
#     1923,1930,1936,1943,1949,1954,1959,1964,1969,1974,1978,1982, | C7 to B7
#     1985,1988,1992,1995,1998,2001,2004,2006,2009,2011,2013,2015  | C8 to B8
# };
#
# That means... MOD C0 (period 1712) = GB C3 (freq 44, index 0)
# Anyway, they don't sound the same...

def mod_get_index_from_period(period):
    if period <= 0:
        return -1

    mod_period = [
        1712,1616,1524,1440,1356,1280,1208,1140,1076,1016, 960, 907,
        856, 808, 762, 720, 678, 640, 604, 570, 538, 508, 480, 453,
        428, 404, 381, 360, 339, 320, 302, 285, 269, 254, 240, 226,
        214, 202, 190, 180, 170, 160, 151, 143, 135, 127, 120, 113,
        107, 101,  95,  90,  85,  80,  75,  71,  67,  63,  60,  56,
        53,  50,  47,  45,  42,  40,  37,  35,  33,  31,  30,  28
    ]

    if period < mod_period[(6 * 12) - 1]:
        raise RowConversionError("Note too high")
    elif period > mod_period[0]:
        raise RowConversionError("Note too low")

    for i in range(0, 6 * 12):
        if period == mod_period[i]:
            return i

    # Couldn't find exact match... get nearest value

    nearest_value = 0xFFFF
    nearest_index = 0
    for i in range(0, 6 * 12):
        test_distance = abs(period - mod_period[i])
        nearest_distance = abs(period - nearest_value)

        if test_distance < nearest_distance:
            nearest_value = mod_period[i]
            nearest_index = i

    return nearest_index

def speed_mod_to_gb(speed):
    if do_speed_conversion: # Amiga's 50 Hz to GB's 60 Hz
        return (speed * 60) // 50
    else:
        return speed

EFFECT_PAN              = 0
EFFECT_ARPEGGIO         = 1
EFFECT_NOTE_CUT         = 2
EFFECT_VIBRATO          = 3
EFFECT_VOLUME_SLIDE     = 4
EFFECT_PATTERN_JUMP     = 8
EFFECT_BREAK_SET_STEP   = 9
EFFECT_SPEED            = 10
EFFECT_EVENT            = 15

def effect_mod_to_gb(channel, effectnum, effectparams):

    if effectnum == 0x0: # Arpeggio
        return (EFFECT_ARPEGGIO, effectparams)

    elif effectnum == 0xB: # Jump to pattern
        # TODO: Fail if this jumps out of bounds
        return (EFFECT_PATTERN_JUMP, effectparams)

    elif effectnum == 0xC: # Volume -> Not handled here
        raise RowConversionError("Unexpected error")

    elif effectnum == 0xD: # Break + Set step
        # Effect value is BCD, convert to integer
        val = (((effectparams & 0xF0) >> 4) * 10) + (effectparams & 0x0F)
        return (EFFECT_BREAK_SET_STEP, val)

    elif effectnum == 0xE: # This effect is subdivided into many

        subeffectnum = (effectparams & 0xF0) >> 4
        subeffectparams = effectparams & 0x0F

        if subeffectnum == 0x8: # Pan position
            val = mod_pan_to_gb(subeffectparams, channel)
            return (EFFECT_PAN, val)

        elif subeffectnum == 0xC: # Notecut
            return (EFFECT_NOTE_CUT, subeffectparams)

    elif effectnum == 0xF: # Speed
        if effectparams > 0x1F: # BPM effect - Not supported
            print("Unsupported BPM speed effect")
            return (None, None)
        else: # Speed
            if effectparams == 0:
                raise RowConversionError("Speed must not be zero")

            return (EFFECT_SPEED, speed_mod_to_gb(effectparams))

    print(f"Unsupported effect: {effectnum:01X}{effectparams:02X}")
    return (None, None)

HAS_VOLUME      = 1 << 4
HAS_INSTRUMENT  = 1 << 5
HAS_EFFECT      = 1 << 6
HAS_NOTE        = 1 << 7
HAS_KIT         = 1 << 7

def convert_channel1(note_index, samplenum, effectnum, effectparams):
    command = [0, 0, 0, 0]
    command_ptr = 1

    volume = -1

    # If the effect is "Set Volume", handle it before and clear it so that it
    # isn't handled later.
    if effectnum == 0xC:
        volume = effectparams
        effectnum = 0
        effectparams = 0

    # Check if it's needed to add a note
    if note_index >= 0 and note_index <= (6 * 12 - 1):
        command[0] |= HAS_NOTE
        command[command_ptr] = note_index & 0x7F
        command_ptr += 1

    # Check if there is a sample defined
    if samplenum > 0:
        instrument = samplenum & 3
        command[0] |= HAS_INSTRUMENT
        command[command_ptr] = (instrument << 4) & 0x30

    if (effectnum != 0) or (effectparams != 0):
        converted_num, converted_params = effect_mod_to_gb(1, effectnum, effectparams)
        if converted_num != None:
            command[0] |= HAS_EFFECT
            command[command_ptr] |= converted_num & 0x0F
            command_ptr += 1
            command[command_ptr] = converted_params & 0xFF

    # Check if it's needed to add a volume
    if volume > -1:
        command[0] |= HAS_VOLUME
        command[0] |= mod_volume_to_gb(volume) & 0x0F

    # Note: The volume bit doesn't affect the final size.
    sizes = [ 1, 2, 3, 3, 2, 3, 4, 4 ]
    bits = command[0] >> 5

    return command[:sizes[bits]]

def convert_channel2(note_index, samplenum, effectnum, effectparams):
    command = [0, 0, 0, 0]
    command_ptr = 1

    volume = -1

    # If the effect is "Set Volume", handle it before and clear it so that it
    # isn't handled later.
    if effectnum == 0xC:
        volume = effectparams
        effectnum = 0
        effectparams = 0

    # Check if it's needed to add a note
    if note_index >= 0 and note_index <= (6 * 12 - 1):
        command[0] |= HAS_NOTE
        command[command_ptr] = note_index & 0x7F
        command_ptr += 1

    # Check if there is a sample defined
    if samplenum > 0:
        instrument = samplenum & 3
        command[0] |= HAS_INSTRUMENT
        command[command_ptr] = (instrument << 4) & 0x30

    if (effectnum != 0) or (effectparams != 0):
        converted_num, converted_params = effect_mod_to_gb(2, effectnum, effectparams)
        if converted_num != None:
            command[0] |= HAS_EFFECT
            command[command_ptr] |= converted_num & 0x0F
            command_ptr += 1
            command[command_ptr] = converted_params & 0xFF

    # Check if it's needed to add a volume
    if volume > -1:
        command[0] |= HAS_VOLUME
        command[0] |= mod_volume_to_gb(volume) & 0x0F

    # Note: The volume bit doesn't affect the final size.
    sizes = [ 1, 2, 3, 3, 2, 3, 4, 4 ]
    bits = command[0] >> 5
    return command[:sizes[bits]]

def convert_channel3(note_index, samplenum, effectnum, effectparams):
    command = [0, 0, 0, 0]
    command_ptr = 1

    volume = -1

    # If the effect is "Set Volume", handle it before and clear it so that it
    # isn't handled later.
    if effectnum == 0xC:
        volume = effectparams
        effectnum = 0
        effectparams = 0

    # Check if it's needed to add a note
    if note_index >= 0 and note_index <= (6 * 12 - 1):
        command[0] |= HAS_NOTE
        command[command_ptr] = note_index & 0x7F
        command_ptr += 1

    # Check if there is a sample defined
    if samplenum > 0:
        instrument = samplenum & 7
        command[0] |= HAS_INSTRUMENT
        command[command_ptr] = (instrument << 4) & 0xF0

    if (effectnum != 0) or (effectparams != 0):
        converted_num, converted_params = effect_mod_to_gb(3, effectnum, effectparams)
        if converted_num != None:
            command[0] |= HAS_EFFECT
            command[command_ptr] |= converted_num & 0x0F
            command_ptr += 1
            command[command_ptr] = converted_params & 0xFF

    # Check if it's needed to add a volume
    if volume > -1:
        command[0] |= HAS_VOLUME
        command[0] |= mod_volume_to_gb_ch3(volume) & 0x07

    # Note: The volume bit doesn't affect the final size.
    sizes = [ 1, 2, 3, 3, 2, 3, 4, 4 ]
    bits = command[0] >> 5
    return command[:sizes[bits]]

def convert_channel4(samplenum, effectnum, effectparams):
    command = [0, 0, 0, 0]
    command_ptr = 1

    volume = -1

    # If the effect is "Set Volume", handle it before and clear it so that it
    # isn't handled later.
    if effectnum == 0xC:
        volume = effectparams
        effectnum = 0
        effectparams = 0

    # Check if there is a sample defined
    if samplenum > 0:
        kit = samplenum & 0xF
        command[0] |= HAS_KIT
        command[command_ptr] = kit & 0x0F
        command_ptr += 1

    if (effectnum != 0) or (effectparams != 0):
        converted_num, converted_params = effect_mod_to_gb(4, effectnum, effectparams)
        if converted_num != None:
            command[0] |= HAS_EFFECT
            command[command_ptr] |= converted_num & 0x0F
            command_ptr += 1
            command[command_ptr] = converted_params & 0xFF

    # Check if it's needed to add a volume
    if volume > -1:
        command[0] |= HAS_VOLUME
        command[0] |= mod_volume_to_gb(volume) & 0x0F

    # Note: The volume bit doesn't affect the final size.
    sizes = [ 1, 3, 2, 4 ]
    bits = command[0] >> 6
    return command[:sizes[bits]]

def export_pattern(mod, fileout, label_name, index):

    fileout.write(f"static const uint8_t {label_name}_{index}[] = {{\n")

    for i in range(0, 64):
        try:
            channel = 1
            sample, note, effect, effectarg = mod.pattern_get_unpacked_step(index, i, 0)
            step1 = convert_channel1(note, sample, effect, effectarg)
            channel = 2
            sample, note, effect, effectarg = mod.pattern_get_unpacked_step(index, i, 1)
            step2 = convert_channel2(note, sample, effect, effectarg)
            channel = 3
            sample, note, effect, effectarg = mod.pattern_get_unpacked_step(index, i, 2)
            step3 = convert_channel3(note, sample, effect, effectarg)
            channel = 4
            sample, note, effect, effectarg = mod.pattern_get_unpacked_step(index, i, 3)
            step4 = convert_channel4(sample, effect, effectarg)
        except RowConversionError as e:
            e.row = i
            e.pattern = index
            e.channel = channel
            raise e

        fileout.write("    ")
        row = step1 + step2 + step3 + step4
        for s in row:
            fileout.write(f"0x{s:02X},")
        fileout.write("\n")

    fileout.write("};\n")
    fileout.write("\n")

def convert_file(mod_in_path, label_name, speed_conversion):

    # TODO: Clean this
    global do_speed_conversion
    do_speed_conversion = speed_conversion

    with open(mod_in_path, "rb") as file:
        file_byte_array = bytearray(file.read())

    mod = ModFile(file_byte_array)

    output_path = label_name + ".c"

    with open(output_path, "w") as fileout:

        fileout.write("// File created by mod2gbt\n\n"
                    "#include <stddef.h>\n#include <stdint.h>\n\n")

        print("Exporting patterns...")

        # There are always 64 patterns in the MOD file. Check the pattern table
        # to determine the patterns that are actually used.
        for i in range(0, 64):
            if i in mod.pattern_table:
                export_pattern(mod, fileout, label_name, i)

        print("Exporting pattern order...")

        fileout.write(f"const uint8_t *{label_name}[] = {{\n")
        fileout.write("    NULL,\n") # MOD files have no initial state

        for i in range(0, mod.song_length):
            num = str(mod.pattern_table[i])
            fileout.write(f"    {label_name}_{num},\n")

        fileout.write("    NULL\n")
        fileout.write("};")

if __name__ == "__main__":

    import argparse
    import sys

    print("mod2gbt v4.4.1 (part of GBT Player)")
    print("Copyright (c) 2009-2022 Antonio Niño Díaz <antonio_nd@outlook.com>")
    print("All rights reserved")
    print("")
    print("GBT Player has deprecated the usage of MOD files. This converter is")
    print("only provided for backwards compatibility.")
    print("")

    parser = argparse.ArgumentParser(
                        description='Splits a GBT Player S3M file into two.')
    parser.add_argument("modfile", default=None,
                        help="path to the input MOD file")
    parser.add_argument("song_name", default=None,
                        help="output song name")
    parser.add_argument("-speed", default=True, required=False,
                        action='store_false',
                        help="don't convert speed from 50 Hz to 60 Hz")

    args = parser.parse_args()

    try:
        convert_file(args.modfile, args.song_name, args.speed)
    except RowConversionError as e:
        print("ERROR: " + str(e))
        sys.exit(1)
    except MODFormatError as e:
        print("ERROR: Invalid MOD file: " + str(e))
        sys.exit(1)

    print("Done!")

    sys.exit(0)
