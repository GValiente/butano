#!/usr/bin/env python3

# vgm2gba
# SPDX-License-Identifier: CC0-1.0
#
# Original C code  by akkera102
# Port to Python   by copyrat90


class VgmFormatError(Exception):
    pass


class VgmFile:
    def __init__(self, data):
        self.data = data

        # validate header
        if data[0:3] != b"Vgm":
            raise VgmFormatError("vgm ident")
        if data[8:10] != b"\x61\x01" or data[0x10:0x12] != b"\x00\x00":
            raise VgmFormatError("not version 1.61")
        if data[0x80:0x84] != b"\x00\x00\x40\x00":
            raise VgmFormatError("not use Game Boy")

        # convert reg
        p = 0xC0
        while data[p] != 0x66:  # end of mark
            # ignore 0x00
            if data[p] == 0x00:
                p += 1
                continue

            # wait: 0x61 nn nn
            if data[p] == 0x61:
                p += 3
                continue
            elif 0x62 <= data[p] <= 0x63:
                p += 1
                continue

            # write reg: 0xb3 aa dd
            if data[p] == 0xB3:
                p += 1
                tmp = data[p] + 0x10
                if tmp == 0x10:  # NR 10
                    adr = 0x60
                elif tmp == 0x11:  # NR 11
                    adr = 0x62
                elif tmp == 0x12:  # NR 12
                    adr = 0x63
                elif tmp == 0x13:  # NR 13
                    adr = 0x64
                elif tmp == 0x14:  # NR 14
                    adr = 0x65
                elif tmp == 0x16:  # NR 21
                    adr = 0x68
                elif tmp == 0x17:  # NR 22
                    adr = 0x69
                elif tmp == 0x18:  # NR 23
                    adr = 0x6C
                elif tmp == 0x19:  # NR 24
                    adr = 0x6D
                elif tmp == 0x1A:  # NR 30
                    adr = 0x70
                elif tmp == 0x1B:  # NR 31
                    adr = 0x72
                elif tmp == 0x1C:  # NR 32
                    adr = 0x73
                elif tmp == 0x1D:  # NR 33
                    adr = 0x74
                elif tmp == 0x1E:  # NR 34
                    adr = 0x75
                elif tmp == 0x20:  # NR 41
                    adr = 0x78
                elif tmp == 0x21:  # NR 42
                    adr = 0x79
                elif tmp == 0x22:  # NR 43
                    adr = 0x7C
                elif tmp == 0x23:  # NR 44
                    adr = 0x7D
                elif tmp == 0x24:  # NR 50
                    adr = 0x80
                elif tmp == 0x25:  # NR 51
                    adr = 0x81
                elif tmp == 0x26:  # NR 52
                    adr = 0x84
                # RAM
                elif 0x30 <= tmp <= 0x3F:
                    adr = 0x90 + tmp - 0x30
                else:
                    raise VgmFormatError(f"offset 0x{p:02x} = 0x{data[p]:02x}")

                dat = data[p + 1]

                if adr == 0x70:  # NR 30
                    dat = dat & 0x80
                if adr == 0x73:  # NR 32
                    dat = dat & 0x60
                if adr == 0x80 and dat & 0x08 != 0:  # NR 50
                    print("Warning: no use GBA bit. NR 50(FF24) Right Flag.")
                if adr == 0x80 and dat & 0x80 != 0:  # NR 50
                    print("Warning: no use GBA bit. NR 50(FF24) Left Flag.")

                data[p] = adr
                p += 1
                data[p] = dat
                p += 1
                continue

            raise VgmFormatError(f"Commands. offset 0x{p:02x} = 0x{data[p]:02x}")

    def write_binary(self, output_path):
        with open(output_path, "wb") as file:
            file.write(self.__get_converted_data())

    def write_c_array(self, output_path, c_array_identifier):
        converted = self.__get_converted_data()

        with open(output_path, "w") as file:
            file.write(f"const unsigned char {c_array_identifier}[] = {{")
            for i, num in enumerate(converted):
                if i % 16 == 0:
                    file.write("\n")
                file.write(f"0x{num:02x},")
            file.write("};\n")

            file.write(
                f"const unsigned int {c_array_identifier}_len = {len(converted)};\n"
            )

    def __get_converted_data(self):
        data = self.data

        converted = bytearray()

        is_loop = False
        loop_bin = 0
        loop_vgm = (
            data[0x1C] | (data[0x1D] << 8) | (data[0x1E] << 16) | (data[0x1F] << 24)
        )
        loop_vgm += 0x1C

        print(f"VgmLoopOffset: 0x{loop_vgm:02x}")

        p = 0xC0
        fputc_cnt = 0

        while data[p] != 0x66:  # end of mark
            # check loop offset
            if p == loop_vgm:
                print(f"BinLoopOffset: 0x{fputc_cnt:02x}")

                loop_bin = fputc_cnt
                is_loop = True

            # ignore 0x00
            if data[p] == 0x00:
                p += 1
                continue

            # wait: 0x61 nn nn
            if 0x61 <= data[p] <= 0x63:
                # GBA side use vblank
                converted += b"\x61"
                fputc_cnt += 1
                # ignore param
                if data[p] == 0x61:
                    p += 3
                else:
                    p += 1
                continue

            # write reg: 0xb3 aa dd
            if data[p] == 0xB3:
                d2 = data[p + 1]

                converted += data[p : p + 3]
                p += 3
                fputc_cnt += 3

                # GBA patch

                # wave adr?
                if 0x90 <= d2 <= 0x9F:
                    # add REG_SOUND3CNT_L = 0x40;
                    converted += b"\xb3\x70\x40"
                    fputc_cnt += 3

                continue

            raise VgmFormatError(f"Conv-commands. offset 0x{p:02x} = 0x{data[p]:02x}")

        if not is_loop:
            print("Warning: loop offset not found")

        # write end of mark
        converted += data[p].to_bytes(1, byteorder="little")
        p += 1

        # write loop offset
        converted += loop_bin.to_bytes(4, byteorder="little")

        # zero padding
        pad = len(converted) & 0xF
        if pad != 0:
            converted += b"\x00" * (0x10 - pad)

        return converted


