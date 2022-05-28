#!/usr/bin/env python3

# s3m2gbt v4.4.1 (Part of GBT Player)
#
# SPDX-License-Identifier: MIT
#
# Copyright (c) 2022 Antonio Niño Díaz <antonio_nd@outlook.com>

class RowConversionError(Exception):
    def __init__(self, message, pattern = -1, row = -1, channel = -1):
        self.pattern = pattern
        self.row = row
        self.channel = channel + 1
        self.message = message

    def __str__(self):
        return f"Pattern {self.pattern} | Row {self.row} | Channel {self.channel} | {self.message}"

class S3MFormatError(Exception):
    pass

class S3MFormatReader:

    def read_u8(self):
        offset = self.read_ptr
        self.read_ptr += 1
        return int(self.data[offset])

    def read_u16(self):
        offset = self.read_ptr
        self.read_ptr += 2
        return int((self.data[offset + 1] << 8) | self.data[offset])

    def read_memseg(self):
        offset = self.read_ptr
        self.read_ptr += 3
        part1 = self.data[offset + 0]
        part2 = self.data[offset + 1]
        part3 = self.data[offset + 2]
        return int((part1 << 16) | (part3 << 8) | part2)

    def read_string(self, size):
        offset = self.read_ptr
        self.read_ptr += size
        return self.data[offset:offset+size]

class S3MFileInstrument(S3MFormatReader):

    def __init__(self, data, offset):
        self.data = data
        self.read_ptr = offset

        instrument_type = self.read_u8()
        if instrument_type != 1:
            self.exists = False
            return
        self.exists = True

        self.dos_filename = self.read_string(12).decode("utf-8")

        self.sample_data_offset = self.read_memseg() * 16

        self.length = self.read_u16()
        self.length |= self.read_u16() << 16

        self.read_ptr += 4 + 4 # Skip loop begin and loop end

        self.default_volume = self.read_u8()

        self.read_ptr = offset + 0x30

        self.sample_name = self.read_string(28).decode("utf-8")

        if self.read_string(4) != b'SCRS':
            raise S3MFormatError("Invalid magic string in instrument")

        start = self.sample_data_offset
        end = start + self.length
        self.sample_data = self.data[start:end]

class S3MFilePatternCell():

    def __init__(self, header, channel, note, instrument, volume,
                 effect, effect_args):

        if header == 0:
            self.empty = True
            return

        self.empty = False

        self.channel = channel

        if (note != None) or (instrument != None):
            self.has_note_and_instrument = True
            self.note = note
            self.instrument = instrument
        else:
            self.has_note_and_instrument = False

        if volume != None:
            self.has_volume = True
            self.volume = volume
        else:
            self.has_volume = False

        if (effect != None) or (effect_args != None):
            self.has_effect = True
            self.effect = effect
            self.effect_args = effect_args
        else:
            self.has_effect = False

class S3MFilePattern(S3MFormatReader):

    def __init__(self, data, offset):

        # Check if we have asked to generate an empty pattern
        if data == None:
            cell = S3MFilePatternCell(0, 0, 0, 0, 0, 0, 0)
            self.cells = []
            for i in range(0, 64):
                self.cells.append(cell)
            return

        self.data = data
        self.read_ptr = offset

        length = self.read_u16() - 2

        self.cells = []

        while length > 0:
            header = self.read_u8()
            length -= 1

            channel = header & 31

            note = None
            instrument = None
            volume = None
            effect = None
            effect_args = None

            if (header & (1 << 5)) != 0: # Has note and instrument
                note = self.read_u8()
                instrument = self.read_u8()
                length -= 2

            if (header & (1 << 6)) != 0: # Has volume
                volume = self.read_u8()
                length -= 1

            if (header & (1 << 7)) != 0: # Has effect
                effect = self.read_u8()
                effect_args = self.read_u8()
                length -= 2

            cell = S3MFilePatternCell(header, channel, note, instrument, volume,
                                      effect, effect_args)
            self.cells.append(cell)

