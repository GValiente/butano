"""
Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
zlib License, see LICENSE file.
"""

import os
import json
import re
import string
import subprocess
import sys

from bmp import BMP
from file_info import FileInfo
from pool import create_pool


def parse_colors_count(info, bmp, tag='colors_count'):
    try:
        colors_count = int(info[tag])

        if colors_count < 1 or colors_count > 256:
            raise ValueError('Invalid colors count: ' + str(colors_count))

        extra_colors = colors_count % 16

        if extra_colors > 0:
            colors_count += 16 - extra_colors

        return colors_count
    except KeyError:
        return bmp.colors_count


def parse_sprite_bpp_mode(info, colors_count):
    try:
        bpp_mode = str(info['bpp_mode'])

        if bpp_mode == 'bpp_8':
            return True
        elif bpp_mode == 'bpp_4':
            if colors_count > 16:
                raise ValueError('Too many colors for BPP4 mode: ' + str(colors_count))

            return False
        else:
            raise ValueError('Invalid BPP mode: ' + bpp_mode)
    except KeyError:
        return colors_count > 16


def parse_bg_bpp_mode(info, file_name_no_ext):
    try:
        bpp_mode = str(info['bpp_mode'])
    except KeyError:
        raise ValueError('bpp_mode field not found in graphics json file: ' + file_name_no_ext + '.json')

    if bpp_mode == 'bpp_8':
        return True
    elif bpp_mode == 'bpp_4':
        return False
    else:
        raise ValueError('Invalid BPP mode: ' + bpp_mode)


def validate_palette_item(palette_item):
    if len(palette_item) == 0:
        raise ValueError('Empty palette item')

    if palette_item[0] not in string.ascii_lowercase:
        raise ValueError('Invalid palette item: ' + palette_item +
                         ' (invalid character: \'' + palette_item[0] + '\')')

    valid_characters = '_%s%s' % (string.ascii_lowercase, string.digits)

    for palette_item_character in palette_item:
        if palette_item_character not in valid_characters:
            raise ValueError('Invalid palette item: ' + palette_item +
                             ' (invalid character: \'' + palette_item_character + '\')')


def validate_compression(compression):
    if compression not in ['none', 'lz77', 'run_length', 'huffman', 'auto', 'auto_no_huffman']:
        raise ValueError('Unknown compression: ' + str(compression))


def compression_label(compression):
    if compression == 'none':
        return 'compression_type::NONE'

    if compression == 'lz77':
        return 'compression_type::LZ77'

    if compression == 'run_length':
        return 'compression_type::RUN_LENGTH'

    if compression == 'huffman':
        return 'compression_type::HUFFMAN'

    raise ValueError('Unknown compression: ' + str(compression))


def append_compression_command(tag, compression, command):
    if compression == 'lz77':
        command.append('-' + tag + 'zl')
    elif compression == 'run_length':
        command.append('-' + tag + 'zr')
    elif compression == 'huffman':
        command.append('-' + tag + 'zh')


def remove_file(file_path):
    if os.path.exists(file_path):
        os.remove(file_path)