def convert_file_binary(vgm_path, output_path):
    with open(vgm_path, "rb") as file:
        file_byte_array = bytearray(file.read())

    vgm = VgmFile(file_byte_array)
    vgm.write_binary(output_path)


def convert_file_c_array(vgm_path, output_path, c_array_identifier=None):
    if not c_array_identifier:
        import os

        output_filename = os.path.splitext(os.path.split(output_path)[-1])[0]
        c_array_identifier = output_filename

    with open(vgm_path, "rb") as file:
        file_byte_array = bytearray(file.read())

    vgm = VgmFile(file_byte_array)
    vgm.write_c_array(output_path, c_array_identifier)


if __name__ == "__main__":
    import argparse
    import sys

    print("vgm2gba - convert hUGETracker VGM")
    print("This program is distributed under the CC0-1.0")
    print()
    print("Original C code  by akkera102")
    print("Port to Python   by copyrat90")
    print()

    parser = argparse.ArgumentParser(
        description="convert hUGETracker VGM files into VGM Player binary format."
    )
    parser.add_argument(
        "--input", default=None, required=True, help="input hUGETracker VGM file path"
    )
    parser.add_argument(
        "--output", default=None, required=False, help="output file path"
    )
    parser.add_argument(
        "--c-array",
        nargs="?",
        default=None,
        const=True,
        required=False,
        help="output as C array, instead of binary. Optionally, you can pass a desired identifier for the array.",
    )

    args = parser.parse_args()

    if not args.output:
        import os

        # Use `input_filename.bin` as output file path
        ext_str = ".c" if args.c_array else ".bin"
        args.output = os.path.splitext(args.input)[0] + ext_str

    # `--c-array` without argument
    if type(args.c_array) == bool:
        import os

        # Use `output_filename` as C array identifier
        args.c_array = os.path.splitext(os.path.split(args.output)[-1])[0]

    try:
        if args.c_array:
            convert_file_c_array(args.input, args.output, args.c_array)
        else:
            convert_file_binary(args.input, args.output)
    except VgmFormatError as e:
        print("ERROR: Invalid VGM file: " + str(e))
        sys.exit(1)

    print("Conversion successful!")