class S3MFile(S3MFormatReader):

    def __init__(self, data):

        # Save data for now

        self.data = data
        self.read_ptr = 0

        self.name = self.read_string(28).decode("utf-8")
        print(f"Song Name: '{self.name}'")

        self.read_ptr += 1 + 1 + 2 # Ignore fields

        self.song_length = self.read_u16()
        print(f"Song Length: {self.song_length}")

        self.num_instruments = self.read_u16()
        self.num_patterns = self.read_u16()

        self.read_ptr += 6 # Ignore fields

        if self.read_string(4) != b'SCRM':
            raise S3MFormatError("Invalid magic string in file")

        self.read_ptr += 1 # Ignore global volume

        self.initial_speed = self.read_u8()

        if self.read_u8() != 150:
            raise S3MFormatError("Invalid tempo: It must be 150")

        self.read_ptr += 2 # Ignore master volume and ultraclick removal

        # Save this for later
        has_custom_pan = False
        if self.read_u8() == 252:
            has_custom_pan = True

        self.read_ptr = 0x40
        channel_settings = self.read_string(4)
        if channel_settings[0] >= 16 or channel_settings[1] >= 16 or \
           channel_settings[2] >= 16 or channel_settings[3] >= 16:
            raise S3MFormatError("Invalid channel settings: Channels 0-3 must be enabled")

        # Read orders

        self.read_ptr = 0x60

        self.song_orders = self.read_string(self.song_length)
        if self.song_length % 2 == 1:
            self.read_ptr += 1 # Align to 2

        # Read instrument parapointers

        self.instrument_offsets = [None] * self.num_instruments
        for i in range(0, self.num_instruments):
            self.instrument_offsets[i] = self.read_u16() * 16

        # Read pattern parapointers

        self.pattern_offsets = [None] * self.num_patterns
        for i in range(0, self.num_patterns):
            self.pattern_offsets[i] = self.read_u16() * 16

        # Read default panning

        if has_custom_pan:
            self.channel_pan = [b & 0xF for b in self.read_string(4)]
        else:
            self.channel_pan = [8, 8, 8, 8]

        # Load instruments

        self.instruments = [None] * self.num_instruments
        for i in range(0, len(self.instrument_offsets)):
            offset = self.instrument_offsets[i]
            if offset != 0:
                instr = S3MFileInstrument(self.data, offset)
                if instr.exists:
                    self.instruments[i] = instr

        # Load patterns

        self.patterns = [None] * self.num_patterns
        for i in range(0, len(self.pattern_offsets)):
            offset = self.pattern_offsets[i]
            if offset != 0:
                self.patterns[i] = S3MFilePattern(self.data, offset)
            else:
                # A NULL pointer means that the pattern is empty
                self.patterns[i] = S3MFilePattern(None, 0)

        # The file data is no longer needed

        self.data = []

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
        raise RowConversionError("Note too low")
    elif note > 32 + 16 * 6:
        raise RowConversionError("Note too high")

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
EFFECT_EVENT            = 15

# Returns (converted_num, converted_params) if there was a valid effect. If
# there is none, it returns (None, None). Note that it is needed to pass the
# channel to this function because some effects behave differently depending on
# the channel (like panning).
def effect_s3m_to_gb(channel, effectnum, effectparams):

    if effectnum == 'A': # Set Speed
        if effectparams == 0:
            raise RowConversionError("Speed must not be zero")

        return (EFFECT_SPEED, effectparams)

    if effectnum == 'B': # Pattern jump
        # TODO: Fail if this jumps out of bounds
        return (EFFECT_PATTERN_JUMP, effectparams)

    elif effectnum == 'C': # Break + Set row
        # Effect value is BCD, convert to integer
        val = (((effectparams & 0xF0) >> 4) * 10) + (effectparams & 0x0F)
        return (EFFECT_BREAK_SET_STEP, val)

    elif effectnum == 'D': # Volume Slide
        if channel == 3:
            raise RowConversionError("Volume slide not supported in channel 3")

        if effectparams == 0:
            # Ignore volume slide commands that just continue the effect,
            # they are only needed for the S3M player.
            return (None, None)

        upper = (effectparams >> 4) & 0xF
        lower = effectparams & 0xF

        if upper == 0xF or lower == 0xF:
            raise RowConversionError("Fine volume slide not supported")

        elif lower == 0: # Volume goes up
            params = 1 << 3 # Increase
            delay = 7 - upper + 1
            if delay <= 0:
                raise RowConversionError("Volume slide too steep")
            params |= delay
            return (EFFECT_VOLUME_SLIDE, params)
        elif upper == 0: # Volume goes down
            params = 0 << 3 # Decrease
            delay = 7 - lower + 1
            if delay <= 0:
                raise RowConversionError("Volume slide too steep")
            params = delay
            return (EFFECT_VOLUME_SLIDE, params)
        else:
            raise RowConversionError("Invalid volume slide arguments")

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

        elif subeffectnum == 0xC: # Notecut
            return (EFFECT_NOTE_CUT, subeffectparams)

        elif subeffectnum == 0xF: # Funkrepeat? Set active macro?
            # This effect is either unused, or it's the "set active macro"
            # command, which doesn't have any effect if you don't use the macro
            # afterwards. It can safely be overloaded for event callbacks.
            return (EFFECT_EVENT, subeffectparams)

    raise RowConversionError(f"Unsupported effect: {effectnum}{effectparams:02X}")

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
            raise("Note cut + Sample in same row: Not supported in channel 4")
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

STARTUP_CMD_DONE                = 0
STARTUP_CMD_SPEED               = 1
STARTUP_CMD_PANING              = 2
STARTUP_CMD_CHANNEL3_INSTRUMENT = 3

SAMPLE_64_ENTRIES = 1 << 7

