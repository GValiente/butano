"""
Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
zlib License, see LICENSE file.
"""

import os
import json
import argparse
import subprocess
import time

from bmp import BMP
from file_info import FileInfo


def remove_file(file_path):
    if os.path.exists(file_path):
        os.remove(file_path)


class SpriteItem:

    @staticmethod
    def valid_sizes_message():
        return ' (valid sprite sizes: 8x8, 16x16, 32x32, 64x64, 16x8, 32x8, 32x16, 8x16, 8x32, 16x32, 32x64)'

    def __init__(self, file_path, file_name_no_ext, build_folder_path, info):
        bmp = BMP(file_path)
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__build_folder_path = build_folder_path
        self.__colors_count = bmp.colors_count
        self.__bpp8 = self.__colors_count > 16

        try:
            height = int(info['height'])

            if bmp.height % height:
                raise ValueError('File height is not divisible by item height: ' +
                                 str(bmp.height) + ' - ' + str(height))
        except KeyError:
            height = bmp.height

        self.__graphics = int(bmp.height / height)
        width = bmp.width

        if width == 8:
            if height == 8:
                self.__shape = 'SQUARE'
                self.__size = 'SMALL'
            elif height == 16:
                self.__shape = 'TALL'
                self.__size = 'SMALL'
            elif height == 32:
                self.__shape = 'TALL'
                self.__size = 'NORMAL'
            elif height == 64:
                raise ValueError('Invalid sprite size: (' + str(width) + 'x' + str(height) + ')' +
                                 SpriteItem.valid_sizes_message())
            else:
                raise ValueError('Invalid sprite height: ' + str(height) + SpriteItem.valid_sizes_message())
        elif width == 16:
            if height == 8:
                self.__shape = 'WIDE'
                self.__size = 'SMALL'
            elif height == 16:
                self.__shape = 'SQUARE'
                self.__size = 'NORMAL'
            elif height == 32:
                self.__shape = 'TALL'
                self.__size = 'BIG'
            elif height == 64:
                raise ValueError('Invalid sprite size: (: ' + str(width) + 'x' + str(height) + ')' +
                                 SpriteItem.valid_sizes_message())
            else:
                raise ValueError('Invalid sprite height: ' + str(height) + SpriteItem.valid_sizes_message())
        elif width == 32:
            if height == 8:
                self.__shape = 'WIDE'
                self.__size = 'NORMAL'
            elif height == 16:
                self.__shape = 'WIDE'
                self.__size = 'BIG'
            elif height == 32:
                self.__shape = 'SQUARE'
                self.__size = 'BIG'
            elif height == 64:
                self.__shape = 'TALL'
                self.__size = 'HUGE'
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
                self.__shape = 'WIDE'
                self.__size = 'HUGE'
            elif height == 64:
                self.__shape = 'SQUARE'
                self.__size = 'HUGE'
            else:
                raise ValueError('Invalid sprite height: ' + str(height) + SpriteItem.valid_sizes_message())
        else:
            raise ValueError('Invalid sprite width: ' + str(width) + SpriteItem.valid_sizes_message())

    def write_header(self):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_btn_graphics.h'
        header_file_path = self.__build_folder_path + '/btn_sprite_items_' + name + '.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned int', 'btn::tile')
            grit_data = grit_data.replace(']', ' / (sizeof(btn::tile) / sizeof(uint32_t))]', 1)
            grit_data = grit_data.replace('unsigned short', 'btn::color')

        remove_file(grit_file_path)

        if self.__bpp8:
            bpp_mode_label = 'palette_bpp_mode::BPP_8'
        else:
            bpp_mode_label = 'palette_bpp_mode::BPP_4'

        with open(header_file_path, 'w') as header_file:
            include_guard = 'BTN_SPRITE_ITEMS_' + name.upper() + '_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "btn_sprite_item.h"' + '\n')
            header_file.write(grit_data)
            header_file.write('\n')
            header_file.write('namespace btn::sprite_items' + '\n')
            header_file.write('{' + '\n')
            header_file.write('    constexpr const sprite_item ' + name + '(' +
                              'sprite_shape_size(sprite_shape::' + self.__shape + ', ' +
                              'sprite_size::' + self.__size + '), ' + '\n            ' +
                              'span<const tile>(' + name + '_btn_graphicsTiles), ' + '\n            ' +
                              'span<const color>(' + name + '_btn_graphicsPal, ' + str(self.__colors_count) + '), ' +
                              bpp_mode_label + ', ' + str(self.__graphics) + ');' + '\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        print('sprite_item file written in ' + header_file_path)

    def process(self):
        command = ['grit', self.__file_path, '-gt']

        if self.__colors_count == 16:
            command.append('-gB4')
        else:
            command.append('-gB8')

        command.append('-o' + self.__build_folder_path + '/' + self.__file_name_no_ext + '_btn_graphics')
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError('grit call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


class RegularBgItem:

    @staticmethod
    def valid_sizes_message():
        return ' (valid regular BG sizes: 256x256, 512x256, 256x512, 512x512)'

    def __init__(self, file_path, file_name_no_ext, build_folder_path, info):
        bmp = BMP(file_path)
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__build_folder_path = build_folder_path
        self.__colors_count = bmp.colors_count

        width = bmp.width
        height = bmp.height

        if width == 256 and height == 256:
            self.__sbb = False
        elif (width == 256 and height == 512) or (width == 512 and height == 256) or (width == 512 and height == 512):
            self.__sbb = True
        else:
            raise ValueError('Invalid regular BG size: (' + str(width) + 'x' + str(height) + ')' +
                             RegularBgItem.valid_sizes_message())

        self.__width = int(width / 8)
        self.__height = int(height / 8)
        self.__bpp8 = False

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
                bpp_mode = 'bpp8'

            if bpp_mode == 'bpp8':
                self.__bpp8 = True
            elif bpp_mode == 'bpp4_auto':
                self.__file_path = self.__build_folder_path + '/' + file_name_no_ext + '.btn_quantized.bmp'
                print('Generating 4bpp image in ' + self.__file_path + '...')
                start = time.time()
                self.__colors_count = bmp.quantize(self.__file_path)
                end = time.time()
                print('4bpp image with ' + str(self.__colors_count) + ' colors generated in ' +
                      str(int((end - start) * 1000)) + ' milliseconds')
            elif bpp_mode != 'bpp4_manual':
                raise ValueError('Invalid BPP mode: ' + bpp_mode)

    def write_header(self):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_btn_graphics.h'
        header_file_path = self.__build_folder_path + '/btn_regular_bg_items_' + name + '.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned int', 'btn::tile', 1)
            grit_data = grit_data.replace(']', ' / (sizeof(btn::tile) / sizeof(uint32_t))]', 1)
            grit_data = grit_data.replace('unsigned short', 'btn::regular_bg_map_cell', 1)
            grit_data = grit_data.replace('unsigned short', 'btn::color', 1)

        remove_file(grit_file_path)

        if self.__bpp8:
            bpp_mode_label = 'palette_bpp_mode::BPP_8'
        else:
            bpp_mode_label = 'palette_bpp_mode::BPP_4'

        with open(header_file_path, 'w') as header_file:
            include_guard = 'BTN_REGULAR_BG_ITEMS_' + name.upper() + '_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "btn_regular_bg_item.h"' + '\n')
            header_file.write(grit_data)
            header_file.write('\n')
            header_file.write('namespace btn::regular_bg_items' + '\n')
            header_file.write('{' + '\n')
            header_file.write('    constexpr const regular_bg_item ' + name + '(' +
                              'span<const tile>(' + name + '_btn_graphicsTiles), ' + '\n            ' +
                              name + '_btn_graphicsMap[0], size(' + str(self.__width) + ', ' + str(self.__height) + '),'
                              '\n            ' +
                              'span<const color>(' + name + '_btn_graphicsPal, ' + str(self.__colors_count) + '), ' +
                              bpp_mode_label + ');' + '\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        print('regular_bg_item file written in ' + header_file_path)

    def process(self):
        command = ['grit', self.__file_path]

        if self.__bpp8:
            command.append('-gB8')

            if self.__repeated_tiles_reduction:
                command.append('-mRt')

            if self.__flipped_tiles_reduction:
                command.append('-mRf')
        else:
            command.append('-gB4')

            if self.__repeated_tiles_reduction:
                command.append('-mRtp')

            if self.__flipped_tiles_reduction:
                command.append('-mRf')

        if self.__sbb:
            command.append('-mLs')
        else:
            command.append('-mLf')

        command.append('-o' + self.__build_folder_path + '/' + self.__file_name_no_ext + '_btn_graphics')
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError('grit call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


class GraphicsFileInfo:

    def __init__(self, graphics_type, info, file_path, file_name_no_ext, new_file_info,
                 file_info_path, new_json_file_info, json_file_info_path):
        self.__graphics_type = graphics_type
        self.__info = info
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__new_file_info = new_file_info
        self.__file_info_path = file_info_path
        self.__new_json_file_info = new_json_file_info
        self.__json_file_info_path = json_file_info_path

    def process(self, build_folder_path):
        print('Processing graphics file: ' + self.__file_path)

        if self.__graphics_type == 'sprite':
            item = SpriteItem(self.__file_path, self.__file_name_no_ext, build_folder_path, self.__info)
        else:
            item = RegularBgItem(self.__file_path, self.__file_name_no_ext, build_folder_path, self.__info)

        item.process()
        item.write_header()
        self.__new_file_info.write(self.__file_info_path)
        self.__new_json_file_info.write(self.__json_file_info_path)


def list_graphics_file_infos(graphics_folder_paths, build_folder_path):
    graphics_folder_path_list = graphics_folder_paths.split(' ')
    graphics_file_infos = []
    sprite_file_names_set = set()
    regular_bg_file_names_set = set()

    for graphics_folder_path in graphics_folder_path_list:
        graphics_file_names = sorted(os.listdir(graphics_folder_path))

        for graphics_file_name in graphics_file_names:
            graphics_file_path = graphics_folder_path + '/' + graphics_file_name

            if FileInfo.validate(graphics_file_name):
                graphics_file_name_split = os.path.splitext(graphics_file_name)
                graphics_file_name_no_ext = graphics_file_name_split[0]
                graphics_file_name_ext = graphics_file_name_split[1]

                if graphics_file_name_ext == '.bmp':
                    json_file_path = graphics_folder_path + '/' + graphics_file_name_no_ext + '.json'

                    if not os.path.isfile(json_file_path):
                        raise ValueError('Graphics json file not found: ' + json_file_path)

                    try:
                        with open(json_file_path) as json_file:
                            info = json.load(json_file)
                    except Exception as ex:
                        raise ValueError(json_file_path + ' graphics json file parse failed: ' + str(ex))

                    try:
                        graphics_type = str(info['type'])
                    except KeyError:
                        raise ValueError('type filed not found in graphics json file: ' + json_file_path)

                    if graphics_type == 'sprite':
                        file_names_set = sprite_file_names_set
                    elif graphics_type == 'regular_bg':
                        file_names_set = regular_bg_file_names_set
                    else:
                        raise ValueError('Unknown type (' + graphics_type + ') in graphics json file: ' +
                                         json_file_path)

                    if graphics_file_name_no_ext in file_names_set:
                        raise ValueError('There\'s two or more ' + graphics_type +
                                         ' graphics files with the same name: ' + graphics_file_name_no_ext)

                    file_names_set.add(graphics_file_name_no_ext)

                    file_info_path_prefix = build_folder_path + '/_btn_' + graphics_file_name_no_ext + '_'
                    file_info_path = file_info_path_prefix + graphics_type + '_file_info.txt'
                    json_file_info_path = file_info_path_prefix + graphics_type + '_json_file_info.txt'
                    old_file_info = FileInfo.read(file_info_path)
                    new_file_info = FileInfo.build_from_file(graphics_file_path)
                    old_json_file_info = FileInfo.read(json_file_info_path)
                    new_json_file_info = FileInfo.build_from_file(json_file_path)

                    if old_file_info != new_file_info or old_json_file_info != new_json_file_info:
                        graphics_file_infos.append(GraphicsFileInfo(
                            graphics_type, info, graphics_file_path, graphics_file_name_no_ext, new_file_info,
                            file_info_path, new_json_file_info, json_file_info_path))
            else:
                print('Graphics file skipped: ' + graphics_file_path)

    return graphics_file_infos


def process(graphics_folder_paths, build_folder_path):
    graphics_file_infos = list_graphics_file_infos(graphics_folder_paths, build_folder_path)

    for graphics_file_info in graphics_file_infos:
        graphics_file_info.process(build_folder_path)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='butano graphics tool.')
    parser.add_argument('--graphics', required=True, help='graphics folder paths')
    parser.add_argument('--build', required=True, help='build folder path')
    args = parser.parse_args()
    process(args.graphics, args.build)
