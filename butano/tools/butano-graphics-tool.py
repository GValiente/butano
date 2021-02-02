"""
Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
zlib License, see LICENSE file.
"""

import os
import json
import argparse
import subprocess
import sys
from multiprocessing import Pool
import traceback

from bmp import BMP
from file_info import FileInfo


def validate_compression(compression):
    if compression != 'none' and compression != 'lz77' and compression != 'run_length' and compression != 'auto':
        raise ValueError('Unknown compression: ' + str(compression))


def remove_file(file_path):
    if os.path.exists(file_path):
        os.remove(file_path)


def compression_label(compression):
    if compression == 'none':
        return 'compression_type::NONE'

    if compression == 'lz77':
        return 'compression_type::LZ77'

    if compression == 'run_length':
        return 'compression_type::RUN_LENGTH'

    raise ValueError('Unknown compression: ' + str(compression))


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
        self.__colors_count = bmp.colors_count

        try:
            height = int(info['height'])
        except KeyError:
            raise ValueError('height field not found in graphics json file: ' + file_name_no_ext + '.json')

        if bmp.height % height:
            raise ValueError('File height is not divisible by item height: ' + str(bmp.height) + ' - ' + str(height))

        self.__graphics = int(bmp.height / height)
        self.__shape, self.__size = SpriteItem.shape_and_size(bmp.width, height)

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

    def process(self):
        tiles_compression = self.__tiles_compression
        palette_compression = self.__palette_compression

        if tiles_compression == 'auto':
            tiles_compression, file_size = self.__test_tiles_compression(tiles_compression, 'none', None)
            tiles_compression, file_size = self.__test_tiles_compression(tiles_compression, 'lz77', file_size)
            tiles_compression, file_size = self.__test_tiles_compression(tiles_compression, 'run_length', file_size)

        if palette_compression == 'auto':
            palette_compression, file_size = self.__test_palette_compression(palette_compression, 'none', None)
            palette_compression, file_size = self.__test_palette_compression(palette_compression, 'lz77', file_size)
            palette_compression, file_size = self.__test_palette_compression(palette_compression, 'run_length',
                                                                             file_size)

        self.__execute_command(tiles_compression, palette_compression)
        return self.__write_header(tiles_compression, palette_compression, False)

    def __test_tiles_compression(self, best_tiles_compression, new_tiles_compression, best_file_size):
        self.__execute_command(new_tiles_compression, 'none')
        new_file_size = self.__write_header(new_tiles_compression, 'none', True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_tiles_compression, new_file_size

        return best_tiles_compression, best_file_size

    def __test_palette_compression(self, best_palette_compression, new_palette_compression, best_file_size):
        self.__execute_command('none', new_palette_compression)
        new_file_size = self.__write_header('none', new_palette_compression, True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_palette_compression, new_file_size

        return best_palette_compression, best_file_size

    def __write_header(self, tiles_compression, palette_compression, skip_write):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_bn_graphics.h'
        header_file_path = self.__build_folder_path + '/bn_sprite_items_' + name + '.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned int', 'bn::tile')
            grit_data = grit_data.replace(']', ' / (sizeof(bn::tile) / sizeof(uint32_t))]', 1)
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

        if self.__colors_count == 16:
            bpp_mode_label = 'bpp_mode::BPP_4'
        else:
            bpp_mode_label = 'bpp_mode::BPP_8'
            tiles_count *= 2

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
            header_file.write('    constexpr const sprite_item ' + name + '(' +
                              'sprite_shape_size(sprite_shape::' + self.__shape + ', ' +
                              'sprite_size::' + self.__size + '), ' + '\n            ' +
                              'sprite_tiles_item(span<const tile>(' + name + '_bn_graphicsTiles, ' +
                              str(tiles_count) + '), ' + bpp_mode_label + ', ' + compression_label(tiles_compression) +
                              ', ' + str(self.__graphics) + '), ' + '\n            ' +
                              'sprite_palette_item(span<const color>(' + name + '_bn_graphicsPal, ' +
                              str(self.__colors_count) + '), ' + bpp_mode_label + ', ' +
                              compression_label(palette_compression) + '));\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        return total_size, header_file_path

    def __execute_command(self, tiles_compression, palette_compression):
        command = ['grit', self.__file_path, '-gt', '-pe' + str(self.__colors_count)]

        if self.__colors_count == 16:
            command.append('-gB4')
        else:
            command.append('-gB8')

        if tiles_compression == 'lz77':
            command.append('-gzl')
        elif tiles_compression == 'run_length':
            command.append('-gzr')

        if palette_compression == 'lz77':
            command.append('-pzl')
        elif palette_compression == 'run_length':
            command.append('-pzr')

        command.append('-o' + self.__build_folder_path + '/' + self.__file_name_no_ext + '_bn_graphics')
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError('grit call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


class SpriteTilesItem:

    @staticmethod
    def valid_sizes_message():
        return ' (valid sprite sizes: 8x8, 16x16, 32x32, 64x64, 16x8, 32x8, 32x16, 8x16, 8x32, 16x32, 32x64)'

    def __init__(self, file_path, file_name_no_ext, build_folder_path, info):
        bmp = BMP(file_path)
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__build_folder_path = build_folder_path
        self.__colors_count = bmp.colors_count

        try:
            height = int(info['height'])
        except KeyError:
            raise ValueError('height field not found in graphics json file: ' + file_name_no_ext + '.json')

        if bmp.height % height:
            raise ValueError('File height is not divisible by item height: ' + str(bmp.height) + ' - ' + str(height))

        self.__graphics = int(bmp.height / height)
        self.__shape, self.__size = SpriteItem.shape_and_size(bmp.width, height)

        try:
            self.__compression = info['compression']
            validate_compression(self.__compression)
        except KeyError:
            self.__compression = 'none'

    def process(self):
        compression = self.__compression

        if compression == 'auto':
            compression, file_size = self.__test_compression(compression, 'none', None)
            compression, file_size = self.__test_compression(compression, 'lz77', file_size)
            compression, file_size = self.__test_compression(compression, 'run_length', file_size)

        self.__execute_command(compression)
        return self.__write_header(compression, False)

    def __test_compression(self, best_compression, new_compression, best_file_size):
        self.__execute_command(new_compression)
        new_file_size = self.__write_header(new_compression, True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_compression, new_file_size

        return best_compression, best_file_size

    def __write_header(self, compression, skip_write):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_bn_graphics.h'
        header_file_path = self.__build_folder_path + '/bn_sprite_tiles_items_' + name + '.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned int', 'bn::tile')
            grit_data = grit_data.replace(']', ' / (sizeof(bn::tile) / sizeof(uint32_t))]', 1)

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

        if self.__colors_count == 16:
            bpp_mode_label = 'bpp_mode::BPP_4'
        else:
            bpp_mode_label = 'bpp_mode::BPP_8'
            tiles_count *= 2

        with open(header_file_path, 'w') as header_file:
            include_guard = 'BN_SPRITE_TILES_ITEMS_' + name.upper() + '_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "bn_sprite_tiles_item.h"' + '\n')
            header_file.write(grit_data)
            header_file.write('\n')
            header_file.write('namespace bn::sprite_tiles_items' + '\n')
            header_file.write('{' + '\n')
            header_file.write('    constexpr const sprite_tiles_item ' + name + '(span<const tile>(' +
                              name + '_bn_graphicsTiles, ' + str(tiles_count) + '), ' + '\n            ' +
                              bpp_mode_label + ', ' + compression_label(compression) + ', ' +
                              str(self.__graphics) + ');\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        return total_size, header_file_path

    def __execute_command(self, compression):
        command = ['grit', self.__file_path, '-gt', '-p!']

        if self.__colors_count == 16:
            command.append('-gB4')
        else:
            command.append('-gB8')

        if compression == 'lz77':
            command.append('-gzl')
        elif compression == 'run_length':
            command.append('-gzr')

        command.append('-o' + self.__build_folder_path + '/' + self.__file_name_no_ext + '_bn_graphics')
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError('grit call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


class SpritePaletteItem:

    def __init__(self, file_path, file_name_no_ext, build_folder_path, info):
        bmp = BMP(file_path)
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__build_folder_path = build_folder_path
        self.__colors_count = bmp.colors_count

        try:
            self.__compression = info['compression']
            validate_compression(self.__compression)
        except KeyError:
            self.__compression = 'none'

    def process(self):
        compression = self.__compression

        if compression == 'auto':
            compression, file_size = self.__test_compression(compression, 'none', None)
            compression, file_size = self.__test_compression(compression, 'lz77', file_size)
            compression, file_size = self.__test_compression(compression, 'run_length', file_size)

        self.__execute_command(compression)
        return self.__write_header(compression, False)

    def __test_compression(self, best_compression, new_compression, best_file_size):
        self.__execute_command(new_compression)
        new_file_size = self.__write_header(new_compression, True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_compression, new_file_size

        return best_compression, best_file_size

    def __write_header(self, compression, skip_write):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_bn_graphics.h'
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

        if self.__colors_count == 16:
            bpp_mode_label = 'bpp_mode::BPP_4'
        else:
            bpp_mode_label = 'bpp_mode::BPP_8'

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
            header_file.write('    constexpr const sprite_palette_item ' + name + '(' +
                              'span<const color>(' + name + '_bn_graphicsPal, ' +
                              str(self.__colors_count) + '), ' + '\n            ' +
                              bpp_mode_label + ', ' + compression_label(compression) + ');' + '\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        return total_size, header_file_path

    def __execute_command(self, compression):
        command = ['grit', self.__file_path, '-g!', '-pe' + str(self.__colors_count)]

        if compression == 'lz77':
            command.append('-pzl')
        elif compression == 'run_length':
            command.append('-pzr')

        command.append('-o' + self.__build_folder_path + '/' + self.__file_name_no_ext + '_bn_graphics')
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError('grit call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


class RegularBgItem:

    def __init__(self, file_path, file_name_no_ext, build_folder_path, info):
        bmp = BMP(file_path)
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__build_folder_path = build_folder_path
        self.__colors_count = bmp.colors_count

        width = bmp.width
        height = bmp.height

        if width % 256 != 0:
            raise ValueError('Regular BGs width must be divisible by 256: ' + str(width))

        if height % 256 != 0:
            raise ValueError('Regular BGs height must be divisible by 256: ' + str(height))

        self.__width = int(width / 8)
        self.__height = int(height / 8)
        self.__bpp_8 = False
        self.__sbb = (width == 256 and height == 512) or (width == 512 and height == 256) or \
                     (width == 512 and height == 512)

        try:
            self.__repeated_tiles_reduction = bool(info['repeated_tiles_reduction'])
        except KeyError:
            self.__repeated_tiles_reduction = True

        try:
            self.__flipped_tiles_reduction = bool(info['flipped_tiles_reduction'])
        except KeyError:
            self.__flipped_tiles_reduction = True

        if self.__colors_count > 16:
            try:
                bpp_mode = str(info['bpp_mode'])
            except KeyError:
                bpp_mode = 'bpp_8'

            if bpp_mode == 'bpp_8':
                self.__bpp_8 = True
            elif bpp_mode == 'bpp_4_auto':
                self.__file_path = self.__build_folder_path + '/' + file_name_no_ext + '.bn_quantized.bmp'
                self.__colors_count = bmp.quantize(self.__file_path)
            elif bpp_mode != 'bpp_4_manual':
                raise ValueError('Invalid BPP mode: ' + bpp_mode)

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

        try:
            self.__map_compression = info['map_compression']
            validate_compression(self.__map_compression)
        except KeyError:
            try:
                self.__map_compression = info['compression']
                validate_compression(self.__map_compression)
            except KeyError:
                self.__map_compression = 'none'

    def process(self):
        tiles_compression = self.__tiles_compression
        palette_compression = self.__palette_compression
        map_compression = self.__map_compression

        if tiles_compression == 'auto':
            tiles_compression, file_size = self.__test_tiles_compression(tiles_compression, 'none', None)
            tiles_compression, file_size = self.__test_tiles_compression(tiles_compression, 'lz77', file_size)
            tiles_compression, file_size = self.__test_tiles_compression(tiles_compression, 'run_length', file_size)

        if palette_compression == 'auto':
            palette_compression, file_size = self.__test_palette_compression(palette_compression, 'none', None)
            palette_compression, file_size = self.__test_palette_compression(palette_compression, 'lz77', file_size)
            palette_compression, file_size = self.__test_palette_compression(palette_compression, 'run_length',
                                                                             file_size)

        if map_compression == 'auto':
            map_compression, file_size = self.__test_map_compression(map_compression, 'none', None)
            map_compression, file_size = self.__test_map_compression(map_compression, 'lz77', file_size)
            map_compression, file_size = self.__test_map_compression(map_compression, 'run_length', file_size)

        self.__execute_command(tiles_compression, palette_compression, map_compression)
        return self.__write_header(tiles_compression, palette_compression, map_compression, False)

    def __test_tiles_compression(self, best_tiles_compression, new_tiles_compression, best_file_size):
        self.__execute_command(new_tiles_compression, 'none', 'none')
        new_file_size = self.__write_header(new_tiles_compression, 'none', 'none', True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_tiles_compression, new_file_size

        return best_tiles_compression, best_file_size

    def __test_palette_compression(self, best_palette_compression, new_palette_compression, best_file_size):
        self.__execute_command('none', new_palette_compression, 'none')
        new_file_size = self.__write_header('none', new_palette_compression, 'none', True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_palette_compression, new_file_size

        return best_palette_compression, best_file_size

    def __test_map_compression(self, best_map_compression, new_map_compression, best_file_size):
        self.__execute_command('none', 'none', new_map_compression)
        new_file_size = self.__write_header('none', 'none', new_map_compression, True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_map_compression, new_file_size

        return best_map_compression, best_file_size

    def __write_header(self, tiles_compression, palette_compression, map_compression, skip_write):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_bn_graphics.h'
        header_file_path = self.__build_folder_path + '/bn_regular_bg_items_' + name + '.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned int', 'bn::tile', 1)
            grit_data = grit_data.replace(']', ' / (sizeof(bn::tile) / sizeof(uint32_t))]', 1)
            grit_data = grit_data.replace('unsigned short', 'bn::regular_bg_map_cell', 1)
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

        with open(header_file_path, 'w') as header_file:
            include_guard = 'BN_REGULAR_BG_ITEMS_' + name.upper() + '_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "bn_regular_bg_item.h"' + '\n')
            header_file.write(grit_data)
            header_file.write('\n')
            header_file.write('namespace bn::regular_bg_items' + '\n')
            header_file.write('{' + '\n')
            header_file.write('    constexpr const regular_bg_item ' + name + '(' + '\n            ' +
                              'regular_bg_tiles_item(span<const tile>(' + name + '_bn_graphicsTiles, ' +
                              str(tiles_count) + '), ' + bpp_mode_label + ', ' + compression_label(tiles_compression) +
                              '), ' + '\n            ' +
                              'bg_palette_item(span<const color>(' + name + '_bn_graphicsPal, ' +
                              str(self.__colors_count) + '), ' + bpp_mode_label + ', ' +
                              compression_label(palette_compression) + '),' + '\n            ' +
                              'regular_bg_map_item(' + name + '_bn_graphicsMap[0], ' +
                              'size(' + str(self.__width) + ', ' + str(self.__height) + '), ' +
                              compression_label(map_compression) + '));' + '\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        return total_size, header_file_path

    def __execute_command(self, tiles_compression, palette_compression, map_compression):
        command = ['grit', self.__file_path, '-pe' + str(self.__colors_count)]

        if self.__bpp_8:
            command.append('-gB8')

            if self.__repeated_tiles_reduction and self.__flipped_tiles_reduction:
                command.append('-mRtf')
            elif self.__repeated_tiles_reduction:
                command.append('-mRt')
            elif self.__flipped_tiles_reduction:
                command.append('-mRf')
            else:
                command.append('-mR!')
        else:
            command.append('-gB4')

            if self.__repeated_tiles_reduction and self.__flipped_tiles_reduction:
                command.append('-mRtpf')
            elif self.__repeated_tiles_reduction:
                command.append('-mRtp')
            elif self.__flipped_tiles_reduction:
                command.append('-mRpf')
            else:
                command.append('-mRp')

        if self.__sbb:
            command.append('-mLs')
        else:
            command.append('-mLf')

        if tiles_compression == 'lz77':
            command.append('-gzl')
        elif tiles_compression == 'run_length':
            command.append('-gzr')

        if palette_compression == 'lz77':
            command.append('-pzl')
        elif palette_compression == 'run_length':
            command.append('-pzr')

        if map_compression == 'lz77':
            command.append('-mzl')
        elif map_compression == 'run_length':
            command.append('-mzr')

        command.append('-o' + self.__build_folder_path + '/' + self.__file_name_no_ext + '_bn_graphics')
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError('grit call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


class AffineBgItem:

    def __init__(self, file_path, file_name_no_ext, build_folder_path, info):
        bmp = BMP(file_path)
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__build_folder_path = build_folder_path
        self.__colors_count = bmp.colors_count

        width = bmp.width
        height = bmp.height

        if width != 128 and width % 256 != 0:
            raise ValueError('Affine BGs width must be 128 or divisible by 256: ' + str(width))

        if height != 128 and height % 256 != 0:
            raise ValueError('Affine BGs height must be 128 or divisible by 256: ' + str(height))

        self.__width = int(width / 8)
        self.__height = int(height / 8)

        try:
            self.__repeated_tiles_reduction = bool(info['repeated_tiles_reduction'])
        except KeyError:
            self.__repeated_tiles_reduction = True

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

        try:
            self.__map_compression = info['map_compression']
            validate_compression(self.__map_compression)
        except KeyError:
            try:
                self.__map_compression = info['compression']
                validate_compression(self.__map_compression)
            except KeyError:
                self.__map_compression = 'none'

    def process(self):
        tiles_compression = self.__tiles_compression
        palette_compression = self.__palette_compression
        map_compression = self.__map_compression

        if tiles_compression == 'auto':
            tiles_compression, file_size = self.__test_tiles_compression(tiles_compression, 'none', None)
            tiles_compression, file_size = self.__test_tiles_compression(tiles_compression, 'lz77', file_size)
            tiles_compression, file_size = self.__test_tiles_compression(tiles_compression, 'run_length', file_size)

        if palette_compression == 'auto':
            palette_compression, file_size = self.__test_palette_compression(palette_compression, 'none', None)
            palette_compression, file_size = self.__test_palette_compression(palette_compression, 'lz77', file_size)
            palette_compression, file_size = self.__test_palette_compression(palette_compression, 'run_length',
                                                                             file_size)

        if map_compression == 'auto':
            map_compression, file_size = self.__test_map_compression(map_compression, 'none', None)
            map_compression, file_size = self.__test_map_compression(map_compression, 'lz77', file_size)
            map_compression, file_size = self.__test_map_compression(map_compression, 'run_length', file_size)

        self.__execute_command(tiles_compression, palette_compression, map_compression)
        return self.__write_header(tiles_compression, palette_compression, map_compression, False)

    def __test_tiles_compression(self, best_tiles_compression, new_tiles_compression, best_file_size):
        self.__execute_command(new_tiles_compression, 'none', 'none')
        new_file_size = self.__write_header(new_tiles_compression, 'none', 'none', True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_tiles_compression, new_file_size

        return best_tiles_compression, best_file_size

    def __test_palette_compression(self, best_palette_compression, new_palette_compression, best_file_size):
        self.__execute_command('none', new_palette_compression, 'none')
        new_file_size = self.__write_header('none', new_palette_compression, 'none', True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_palette_compression, new_file_size

        return best_palette_compression, best_file_size

    def __test_map_compression(self, best_map_compression, new_map_compression, best_file_size):
        self.__execute_command('none', 'none', new_map_compression)
        new_file_size = self.__write_header('none', 'none', new_map_compression, True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_map_compression, new_file_size

        return best_map_compression, best_file_size

    def __write_header(self, tiles_compression, palette_compression, map_compression, skip_write):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_bn_graphics.h'
        header_file_path = self.__build_folder_path + '/bn_affine_bg_items_' + name + '.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned int', 'bn::tile', 1)
            grit_data = grit_data.replace(']', ' / (sizeof(bn::tile) / sizeof(uint32_t))]', 1)
            grit_data = grit_data.replace('unsigned char', 'bn::affine_bg_map_cell', 1)
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

        with open(header_file_path, 'w') as header_file:
            include_guard = 'BN_AFFINE_BG_ITEMS_' + name.upper() + '_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "bn_affine_bg_item.h"' + '\n')
            header_file.write(grit_data)
            header_file.write('\n')
            header_file.write('namespace bn::affine_bg_items' + '\n')
            header_file.write('{' + '\n')
            header_file.write('    constexpr const affine_bg_item ' + name + '(' + '\n            ' +
                              'affine_bg_tiles_item(span<const tile>(' + name + '_bn_graphicsTiles, ' +
                              str(tiles_count * 2) + '), ' + compression_label(tiles_compression) +
                              '), ' + '\n            ' +
                              'bg_palette_item(span<const color>(' + name + '_bn_graphicsPal, ' +
                              str(self.__colors_count) + '), bpp_mode::BPP_8, ' +
                              compression_label(palette_compression) + '),' + '\n            ' +
                              'affine_bg_map_item(' + name + '_bn_graphicsMap[0], ' +
                              'size(' + str(self.__width) + ', ' + str(self.__height) + '), ' +
                              compression_label(map_compression) + '));' + '\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        return total_size, header_file_path

    def __execute_command(self, tiles_compression, palette_compression, map_compression):
        command = ['grit', self.__file_path, '-gB8', '-mLa', '-mu8', '-pe' + str(self.__colors_count)]

        if self.__repeated_tiles_reduction:
            command.append('-mRt')
        else:
            command.append('-mR!')

        if tiles_compression == 'lz77':
            command.append('-gzl')
        elif tiles_compression == 'run_length':
            command.append('-gzr')

        if palette_compression == 'lz77':
            command.append('-pzl')
        elif palette_compression == 'run_length':
            command.append('-pzr')

        if map_compression == 'lz77':
            command.append('-mzl')
        elif map_compression == 'run_length':
            command.append('-mzr')

        command.append('-o' + self.__build_folder_path + '/' + self.__file_name_no_ext + '_bn_graphics')
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError('grit call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


class BgPaletteItem:

    def __init__(self, file_path, file_name_no_ext, build_folder_path, info):
        bmp = BMP(file_path)
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__build_folder_path = build_folder_path
        self.__colors_count = bmp.colors_count

        try:
            bpp_mode = str(info['bpp_mode'])
        except KeyError:
            raise ValueError('bpp_mode field not found in graphics json file: ' + file_name_no_ext + '.json')

        if bpp_mode == 'bpp_8':
            self.__bpp_8 = True
        elif bpp_mode == 'bpp_4':
            self.__bpp_8 = False
        else:
            raise ValueError('Invalid BPP mode: ' + bpp_mode)

        try:
            self.__compression = info['compression']
            validate_compression(self.__compression)
        except KeyError:
            self.__compression = 'none'

    def process(self):
        compression = self.__compression

        if compression == 'auto':
            compression, file_size = self.__test_compression(compression, 'none', None)
            compression, file_size = self.__test_compression(compression, 'lz77', file_size)
            compression, file_size = self.__test_compression(compression, 'run_length', file_size)

        self.__execute_command(compression)
        return self.__write_header(compression, False)

    def __test_compression(self, best_compression, new_compression, best_file_size):
        self.__execute_command(new_compression)
        new_file_size = self.__write_header(new_compression, True)

        if best_file_size is None or new_file_size < best_file_size:
            return new_compression, new_file_size

        return best_compression, best_file_size

    def __write_header(self, compression, skip_write):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_bn_graphics.h'
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
            header_file.write('    constexpr const bg_palette_item ' + name + '(' +
                              'span<const color>(' + name + '_bn_graphicsPal, ' +
                              str(self.__colors_count) + '), ' + '\n            ' +
                              bpp_mode_label + ', ' + compression_label(compression) + ');' + '\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        return total_size, header_file_path

    def __execute_command(self, compression):
        command = ['grit', self.__file_path, '-g!', '-pe' + str(self.__colors_count)]

        if compression == 'lz77':
            command.append('-pzl')
        elif compression == 'run_length':
            command.append('-pzr')

        command.append('-o' + self.__build_folder_path + '/' + self.__file_name_no_ext + '_bn_graphics')
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError('grit call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


class GraphicsFileInfo:

    def __init__(self, json_file_path, file_path, file_name, file_name_no_ext, new_file_info,
                 file_info_path, new_json_file_info, json_file_info_path):
        self.__json_file_path = json_file_path
        self.__file_path = file_path
        self.__file_name = file_name
        self.__file_name_no_ext = file_name_no_ext
        self.__new_file_info = new_file_info
        self.__file_info_path = file_info_path
        self.__new_json_file_info = new_json_file_info
        self.__json_file_info_path = json_file_info_path

    def print_file_name(self):
        print(self.__file_name)

    def process(self, build_folder_path):
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
            elif graphics_type == 'affine_bg':
                item = AffineBgItem(self.__file_path, self.__file_name_no_ext, build_folder_path, info)
            elif graphics_type == 'bg_palette':
                item = BgPaletteItem(self.__file_path, self.__file_name_no_ext, build_folder_path, info)
            else:
                raise ValueError('Unknown graphics type "' + graphics_type +
                                 '" found in graphics json file: ' + self.__json_file_path)

            total_size, header_file_path = item.process()
            self.__new_file_info.write(self.__file_info_path)
            self.__new_json_file_info.write(self.__json_file_info_path)
            return [self.__file_name, header_file_path, total_size]
        except Exception as exc:
            return [self.__file_name, exc]


class GraphicsFileInfoProcessor:

    def __init__(self, build_folder_path):
        self.__build_folder_path = build_folder_path

    def __call__(self, graphics_file_info):
        return graphics_file_info.process(self.__build_folder_path)


def list_graphics_file_infos(graphics_folder_paths, build_folder_path):
    graphics_folder_path_list = graphics_folder_paths.split(' ')
    graphics_file_infos = []
    file_names_set = set()

    for graphics_folder_path in graphics_folder_path_list:
        graphics_file_names = sorted(os.listdir(graphics_folder_path))

        for graphics_file_name in graphics_file_names:
            graphics_file_path = graphics_folder_path + '/' + graphics_file_name

            if os.path.isfile(graphics_file_path) and FileInfo.validate(graphics_file_name):
                graphics_file_name_split = os.path.splitext(graphics_file_name)
                graphics_file_name_no_ext = graphics_file_name_split[0]
                graphics_file_name_ext = graphics_file_name_split[1]

                if graphics_file_name_ext == '.bmp':
                    if graphics_file_name_no_ext in file_names_set:
                        raise ValueError('There\'s two or more graphics files with the same name: ' +
                                         graphics_file_name_no_ext)

                    file_names_set.add(graphics_file_name_no_ext)
                    json_file_path = graphics_folder_path + '/' + graphics_file_name_no_ext + '.json'

                    if not os.path.isfile(json_file_path):
                        raise ValueError('Graphics json file not found: ' + json_file_path)

                    file_info_path_prefix = build_folder_path + '/_bn_' + graphics_file_name_no_ext + '_'
                    file_info_path = file_info_path_prefix + '_file_info.txt'
                    json_file_info_path = file_info_path_prefix + '_json_file_info.txt'
                    old_file_info = FileInfo.read(file_info_path)
                    new_file_info = FileInfo.build_from_file(graphics_file_path)
                    old_json_file_info = FileInfo.read(json_file_info_path)
                    new_json_file_info = FileInfo.build_from_file(json_file_path)

                    if old_file_info != new_file_info or old_json_file_info != new_json_file_info:
                        graphics_file_infos.append(GraphicsFileInfo(
                            json_file_path, graphics_file_path, graphics_file_name, graphics_file_name_no_ext,
                            new_file_info, file_info_path, new_json_file_info, json_file_info_path))

    return graphics_file_infos


def process(graphics_folder_paths, build_folder_path):
    graphics_file_infos = list_graphics_file_infos(graphics_folder_paths, build_folder_path)

    if len(graphics_file_infos) > 0:
        for graphics_file_info in graphics_file_infos:
            graphics_file_info.print_file_name()

        sys.stdout.flush()

        pool = Pool()
        process_results = pool.map(GraphicsFileInfoProcessor(build_folder_path), graphics_file_infos)
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


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='butano graphics tool.')
    parser.add_argument('--graphics', required=True, help='graphics folder paths')
    parser.add_argument('--build', required=True, help='build folder path')

    try:
        args = parser.parse_args()
        process(args.graphics, args.build)
    except Exception as ex:
        sys.stderr.write('Error: ' + str(ex) + '\n')
        traceback.print_exc()
        exit(-1)