def initial_state_array(speed, panning_array, instruments):
    array = []

    # Initial speed
    # -------------

    array.extend([STARTUP_CMD_SPEED, speed])

    # Initial panning
    # ---------------

    array.extend([STARTUP_CMD_PANING])
    array.extend(panning_array)

    # Channel 3 instruments
    # ---------------------

    if instruments is not None:
        print("Exporting instruments...")
        count = 0
        for inst in instruments:
            # In the tracker, instruments start at index 1, but they start at
            # index 0 in the S3M file.
            count += 1

            # Only handle instruments assigned to channel 3
            if count < 8 or count > 15:
                continue

            name = inst.sample_name

            size = inst.length
            if size != 32 and size != 64:
                raise S3MFormatError(f"Sample '{name}': Invalid sample length: {size}")
            else:
                flags = count - 8 # The low bits are the instrument index
                if size == 64:
                    flags |= SAMPLE_64_ENTRIES

                array.extend([STARTUP_CMD_CHANNEL3_INSTRUMENT, flags])

                # Convert from 8 bit to 4 bit
                for i in range(0, size, 2):
                    sample_hi = inst.sample_data[i + 0] >> 4
                    sample_lo = inst.sample_data[i + 1] >> 4
                    value = (sample_hi << 4) | sample_lo
                    array.extend([value])

    # End commands
    # ------------

    array.extend([STARTUP_CMD_DONE])

    return array

def convert_file(module_path, song_name, output_path, export_instruments):

    with open(module_path, "rb") as file:
        file_byte_array = bytearray(file.read())

    s3m = S3MFile(file_byte_array)

    if output_path == None:
        output_path = song_name + ".c"

    with open(output_path, "w") as fileout:

        fileout.write("// File created by s3m2gbt\n\n"
                      "#include <stddef.h>\n#include <stdint.h>\n\n")

        # Export patterns
        # ---------------

        print(f"Exporting patterns...")

        pattern = -1
        for p in s3m.patterns:
            pattern += 1

            # Check if pattern is actually used in the order list. If it isn't
            # used, don't export it.
            if pattern not in s3m.song_orders:
                print(f"Pattern {pattern} not exported: Not in the order list")
                continue

            fileout.write(f"static const uint8_t {song_name}_{pattern}[] = {{\n")

            row = 0

            cmd1 = [0]
            cmd2 = [0]
            cmd3 = [0]
            cmd4 = [0]

            for c in p.cells:

                # If an end of row marker is reached, print the previous row.
                # Trust that the S3M file is generated in a valid way and it
                # doesn't have markers at weird positions, and that there is one
                # marker right at the end of each pattern.
                if c.empty:

                    # Write row
                    fileout.write("    ")

                    cmd = cmd1 + cmd2 + cmd3 + cmd4
                    for b in cmd:
                        fileout.write(f"0x{b:02X},")

                    fileout.write("\n")

                    row = row + 1

                    # Clear commands
                    cmd1 = [0]
                    cmd2 = [0]
                    cmd3 = [0]
                    cmd4 = [0]

                    # Next iteration
                    continue

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
                        this_instr = s3m.instruments[instrument - 1]
                        volume = this_instr.default_volume

                effectnum = None
                effectparams = None
                if c.has_effect:
                    # Convert type to ASCII to match the documentation
                    effectnum = chr(c.effect + ord('A') - 1)
                    effectparams = c.effect_args

                channel = c.channel + 1

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
                except RowConversionError as e:
                    e.row = row
                    e.pattern = pattern
                    e.channel = channel
                    raise e

            fileout.write("};\n")
            fileout.write("\n")

        # Export initial state
        # --------------------

        print(f"Exporting initial state...")

        fileout.write(f"const uint8_t {song_name}_init_state[] = {{\n")

        default_pan = [8, 8, 8, 8]
        for i in range(0, 4):
            default_pan[i] = s3m.channel_pan[i]

        gb_default_pan = [
            s3m_pan_to_gb(default_pan[0], 1),
            s3m_pan_to_gb(default_pan[1], 2),
            s3m_pan_to_gb(default_pan[2], 3),
            s3m_pan_to_gb(default_pan[3], 4)
        ]

        instr = None
        if export_instruments:
            instr = s3m.instruments

        state_array = initial_state_array(s3m.initial_speed, gb_default_pan, instr)

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

        fileout.write(f"const uint8_t *{song_name}[] = {{\n")

        fileout.write(f"    {song_name}_init_state,")
        fileout.write("\n")

        for o in s3m.song_orders:
            pattern = int(o)
            if pattern >= s3m.num_patterns:
                # TODO: Warn if the pattern goes over the limit?
                continue
            fileout.write(f"    {song_name}_{pattern},")
            fileout.write("\n")

        fileout.write("    NULL\n")
        fileout.write("};\n")

if __name__ == "__main__":

    import argparse
    import sys

    print("s3m2gbt v4.4.1 (part of GBT Player)")
    print("Copyright (c) 2022 Antonio Niño Díaz <antonio_nd@outlook.com>")
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
    except RowConversionError as e:
        print("ERROR: " + str(e))
        sys.exit(1)
    except S3MFormatError as e:
        print("ERROR: Invalid S3M file: " + str(e))
        sys.exit(1)

    print("Done!")

    sys.exit(0)