class SpriteItem:

    @staticmethod
    def valid_sizes_message():
        return ' (valid sprite sizes: 8x8, 16x16, 32x32, 64x64, 16x8, 32x8, 32x16, 8x16, 8x32, 16x32, 32x64)'

    @staticmethod
    def shape_and_size(width, height):
        if width == 8:
            if height == 8:
                return 'SQUARE', 'SMALL'
            elif height == 16:
                return 'TALL', 'SMALL'
            elif height == 32:
                return'TALL', 'NORMAL'
            elif height == 64:
                raise ValueError('Invalid sprite size: (' + str(width) + 'x' + str(height) + ')' +
                                 SpriteItem.valid_sizes_message())
            else:
                raise ValueError('Invalid sprite height: ' + str(height) + SpriteItem.valid_sizes_message())
        elif width == 16:
            if height == 8:
                return 'WIDE', 'SMALL'
            elif height == 16:
                return 'SQUARE',  'NORMAL'
            elif height == 32:
                return 'TALL', 'BIG'
            elif height == 64:
                raise ValueError('Invalid sprite size: (: ' + str(width) + 'x' + str(height) + ')' +
                                 SpriteItem.valid_sizes_message())
            else:
                raise ValueError('Invalid sprite height: ' + str(height) + SpriteItem.valid_sizes_message())
        elif width == 32:
            if height == 8:
                return 'WIDE', 'NORMAL'
            elif height == 16:
                return 'WIDE', 'BIG'
            elif height == 32:
                return 'SQUARE', 'BIG'
            elif height == 64:
                return 'TALL', 'HUGE'
            else:
                raise ValueError('Invalid sprite height: ' + str(height) + SpriteItem.valid_sizes_message())
        elif width == 64:
            if height == 8:
                raise ValueError('Invalid sprite size: (' + str(width) + 'x' + str(height) + ')' +
                                 SpriteItem.valid_sizes_message())
            elif height == 16:
                raise ValueError('Invalid sprite size: (' + str(width) + 'x' + str(height) + ')' +
                                 SpriteItem.valid_sizes_message())
            elif height == 32:
                return 'WIDE', 'HUGE'
            elif height == 64:
                return 'SQUARE', 'HUGE'
            else:
                raise ValueError('Invalid sprite height: ' + str(height) + SpriteItem.valid_sizes_message())
        else:
            raise ValueError('Invalid sprite width: ' + str(width) + SpriteItem.valid_sizes_message())

    def __init__(self, file_path, file_name_no_ext, build_folder_path, info):
        bmp = BMP(file_path)
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__build_folder_path = build_folder_path

        try:
            height = int(info['height'])

            if bmp.height % height:
                raise ValueError('File height is not divisible by item height: ' +
                                 str(bmp.height) + ' - ' + str(height))

            self.__graphics = int(bmp.height / height)
        except KeyError:
            height = bmp.height
            self.__graphics = 1

        self.__shape, self.__size = SpriteItem.shape_and_size(bmp.width, height)
        self.__colors_count = parse_colors_count(info, bmp)
        self.__bpp_8 = parse_sprite_bpp_mode(info, self.__colors_count)

        try:
            self.__tiles_compression = info['tiles_compression']
            validate_compression(self.__tiles_compression)
        except KeyError:
            try:
                self.__tiles_compression = info['compression']
                validate_compression(self.__tiles_compression)
            except KeyError:
                self.__tiles_compression = 'none'

        try:
            self.__palette_compression = info['palette_compression']
            validate_compression(self.__palette_compression)
        except KeyError:
            try:
                self.__palette_compression = info['compression']
                validate_compression(self.__palette_compression)
            except KeyError:
                self.__palette_compression = 'none'

    def process(self, grit):
        tiles_compression = self.__tiles_compression
        palette_compression = self.__palette_compression

        if tiles_compression.startswith('auto'):
            test_huffman = tiles_compression == 'auto'
            tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'none', None)
            tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'run_length',
                                                                         file_size)
            tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'lz77', file_size)

            if test_huffman:
                tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'huffman',
                                                                             file_size)

        if palette_compression.startswith('auto'):
            test_huffman = palette_compression == 'auto'
            palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'none', None)
            palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'run_length',
                                                                             file_size)
            palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'lz77',
                                                                             file_size)

            if test_huffman:
                palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'huffman',
                                                                                 file_size)

        self.__execute_command(grit, tiles_compression, palette_compression)
        return self.__write_header(tiles_compression, palette_compression, False)

    def __test_tiles_compression(self, grit, best_tiles_compression, new_tiles_compression, best_file_size):
        self.__execute_command(grit, new_tiles_compression, 'none')
        new_file_size = self.__write_header(new_tiles_compression, 'none', True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_tiles_compression, new_file_size

        return best_tiles_compression, best_file_size

    def __test_palette_compression(self, grit, best_palette_compression, new_palette_compression, best_file_size):
        self.__execute_command(grit, 'none', new_palette_compression)
        new_file_size = self.__write_header('none', new_palette_compression, True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_palette_compression, new_file_size

        return best_palette_compression, best_file_size

    def __write_header(self, tiles_compression, palette_compression, skip_write):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_bn_gfx.h'
        header_file_path = self.__build_folder_path + '/bn_sprite_items_' + name + '.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned int', 'bn::tile')
            grit_data = grit_data.replace('unsigned short', 'bn::color')

            for grit_line in grit_data.splitlines():
                if ' tiles ' in grit_line:
                    for grit_word in grit_line.split():
                        try:
                            tiles_count = int(grit_word)
                            break
                        except ValueError:
                            pass

                if 'Total size:' in grit_line:
                    total_size = int(grit_line.split()[-1])

                    if skip_write:
                        return total_size
                    else:
                        break

        remove_file(grit_file_path)

        if self.__bpp_8:
            bpp_mode_label = 'bpp_mode::BPP_8'
            tiles_count *= 2
        else:
            bpp_mode_label = 'bpp_mode::BPP_4'

        grit_data = re.sub(r'Tiles\[([0-9]+)]', 'Tiles[' + str(tiles_count) + ']', grit_data)
        grit_data = re.sub(r'Pal\[([0-9]+)]', 'Pal[' + str(self.__colors_count) + ']', grit_data)

        with open(header_file_path, 'w') as header_file:
            include_guard = 'BN_SPRITE_ITEMS_' + name.upper() + '_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "bn_sprite_item.h"' + '\n')
            header_file.write(grit_data)
            header_file.write('\n')
            header_file.write('namespace bn::sprite_items' + '\n')
            header_file.write('{' + '\n')
            header_file.write('    constexpr inline sprite_item ' + name + '(' +
                              'sprite_shape_size(sprite_shape::' + self.__shape + ', ' +
                              'sprite_size::' + self.__size + '), ' + '\n            ' +
                              'sprite_tiles_item(span<const tile>(' + name + '_bn_gfxTiles, ' +
                              str(tiles_count) + '), ' + bpp_mode_label + ', ' + compression_label(tiles_compression) +
                              ', ' + str(self.__graphics) + '), ' + '\n            ' +
                              'sprite_palette_item(span<const color>(' + name + '_bn_gfxPal, ' +
                              str(self.__colors_count) + '), ' + bpp_mode_label + ', ' +
                              compression_label(palette_compression) + '));\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        return total_size, header_file_path

    def __execute_command(self, grit, tiles_compression, palette_compression):
        command = [grit, self.__file_path, '-gt', '-pe' + str(self.__colors_count)]

        if self.__bpp_8:
            command.append('-gB8')
        else:
            command.append('-gB4')

        append_compression_command('g', tiles_compression, command)
        append_compression_command('p', palette_compression, command)
        command.append('-o' + self.__build_folder_path + '/' + self.__file_name_no_ext + '_bn_gfx')
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError(grit + ' call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


class SpriteTilesItem:

    @staticmethod
    def valid_sizes_message():
        return ' (valid sprite sizes: 8x8, 16x16, 32x32, 64x64, 16x8, 32x8, 32x16, 8x16, 8x32, 16x32, 32x64)'

    def __init__(self, file_path, file_name_no_ext, build_folder_path, info):
        bmp = BMP(file_path)
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__build_folder_path = build_folder_path

        try:
            height = int(info['height'])

            if bmp.height % height:
                raise ValueError('File height is not divisible by item height: ' +
                                 str(bmp.height) + ' - ' + str(height))

            self.__graphics = int(bmp.height / height)
        except KeyError:
            height = bmp.height
            self.__graphics = 1

        self.__shape, self.__size = SpriteItem.shape_and_size(bmp.width, height)
        self.__colors_count = parse_colors_count(info, bmp)
        self.__bpp_8 = parse_sprite_bpp_mode(info, self.__colors_count)

        try:
            self.__compression = info['compression']
            validate_compression(self.__compression)
        except KeyError:
            self.__compression = 'none'

    def process(self, grit):
        compression = self.__compression

        if compression.startswith('auto'):
            test_huffman = compression == 'auto'
            compression, file_size = self.__test_compression(grit, compression, 'none', None)
            compression, file_size = self.__test_compression(grit, compression, 'run_length', file_size)
            compression, file_size = self.__test_compression(grit, compression, 'lz77', file_size)

            if test_huffman:
                compression, file_size = self.__test_compression(grit, compression, 'huffman', file_size)

        self.__execute_command(grit, compression)
        return self.__write_header(compression, False)

    def __test_compression(self, grit, best_compression, new_compression, best_file_size):
        self.__execute_command(grit, new_compression)
        new_file_size = self.__write_header(new_compression, True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_compression, new_file_size

        return best_compression, best_file_size

    def __write_header(self, compression, skip_write):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_bn_gfx.h'
        header_file_path = self.__build_folder_path + '/bn_sprite_tiles_items_' + name + '.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned int', 'bn::tile')

            for grit_line in grit_data.splitlines():
                if ' tiles ' in grit_line:
                    for grit_word in grit_line.split():
                        try:
                            tiles_count = int(grit_word)
                            break
                        except ValueError:
                            pass

                if 'Total size:' in grit_line:
                    total_size = int(grit_line.split()[-1])

                    if skip_write:
                        return total_size
                    else:
                        break

        remove_file(grit_file_path)

        if self.__bpp_8:
            bpp_mode_label = 'bpp_mode::BPP_8'
            tiles_count *= 2
        else:
            bpp_mode_label = 'bpp_mode::BPP_4'

        grit_data = re.sub(r'Tiles\[([0-9]+)]', 'Tiles[' + str(tiles_count) + ']', grit_data)

        with open(header_file_path, 'w') as header_file:
            include_guard = 'BN_SPRITE_TILES_ITEMS_' + name.upper() + '_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "bn_sprite_tiles_item.h"' + '\n')
            header_file.write('#include "bn_sprite_shape_size.h"' + '\n')
            header_file.write(grit_data)
            header_file.write('\n')
            header_file.write('namespace bn::sprite_tiles_items' + '\n')
            header_file.write('{' + '\n')
            header_file.write('    constexpr inline sprite_tiles_item ' + name + '(span<const tile>(' +
                              name + '_bn_gfxTiles, ' + str(tiles_count) + '), ' + '\n            ' +
                              bpp_mode_label + ', ' + compression_label(compression) + ', ' +
                              str(self.__graphics) + ');' + '\n')
            header_file.write('\n')
            header_file.write('    constexpr inline sprite_shape_size ' + name +
                              '_shape_size(sprite_shape::' + self.__shape + ', ' +
                              'sprite_size::' + self.__size + ');' + '\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        return total_size, header_file_path

    def __execute_command(self, grit, compression):
        command = [grit, self.__file_path, '-gt', '-p!']

        if self.__bpp_8:
            command.append('-gB8')
        else:
            command.append('-gB4')

        append_compression_command('g', compression, command)
        command.append('-o' + self.__build_folder_path + '/' + self.__file_name_no_ext + '_bn_gfx')
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError(grit + ' call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


class SpritePaletteItem:

    def __init__(self, file_path, file_name_no_ext, build_folder_path, info):
        bmp = BMP(file_path)
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__build_folder_path = build_folder_path
        self.__colors_count = parse_colors_count(info, bmp)
        self.__bpp_8 = parse_sprite_bpp_mode(info, self.__colors_count)

        try:
            self.__compression = info['compression']
            validate_compression(self.__compression)
        except KeyError:
            self.__compression = 'none'

    def process(self, grit):
        compression = self.__compression

        if compression.startswith('auto'):
            test_huffman = compression == 'auto'
            compression, file_size = self.__test_compression(grit, compression, 'none', None)
            compression, file_size = self.__test_compression(grit, compression, 'run_length', file_size)
            compression, file_size = self.__test_compression(grit, compression, 'lz77', file_size)

            if test_huffman:
                compression, file_size = self.__test_compression(grit, compression, 'huffman', file_size)

        self.__execute_command(grit, compression)
        return self.__write_header(compression, False)

    def __test_compression(self, grit, best_compression, new_compression, best_file_size):
        self.__execute_command(grit, new_compression)
        new_file_size = self.__write_header(new_compression, True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_compression, new_file_size

        return best_compression, best_file_size

    def __write_header(self, compression, skip_write):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_bn_gfx.h'
        header_file_path = self.__build_folder_path + '/bn_sprite_palette_items_' + name + '.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned short', 'bn::color')

            for grit_line in grit_data.splitlines():
                if 'Total size:' in grit_line:
                    total_size = int(grit_line.split()[-1])

                    if skip_write:
                        return total_size
                    else:
                        break

        remove_file(grit_file_path)

        if self.__bpp_8:
            bpp_mode_label = 'bpp_mode::BPP_8'
        else:
            bpp_mode_label = 'bpp_mode::BPP_4'

        grit_data = re.sub(r'Pal\[([0-9]+)]', 'Pal[' + str(self.__colors_count) + ']', grit_data)

        with open(header_file_path, 'w') as header_file:
            include_guard = 'BN_SPRITE_PALETTE_ITEMS_' + name.upper() + '_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "bn_sprite_palette_item.h"' + '\n')
            header_file.write(grit_data)
            header_file.write('\n')
            header_file.write('namespace bn::sprite_palette_items' + '\n')
            header_file.write('{' + '\n')
            header_file.write('    constexpr inline sprite_palette_item ' + name + '(' +
                              'span<const color>(' + name + '_bn_gfxPal, ' +
                              str(self.__colors_count) + '), ' + '\n            ' +
                              bpp_mode_label + ', ' + compression_label(compression) + ');' + '\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        return total_size, header_file_path

    def __execute_command(self, grit, compression):
        command = [grit, self.__file_path, '-g!', '-pe' + str(self.__colors_count)]
        append_compression_command('p', compression, command)
        command.append('-o' + self.__build_folder_path + '/' + self.__file_name_no_ext + '_bn_gfx')
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError(grit + ' call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


class RegularBgItem:

    def __init__(self, file_path, file_name_no_ext, build_folder_path, info):
        bmp = BMP(file_path)
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__build_folder_path = build_folder_path

        width = bmp.width

        if width % 256 != 0:
            raise ValueError('Regular BGs width must be divisible by 256: ' + str(width))

        try:
            height = int(info['height'])

            if bmp.height % height:
                raise ValueError('File height is not divisible by item height: ' +
                                 str(bmp.height) + ' - ' + str(height))

            self.__maps = int(bmp.height / height)
        except KeyError:
            height = bmp.height
            self.__maps = 1

        if height % 256 != 0:
            raise ValueError('Regular BGs height must be divisible by 256: ' + str(height))

        big_dimensions = width > 512 or height > 512

        try:
            self.__big = bool(info['big'])

            if self.__big:
                if width == 256 and height == 256:
                    raise ValueError('Too small size for a big regular BG: ' + str(width) + ' - ' + str(height))
            else:
                if big_dimensions:
                    raise ValueError('Too big size for a not big regular BG: ' + str(width) + ' - ' + str(height))
        except KeyError:
            self.__big = big_dimensions

        self.__width = int(width / 8)
        self.__height = int(height / 8)
        self.__bpp_8 = False

        if self.__big:
            self.__sbb = False
        else:
            self.__sbb = width == 512 or height == 512

        try:
            self.__repeated_tiles_reduction = bool(info['repeated_tiles_reduction'])
        except KeyError:
            self.__repeated_tiles_reduction = True

        try:
            self.__flipped_tiles_reduction = bool(info['flipped_tiles_reduction'])
        except KeyError:
            self.__flipped_tiles_reduction = True

        try:
            self.__palette_reduction = bool(info['palette_reduction'])
        except KeyError:
            self.__palette_reduction = True

        try:
            self.__palette_item = str(info['palette_item'])
            validate_palette_item(self.__palette_item)
            self.__colors_count = 0
        except KeyError:
            self.__palette_item = None
            self.__colors_count = parse_colors_count(info, bmp)

        try:
            bpp_mode = str(info['bpp_mode'])

            if bpp_mode == 'bpp_8':
                self.__bpp_8 = True
            elif bpp_mode == 'bpp_4_auto':
                if self.__palette_item is not None:
                    raise ValueError('BPP mode not supported with an external palette item: ' + bpp_mode)

                self.__file_path = self.__build_folder_path + '/' + file_name_no_ext + '.bn_quantized.bmp'
                self.__colors_count = bmp.quantize(self.__file_path)
            elif bpp_mode != 'bpp_4' and bpp_mode != 'bpp_4_manual':
                raise ValueError('Invalid BPP mode: ' + bpp_mode)
        except KeyError:
            if self.__palette_item is not None:
                raise ValueError('bpp_mode field not found in graphics json file: ' + file_name_no_ext + '.json')

            if self.__colors_count > 16:
                self.__bpp_8 = True

        try:
            self.__tiles_compression = info['tiles_compression']
            validate_compression(self.__tiles_compression)
        except KeyError:
            try:
                self.__tiles_compression = info['compression']
                validate_compression(self.__tiles_compression)
            except KeyError:
                self.__tiles_compression = 'none'

        if self.__palette_item is not None:
            self.__palette_compression = 'none'
        else:
            try:
                self.__palette_compression = info['palette_compression']
                validate_compression(self.__palette_compression)
            except KeyError:
                try:
                    self.__palette_compression = info['compression']
                    validate_compression(self.__palette_compression)
                except KeyError:
                    self.__palette_compression = 'none'

        try:
            self.__map_compression = info['map_compression']
            validate_compression(self.__map_compression)
        except KeyError:
            try:
                self.__map_compression = info['compression']
                validate_compression(self.__map_compression)
            except KeyError:
                self.__map_compression = 'none'

    def process(self, grit):
        tiles_compression = self.__tiles_compression
        palette_compression = self.__palette_compression
        map_compression = self.__map_compression

        if tiles_compression.startswith('auto'):
            test_huffman = tiles_compression == 'auto'
            tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'none', None)
            tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'run_length',
                                                                         file_size)
            tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'lz77', file_size)

            if test_huffman:
                tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'huffman',
                                                                             file_size)

        if palette_compression.startswith('auto'):
            test_huffman = palette_compression == 'auto'
            palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'none', None)
            palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'run_length',
                                                                             file_size)
            palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'lz77',
                                                                             file_size)

            if test_huffman:
                palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'huffman',
                                                                                 file_size)

        if map_compression.startswith('auto'):
            test_huffman = map_compression == 'auto'
            map_compression, file_size = self.__test_map_compression(grit, map_compression, 'none', None)
            map_compression, file_size = self.__test_map_compression(grit, map_compression, 'run_length', file_size)
            map_compression, file_size = self.__test_map_compression(grit, map_compression, 'lz77', file_size)

            if test_huffman:
                map_compression, file_size = self.__test_map_compression(grit, map_compression, 'huffman', file_size)

        self.__execute_command(grit, tiles_compression, palette_compression, map_compression)
        return self.__write_header(tiles_compression, palette_compression, map_compression, False)

    def __test_tiles_compression(self, grit, best_tiles_compression, new_tiles_compression, best_file_size):
        self.__execute_command(grit, new_tiles_compression, 'none', 'none')
        new_file_size = self.__write_header(new_tiles_compression, 'none', 'none', True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_tiles_compression, new_file_size

        return best_tiles_compression, best_file_size

    def __test_palette_compression(self, grit, best_palette_compression, new_palette_compression, best_file_size):
        self.__execute_command(grit, 'none', new_palette_compression, 'none')
        new_file_size = self.__write_header('none', new_palette_compression, 'none', True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_palette_compression, new_file_size

        return best_palette_compression, best_file_size

    def __test_map_compression(self, grit, best_map_compression, new_map_compression, best_file_size):
        self.__execute_command(grit, 'none', 'none', new_map_compression)
        new_file_size = self.__write_header('none', 'none', new_map_compression, True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_map_compression, new_file_size

        return best_map_compression, best_file_size

    def __write_header(self, tiles_compression, palette_compression, map_compression, skip_write):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_bn_gfx.h'
        header_file_path = self.__build_folder_path + '/bn_regular_bg_items_' + name + '.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned int', 'bn::tile', 1)
            grit_data = grit_data.replace('unsigned short', 'bn::regular_bg_map_cell', 1)

            if self.__palette_item is None:
                grit_data = grit_data.replace('unsigned short', 'bn::color', 1)

            for grit_line in grit_data.splitlines():
                if ' tiles ' in grit_line:
                    for grit_word in grit_line.split():
                        try:
                            tiles_count = int(grit_word)
                            break
                        except ValueError:
                            pass

                    if tiles_count > 1024:
                        raise ValueError('Regular BGs with more than 1024 tiles not supported: ' + str(tiles_count))

                if 'Total size:' in grit_line:
                    total_size = int(grit_line.split()[-1])

                    if skip_write:
                        return total_size
                    else:
                        break

        remove_file(grit_file_path)

        if self.__bpp_8:
            bpp_mode_label = 'bpp_mode::BPP_8'
            tiles_count *= 2
        else:
            bpp_mode_label = 'bpp_mode::BPP_4'

        grit_data = re.sub(r'Tiles\[([0-9]+)]', 'Tiles[' + str(tiles_count) + ']', grit_data)
        grit_data = re.sub(r'Pal\[([0-9]+)]', 'Pal[' + str(self.__colors_count) + ']', grit_data)

        with open(header_file_path, 'w') as header_file:
            include_guard = 'BN_REGULAR_BG_ITEMS_' + name.upper() + '_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "bn_regular_bg_item.h"' + '\n')
            header_file.write(grit_data)
            header_file.write('\n')

            if self.__palette_item is not None:
                header_file.write('#include "bn_bg_palette_items_' + self.__palette_item + '.h"' + '\n')
                header_file.write('\n')

            header_file.write('namespace bn::regular_bg_items' + '\n')
            header_file.write('{' + '\n')
            header_file.write('    constexpr inline regular_bg_item ' + name + '(' + '\n            ' +
                              'regular_bg_tiles_item(span<const tile>(' + name + '_bn_gfxTiles, ' +
                              str(tiles_count) + '), ' + bpp_mode_label + ', ' + compression_label(tiles_compression) +
                              '), ' + '\n            ')

            if self.__palette_item is None:
                header_file.write('bg_palette_item(span<const color>(' + name + '_bn_gfxPal, ' +
                                  str(self.__colors_count) + '), ' + bpp_mode_label + ', ' +
                                  compression_label(palette_compression) + '),' + '\n            ')
            else:
                header_file.write('bn::bg_palette_items::' + self.__palette_item + ',' + '\n            ')

            header_file.write('regular_bg_map_item(' + name + '_bn_gfxMap[0], ' +
                              'size(' + str(self.__width) + ', ' + str(self.__height) + '), ' +
                              compression_label(map_compression) + ', ' + str(self.__maps) + ', ' +
                              str(self.__big).lower() + '));' + '\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        return total_size, header_file_path

    def __execute_command(self, grit, tiles_compression, palette_compression, map_compression):
        command = [grit, self.__file_path]

        if self.__colors_count > 0:
            command.append('-pe' + str(self.__colors_count))
        else:
            command.append('-p!')

        map_argument = '-mR'

        if self.__repeated_tiles_reduction:
            map_argument += 't'

        if self.__flipped_tiles_reduction:
            map_argument += 'f'

        if self.__bpp_8:
            command.append('-gB8')
        else:
            command.append('-gB4')

            if self.__palette_reduction:
                map_argument += 'p'

        if map_argument == '-mR':
            map_argument += '!'

        command.append(map_argument)

        if self.__sbb:
            command.append('-mLs')
        else:
            command.append('-mLf')

        append_compression_command('g', tiles_compression, command)
        append_compression_command('p', palette_compression, command)
        append_compression_command('m', map_compression, command)
        command.append('-o' + self.__build_folder_path + '/' + self.__file_name_no_ext + '_bn_gfx')
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError(grit + ' call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


class RegularBgTilesItem:

    def __init__(self, file_path, file_name_no_ext, build_folder_path, info):
        bmp = BMP(file_path)
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__build_folder_path = build_folder_path
        self.__bpp_8 = parse_bg_bpp_mode(info, file_name_no_ext)

        try:
            self.__tiles_compression = info['compression']
            validate_compression(self.__tiles_compression)
        except KeyError:
            self.__tiles_compression = 'none'

        try:
            self.__generate_palette = bool(info['generate_palette'])
        except KeyError:
            self.__generate_palette = False

        if self.__generate_palette:
            self.__palette_colors_count = parse_colors_count(info, bmp, 'palette_colors_count')

            try:
                self.__palette_compression = info['palette_compression']
                validate_compression(self.__palette_compression)
            except KeyError:
                self.__palette_compression = 'none'
        else:
            self.__palette_colors_count = 0
            self.__palette_compression = 'none'

    def process(self, grit):
        tiles_compression = self.__tiles_compression
        palette_compression = self.__palette_compression

        if tiles_compression.startswith('auto'):
            test_huffman = tiles_compression == 'auto'
            tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'none', None)
            tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'run_length',
                                                                         file_size)
            tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'lz77', file_size)

            if test_huffman:
                tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'huffman',
                                                                             file_size)

        if palette_compression.startswith('auto'):
            test_huffman = palette_compression == 'auto'
            palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'none', None)
            palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'run_length',
                                                                             file_size)
            palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'lz77',
                                                                             file_size)

            if test_huffman:
                palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'huffman',
                                                                                 file_size)

        self.__execute_command(grit, tiles_compression, palette_compression)
        return self.__write_header(tiles_compression, palette_compression, False)

    def __test_tiles_compression(self, grit, best_compression, new_compression, best_file_size):
        self.__execute_command(grit, new_compression, 'none')
        new_file_size = self.__write_header(new_compression, 'none', True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_compression, new_file_size

        return best_compression, best_file_size

    def __test_palette_compression(self, grit, best_compression, new_compression, best_file_size):
        self.__execute_command(grit, 'none', new_compression)
        new_file_size = self.__write_header('none', new_compression, True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_compression, new_file_size

        return best_compression, best_file_size

    def __write_header(self, tiles_compression, palette_compression, skip_write):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_bn_gfx.h'
        header_file_path = self.__build_folder_path + '/bn_regular_bg_tiles_items_' + name + '.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned int', 'bn::tile', 1)

            if self.__generate_palette:
                grit_data = grit_data.replace('unsigned short', 'bn::color', 1)

            for grit_line in grit_data.splitlines():
                if ' tiles ' in grit_line:
                    for grit_word in grit_line.split():
                        try:
                            tiles_count = int(grit_word)
                            break
                        except ValueError:
                            pass

                    if tiles_count > 1024:
                        raise ValueError('Regular BG tiles items with more than 1024 tiles not supported: ' +
                                         str(tiles_count))

                if 'Total size:' in grit_line:
                    total_size = int(grit_line.split()[-1])

                    if skip_write:
                        return total_size
                    else:
                        break

        remove_file(grit_file_path)

        if self.__bpp_8:
            bpp_mode_label = 'bpp_mode::BPP_8'
            tiles_count *= 2
        else:
            bpp_mode_label = 'bpp_mode::BPP_4'

        grit_data = re.sub(r'Tiles\[([0-9]+)]', 'Tiles[' + str(tiles_count) + ']', grit_data)

        if self.__generate_palette:
            grit_data = re.sub(r'Pal\[([0-9]+)]', 'Pal[' + str(self.__palette_colors_count) + ']', grit_data)

        with open(header_file_path, 'w') as header_file:
            include_guard = 'BN_REGULAR_BG_TILES_ITEMS_' + name.upper() + '_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "bn_regular_bg_tiles_item.h"' + '\n')

            if self.__generate_palette:
                header_file.write('#include "bn_bg_palette_item.h"' + '\n')

            header_file.write(grit_data)
            header_file.write('\n')
            header_file.write('namespace bn::regular_bg_tiles_items' + '\n')
            header_file.write('{' + '\n')
            header_file.write('    constexpr inline regular_bg_tiles_item ' + name + '(' + '\n            ' +
                              'span<const tile>(' + name + '_bn_gfxTiles, ' +
                              str(tiles_count) + '), ' + bpp_mode_label + ', ' + compression_label(tiles_compression) +
                              ');' + '\n')

            if self.__generate_palette:
                header_file.write('\n')
                header_file.write('    constexpr inline bg_palette_item ' + name + '_palette(' +
                                  'span<const color>(' + name + '_bn_gfxPal, ' +
                                  str(self.__palette_colors_count) + '), ' + '\n            ' +
                                  bpp_mode_label + ', ' + compression_label(palette_compression) + ');' + '\n')

            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        return total_size, header_file_path

    def __execute_command(self, grit, tiles_compression, palette_compression):
        command = [grit, self.__file_path, '-m!']

        if self.__bpp_8:
            command.append('-gB8')
        else:
            command.append('-gB4')

        append_compression_command('g', tiles_compression, command)

        if self.__generate_palette:
            command.append('-pe' + str(self.__palette_colors_count))
            append_compression_command('p', palette_compression, command)
        else:
            command.append('-p!')

        command.append('-o' + self.__build_folder_path + '/' + self.__file_name_no_ext + '_bn_gfx')
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError(grit + ' call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


class AffineBgItem:

    def __init__(self, file_path, file_name_no_ext, build_folder_path, info):
        bmp = BMP(file_path)
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__build_folder_path = build_folder_path

        width = bmp.width

        if width != 128 and width % 256 != 0:
            raise ValueError('Affine BGs width must be 128 or divisible by 256: ' + str(width))

        try:
            height = int(info['height'])

            if bmp.height % height:
                raise ValueError('File height is not divisible by item height: ' +
                                 str(bmp.height) + ' - ' + str(height))

            self.__maps = int(bmp.height / height)
        except KeyError:
            height = bmp.height
            self.__maps = 1

        if height != 128 and height % 256 != 0:
            raise ValueError('Affine BGs height must be 128 or divisible by 256: ' + str(height))

        if width == 128 and height != 128:
            raise ValueError('If the width of an affine BG is 128, its height must be 128: ' +
                             str(width) + ' - ' + str(height))

        if height == 128 and width != 128:
            raise ValueError('If the height of an affine BG is 128, its width must be 128: ' +
                             str(width) + ' - ' + str(height))

        big_dimensions = width != height or (width != 128 and width != 256 and width != 512 and width != 1024)

        try:
            self.__big = bool(info['big'])

            if self.__big:
                if width <= 256 and height <= 256:
                    raise ValueError('Too small size for a big affine BG: ' + str(width) + ' - ' + str(height))
            else:
                if big_dimensions:
                    raise ValueError('Too big size for a not big affine BG: ' + str(width) + ' - ' + str(height))
        except KeyError:
            self.__big = big_dimensions

        self.__width = int(width / 8)
        self.__height = int(height / 8)

        try:
            self.__repeated_tiles_reduction = bool(info['repeated_tiles_reduction'])
        except KeyError:
            self.__repeated_tiles_reduction = True

        try:
            self.__palette_item = str(info['palette_item'])
            validate_palette_item(self.__palette_item)
            self.__colors_count = 0
        except KeyError:
            self.__palette_item = None
            self.__colors_count = parse_colors_count(info, bmp)

        try:
            self.__tiles_compression = info['tiles_compression']
            validate_compression(self.__tiles_compression)
        except KeyError:
            try:
                self.__tiles_compression = info['compression']
                validate_compression(self.__tiles_compression)
            except KeyError:
                self.__tiles_compression = 'none'

        if self.__palette_item is not None:
            self.__palette_compression = 'none'
        else:
            try:
                self.__palette_compression = info['palette_compression']
                validate_compression(self.__palette_compression)
            except KeyError:
                try:
                    self.__palette_compression = info['compression']
                    validate_compression(self.__palette_compression)
                except KeyError:
                    self.__palette_compression = 'none'

        try:
            self.__map_compression = info['map_compression']
            validate_compression(self.__map_compression)
        except KeyError:
            try:
                self.__map_compression = info['compression']
                validate_compression(self.__map_compression)
            except KeyError:
                self.__map_compression = 'none'

    def process(self, grit):
        tiles_compression = self.__tiles_compression
        palette_compression = self.__palette_compression
        map_compression = self.__map_compression

        if tiles_compression.startswith('auto'):
            test_huffman = tiles_compression == 'auto'
            tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'none', None)
            tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'run_length',
                                                                         file_size)
            tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'lz77', file_size)

            if test_huffman:
                tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'huffman',
                                                                             file_size)

        if palette_compression.startswith('auto'):
            test_huffman = palette_compression == 'auto'
            palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'none', None)
            palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'run_length',
                                                                             file_size)
            palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'lz77',
                                                                             file_size)

            if test_huffman:
                palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'huffman',
                                                                                 file_size)

        if map_compression.startswith('auto'):
            test_huffman = map_compression == 'auto'
            map_compression, file_size = self.__test_map_compression(grit, map_compression, 'none', None)
            map_compression, file_size = self.__test_map_compression(grit, map_compression, 'run_length', file_size)
            map_compression, file_size = self.__test_map_compression(grit, map_compression, 'lz77', file_size)

            if test_huffman:
                map_compression, file_size = self.__test_map_compression(grit, map_compression, 'huffman', file_size)

        self.__execute_command(grit, tiles_compression, palette_compression, map_compression)
        return self.__write_header(tiles_compression, palette_compression, map_compression, False)

    def __test_tiles_compression(self, grit, best_tiles_compression, new_tiles_compression, best_file_size):
        self.__execute_command(grit, new_tiles_compression, 'none', 'none')
        new_file_size = self.__write_header(new_tiles_compression, 'none', 'none', True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_tiles_compression, new_file_size

        return best_tiles_compression, best_file_size

    def __test_palette_compression(self, grit, best_palette_compression, new_palette_compression, best_file_size):
        self.__execute_command(grit, 'none', new_palette_compression, 'none')
        new_file_size = self.__write_header('none', new_palette_compression, 'none', True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_palette_compression, new_file_size

        return best_palette_compression, best_file_size

    def __test_map_compression(self, grit, best_map_compression, new_map_compression, best_file_size):
        self.__execute_command(grit, 'none', 'none', new_map_compression)
        new_file_size = self.__write_header('none', 'none', new_map_compression, True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_map_compression, new_file_size

        return best_map_compression, best_file_size

    def __write_header(self, tiles_compression, palette_compression, map_compression, skip_write):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_bn_gfx.h'
        header_file_path = self.__build_folder_path + '/bn_affine_bg_items_' + name + '.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned int', 'bn::tile', 1)
            grit_data = grit_data.replace('unsigned char', 'bn::affine_bg_map_cell', 1)

            if self.__palette_item is None:
                grit_data = grit_data.replace('unsigned short', 'bn::color', 1)

            for grit_line in grit_data.splitlines():
                if ' tiles ' in grit_line:
                    for grit_word in grit_line.split():
                        try:
                            tiles_count = int(grit_word)
                            break
                        except ValueError:
                            pass

                    if tiles_count > 256:
                        raise ValueError('Affine BGs with more than 256 tiles not supported: ' + str(tiles_count))

                if 'Total size:' in grit_line:
                    total_size = int(grit_line.split()[-1])

                    if skip_write:
                        return total_size
                    else:
                        break

        remove_file(grit_file_path)

        tiles_count *= 2
        grit_data = re.sub(r'Tiles\[([0-9]+)]', 'Tiles[' + str(tiles_count) + ']', grit_data)
        grit_data = re.sub(r'Pal\[([0-9]+)]', 'Pal[' + str(self.__colors_count) + ']', grit_data)

        with open(header_file_path, 'w') as header_file:
            include_guard = 'BN_AFFINE_BG_ITEMS_' + name.upper() + '_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "bn_affine_bg_item.h"' + '\n')
            header_file.write(grit_data)
            header_file.write('\n')

            if self.__palette_item is not None:
                header_file.write('#include "bn_bg_palette_items_' + self.__palette_item + '.h"' + '\n')
                header_file.write('\n')

            header_file.write('namespace bn::affine_bg_items' + '\n')
            header_file.write('{' + '\n')
            header_file.write('    constexpr inline affine_bg_item ' + name + '(' + '\n            ' +
                              'affine_bg_tiles_item(span<const tile>(' + name + '_bn_gfxTiles, ' +
                              str(tiles_count) + '), ' + compression_label(tiles_compression) +
                              '), ' + '\n            ')

            if self.__palette_item is None:
                header_file.write('bg_palette_item(span<const color>(' + name + '_bn_gfxPal, ' +
                                  str(self.__colors_count) + '), bpp_mode::BPP_8, ' +
                                  compression_label(palette_compression) + '),' + '\n            ')
            else:
                header_file.write('bn::bg_palette_items::' + self.__palette_item + ',' + '\n            ')

            header_file.write('affine_bg_map_item(' + name + '_bn_gfxMap[0], ' +
                              'size(' + str(self.__width) + ', ' + str(self.__height) + '), ' +
                              compression_label(map_compression) + ', ' + str(self.__maps) + ', ' +
                              str(self.__big).lower() + '));' + '\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        return total_size, header_file_path

    def __execute_command(self, grit, tiles_compression, palette_compression, map_compression):
        command = [grit, self.__file_path, '-gB8', '-mLa', '-mu8']

        if self.__colors_count > 0:
            command.append('-pe' + str(self.__colors_count))
        else:
            command.append('-p!')

        if self.__repeated_tiles_reduction:
            command.append('-mRt')
        else:
            command.append('-mR!')

        append_compression_command('g', tiles_compression, command)
        append_compression_command('p', palette_compression, command)
        append_compression_command('m', map_compression, command)
        command.append('-o' + self.__build_folder_path + '/' + self.__file_name_no_ext + '_bn_gfx')
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError(grit + ' call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


class AffineBgTilesItem:

    def __init__(self, file_path, file_name_no_ext, build_folder_path, info):
        bmp = BMP(file_path)
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__build_folder_path = build_folder_path

        try:
            self.__tiles_compression = info['compression']
            validate_compression(self.__tiles_compression)
        except KeyError:
            self.__tiles_compression = 'none'

        try:
            self.__generate_palette = bool(info['generate_palette'])
        except KeyError:
            self.__generate_palette = False

        if self.__generate_palette:
            self.__palette_colors_count = parse_colors_count(info, bmp, 'palette_colors_count')

            try:
                self.__palette_compression = info['palette_compression']
                validate_compression(self.__palette_compression)
            except KeyError:
                self.__palette_compression = 'none'
        else:
            self.__palette_colors_count = 0
            self.__palette_compression = 'none'

    def process(self, grit):
        tiles_compression = self.__tiles_compression
        palette_compression = self.__palette_compression

        if tiles_compression.startswith('auto'):
            test_huffman = tiles_compression == 'auto'
            tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'none', None)
            tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'run_length',
                                                                         file_size)
            tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'lz77', file_size)

            if test_huffman:
                tiles_compression, file_size = self.__test_tiles_compression(grit, tiles_compression, 'huffman',
                                                                             file_size)

        if palette_compression.startswith('auto'):
            test_huffman = palette_compression == 'auto'
            palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'none', None)
            palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'run_length',
                                                                             file_size)
            palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'lz77',
                                                                             file_size)

            if test_huffman:
                palette_compression, file_size = self.__test_palette_compression(grit, palette_compression, 'huffman',
                                                                                 file_size)

        self.__execute_command(grit, tiles_compression, palette_compression)
        return self.__write_header(tiles_compression, palette_compression, False)

    def __test_tiles_compression(self, grit, best_compression, new_compression, best_file_size):
        self.__execute_command(grit, new_compression, 'none')
        new_file_size = self.__write_header(new_compression, 'none', True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_compression, new_file_size

        return best_compression, best_file_size

    def __test_palette_compression(self, grit, best_compression, new_compression, best_file_size):
        self.__execute_command(grit, 'none', new_compression)
        new_file_size = self.__write_header('none', new_compression, True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_compression, new_file_size

        return best_compression, best_file_size

    def __write_header(self, tiles_compression, palette_compression, skip_write):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_bn_gfx.h'
        header_file_path = self.__build_folder_path + '/bn_affine_bg_tiles_items_' + name + '.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned int', 'bn::tile', 1)

            if self.__generate_palette:
                grit_data = grit_data.replace('unsigned short', 'bn::color', 1)

            for grit_line in grit_data.splitlines():
                if ' tiles ' in grit_line:
                    for grit_word in grit_line.split():
                        try:
                            tiles_count = int(grit_word)
                            break
                        except ValueError:
                            pass

                    if tiles_count > 256:
                        raise ValueError('Affine BG tiles items with more than 256 tiles not supported: ' +
                                         str(tiles_count))

                if 'Total size:' in grit_line:
                    total_size = int(grit_line.split()[-1])

                    if skip_write:
                        return total_size
                    else:
                        break

        remove_file(grit_file_path)

        tiles_count *= 2
        grit_data = re.sub(r'Tiles\[([0-9]+)]', 'Tiles[' + str(tiles_count) + ']', grit_data)

        if self.__generate_palette:
            grit_data = re.sub(r'Pal\[([0-9]+)]', 'Pal[' + str(self.__palette_colors_count) + ']', grit_data)

        with open(header_file_path, 'w') as header_file:
            include_guard = 'BN_AFFINE_BG_TILES_ITEMS_' + name.upper() + '_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "bn_affine_bg_tiles_item.h"' + '\n')

            if self.__generate_palette:
                header_file.write('#include "bn_bg_palette_item.h"' + '\n')

            header_file.write(grit_data)
            header_file.write('\n')
            header_file.write('namespace bn::affine_bg_tiles_items' + '\n')
            header_file.write('{' + '\n')
            header_file.write('    constexpr inline affine_bg_tiles_item ' + name + '(' + '\n            ' +
                              'span<const tile>(' + name + '_bn_gfxTiles, ' +
                              str(tiles_count) + '), ' + compression_label(tiles_compression) +
                              ');' + '\n')

            if self.__generate_palette:
                header_file.write('\n')
                header_file.write('    constexpr inline bg_palette_item ' + name + '_palette(' +
                                  'span<const color>(' + name + '_bn_gfxPal, ' +
                                  str(self.__palette_colors_count) + '), ' + '\n            ' +
                                  'bpp_mode::BPP_8, ' + compression_label(palette_compression) + ');' + '\n')

            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        return total_size, header_file_path

    def __execute_command(self, grit, tiles_compression, palette_compression):
        command = [grit, self.__file_path, '-gB8', '-m!']
        append_compression_command('g', tiles_compression, command)

        if self.__generate_palette:
            command.append('-pe' + str(self.__palette_colors_count))
            append_compression_command('p', palette_compression, command)
        else:
            command.append('-p!')

        command.append('-o' + self.__build_folder_path + '/' + self.__file_name_no_ext + '_bn_gfx')
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError(grit + ' call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


class BgPaletteItem:

    def __init__(self, file_path, file_name_no_ext, build_folder_path, info):
        bmp = BMP(file_path)
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__build_folder_path = build_folder_path
        self.__colors_count = parse_colors_count(info, bmp)
        self.__bpp_8 = parse_bg_bpp_mode(info, file_name_no_ext)

        try:
            self.__compression = info['compression']
            validate_compression(self.__compression)
        except KeyError:
            self.__compression = 'none'

    def process(self, grit):
        compression = self.__compression

        if compression.startswith('auto'):
            test_huffman = compression == 'auto'
            compression, file_size = self.__test_compression(grit, compression, 'none', None)
            compression, file_size = self.__test_compression(grit, compression, 'run_length', file_size)
            compression, file_size = self.__test_compression(grit, compression, 'lz77', file_size)

            if test_huffman:
                compression, file_size = self.__test_compression(grit, compression, 'huffman', file_size)

        self.__execute_command(grit, compression)
        return self.__write_header(compression, False)

    def __test_compression(self, grit, best_compression, new_compression, best_file_size):
        self.__execute_command(grit, new_compression)
        new_file_size = self.__write_header(new_compression, True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_compression, new_file_size

        return best_compression, best_file_size

    def __write_header(self, compression, skip_write):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_bn_gfx.h'
        header_file_path = self.__build_folder_path + '/bn_bg_palette_items_' + name + '.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned short', 'bn::color', 1)

            for grit_line in grit_data.splitlines():
                if 'Total size:' in grit_line:
                    total_size = int(grit_line.split()[-1])

                    if skip_write:
                        return total_size
                    else:
                        break

        remove_file(grit_file_path)

        if self.__bpp_8:
            bpp_mode_label = 'bpp_mode::BPP_8'
        else:
            bpp_mode_label = 'bpp_mode::BPP_4'

        grit_data = re.sub(r'Pal\[([0-9]+)]', 'Pal[' + str(self.__colors_count) + ']', grit_data)

        with open(header_file_path, 'w') as header_file:
            include_guard = 'BN_BG_PALETTE_ITEMS_' + name.upper() + '_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "bn_bg_palette_item.h"' + '\n')
            header_file.write(grit_data)
            header_file.write('\n')
            header_file.write('namespace bn::bg_palette_items' + '\n')
            header_file.write('{' + '\n')
            header_file.write('    constexpr inline bg_palette_item ' + name + '(' +
                              'span<const color>(' + name + '_bn_gfxPal, ' +
                              str(self.__colors_count) + '), ' + '\n            ' +
                              bpp_mode_label + ', ' + compression_label(compression) + ');' + '\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        return total_size, header_file_path

    def __execute_command(self, grit, compression):
        command = [grit, self.__file_path, '-g!', '-pe' + str(self.__colors_count)]
        append_compression_command('p', compression, command)
        command.append('-o' + self.__build_folder_path + '/' + self.__file_name_no_ext + '_bn_gfx')
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError(grit + ' call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


class GraphicsFileInfo:

    def __init__(self, json_file_path, file_path, file_name, file_name_no_ext, file_info_path):
        self.__json_file_path = json_file_path
        self.__file_path = file_path
        self.__file_name = file_name
        self.__file_name_no_ext = file_name_no_ext
        self.__file_info_path = file_info_path

    def print_file_name(self):
        print(self.__file_name)

    def process(self, grit, build_folder_path):
        try:
            try:
                with open(self.__json_file_path) as json_file:
                    info = json.load(json_file)
            except Exception as exception:
                raise ValueError(self.__json_file_path + ' graphics json file parse failed: ' + str(exception))

            try:
                graphics_type = str(info['type'])
            except KeyError:
                raise ValueError('type field not found in graphics json file: ' + self.__json_file_path)

            if graphics_type == 'sprite':
                item = SpriteItem(self.__file_path, self.__file_name_no_ext, build_folder_path, info)
            elif graphics_type == 'sprite_tiles':
                item = SpriteTilesItem(self.__file_path, self.__file_name_no_ext, build_folder_path, info)
            elif graphics_type == 'sprite_palette':
                item = SpritePaletteItem(self.__file_path, self.__file_name_no_ext, build_folder_path, info)
            elif graphics_type == 'regular_bg':
                item = RegularBgItem(self.__file_path, self.__file_name_no_ext, build_folder_path, info)
            elif graphics_type == 'regular_bg_tiles':
                item = RegularBgTilesItem(self.__file_path, self.__file_name_no_ext, build_folder_path, info)
            elif graphics_type == 'affine_bg':
                item = AffineBgItem(self.__file_path, self.__file_name_no_ext, build_folder_path, info)
            elif graphics_type == 'affine_bg_tiles':
                item = AffineBgTilesItem(self.__file_path, self.__file_name_no_ext, build_folder_path, info)
            elif graphics_type == 'bg_palette':
                item = BgPaletteItem(self.__file_path, self.__file_name_no_ext, build_folder_path, info)
            else:
                raise ValueError('Unknown graphics type "' + graphics_type +
                                 '" found in graphics json file: ' + self.__json_file_path)

            total_size, header_file_path = item.process(grit)

            with open(self.__file_info_path, 'w') as file_info:
                file_info.write('')

            return [self.__file_name, header_file_path, total_size]
        except Exception as exc:
            return [self.__file_name, exc]


class GraphicsFileInfoProcessor:

    def __init__(self, grit, build_folder_path):
        self.__grit = grit
        self.__build_folder_path = build_folder_path

    def __call__(self, graphics_file_info):
        return graphics_file_info.process(self.__grit, self.__build_folder_path)


def list_graphics_file_infos(graphics_paths, build_folder_path):
    graphics_file_paths = []

    for graphics_path in graphics_paths.split(' '):
        if os.path.isdir(graphics_path):
            graphics_file_names = os.listdir(graphics_path)

            for graphics_file_name in graphics_file_names:
                graphics_file_path = graphics_path + '/' + graphics_file_name

                if os.path.isfile(graphics_file_path):
                    graphics_file_paths.append(graphics_file_path)
        elif os.path.isfile(graphics_path):
            graphics_file_paths.append(graphics_path)

    graphics_file_infos = []
    file_names_set = set()

    for graphics_file_path in graphics_file_paths:
        graphics_file_name = os.path.basename(graphics_file_path)

        if FileInfo.validate(graphics_file_name):
            graphics_file_name_split = os.path.splitext(graphics_file_name)
            graphics_file_name_ext = graphics_file_name_split[1]

            if graphics_file_name_ext == '.bmp':
                graphics_file_name_no_ext = graphics_file_name_split[0]

                if graphics_file_name_no_ext in file_names_set:
                    raise ValueError('There\'s two or more graphics files with the same name: ' +
                                     graphics_file_name_no_ext)

                file_names_set.add(graphics_file_name_no_ext)
                json_file_path = graphics_file_path[:-len(graphics_file_name_ext)] + '.json'

                if not os.path.isfile(json_file_path):
                    raise ValueError('Graphics json file not found: ' + json_file_path)

                file_info_path = build_folder_path + '/_bn_' + graphics_file_name_no_ext + '_graphics_file_info.txt'

                if not os.path.exists(file_info_path):
                    build = True
                else:
                    file_info_mtime = os.path.getmtime(file_info_path)
                    graphics_file_mtime = os.path.getmtime(graphics_file_path)

                    if file_info_mtime < graphics_file_mtime:
                        build = True
                    else:
                        json_file_mtime = os.path.getmtime(json_file_path)
                        build = file_info_mtime < json_file_mtime

                if build:
                    graphics_file_infos.append(GraphicsFileInfo(
                        json_file_path, graphics_file_path, graphics_file_name, graphics_file_name_no_ext,
                        file_info_path))

    return graphics_file_infos


def process_graphics(grit, graphics_paths, build_folder_path):
    graphics_file_infos = list_graphics_file_infos(graphics_paths, build_folder_path)

    if len(graphics_file_infos) > 0:
        for graphics_file_info in graphics_file_infos:
            graphics_file_info.print_file_name()

        sys.stdout.flush()

        pool = create_pool()
        process_results = pool.map(GraphicsFileInfoProcessor(grit, build_folder_path), graphics_file_infos)
        pool.close()

        total_size = 0
        process_excs = []

        for process_result in process_results:
            if len(process_result) == 3:
                file_size = process_result[2]
                total_size += file_size
                print('    ' + str(process_result[0]) + ' item header written in ' + str(process_result[1]) +
                      ' (graphics size: ' + str(file_size) + ' bytes)')
            else:
                process_excs.append(process_result)

        sys.stdout.flush()

        if len(process_excs) > 0:
            for process_exc in process_excs:
                sys.stderr.write(str(process_exc[0]) + ' error: ' + str(process_exc[1]) + '\n')
                
            exit(-1)

        print('    ' + 'Processed graphics size: ' + str(total_size) + ' bytes')
