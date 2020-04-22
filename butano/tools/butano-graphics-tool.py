"""
Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
MIT License, see LICENSE file.
"""

import os
import json
import argparse
import subprocess
import time
import pickle

from bmp import BMP
from file_info import FileInfo


def remove_file(file_path):
    if os.path.exists(file_path):
        os.remove(file_path)


class GraphicsFolderInfo:

    def __init__(self, sprites, bgs, file_paths):
        self.__sprites = sprites
        self.__bgs = bgs
        self.__file_paths = file_paths
        self.__new_graphics_json = False

    def get_sprite(self, file_name_no_ext):
        return self.__sprites[file_name_no_ext]

    def get_bg(self, file_name_no_ext):
        return self.__bgs[file_name_no_ext]

    def file_paths(self):
        return self.__file_paths

    def new_graphics_json(self):
        return self.__new_graphics_json

    def set_new_graphics_json(self, new_graphics_json):
        self.__new_graphics_json = new_graphics_json


class SpriteItem:

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
                raise ValueError('Invalid sprite size: (' + str(width) + ' - ' + str(height) + ')')
            else:
                raise ValueError('Invalid sprite height: ' + str(height))
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
                raise ValueError('Invalid sprite size: (: ' + str(width) + ' - ' + str(height) + ')')
            else:
                raise ValueError('Invalid sprite height: ' + str(height))
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
                raise ValueError('Invalid sprite height: ' + str(height))
        elif width == 64:
            if height == 8:
                raise ValueError('Invalid sprite size: (' + str(width) + ' - ' + str(height) + ')')
            elif height == 16:
                raise ValueError('Invalid sprite size: (' + str(width) + ' - ' + str(height) + ')')
            elif height == 32:
                self.__shape = 'WIDE'
                self.__size = 'HUGE'
            elif height == 64:
                self.__shape = 'SQUARE'
                self.__size = 'HUGE'
            else:
                raise ValueError('Invalid sprite height: ' + str(height))
        else:
            raise ValueError('Invalid sprite width: ' + str(width))

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


class BgItem:

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
            raise ValueError('Invalid BG size: (' + str(width) + ' - ' + str(height) + ')')

        self.__width = int(width / 8)
        self.__height = int(height / 8)
        self.__bpp8 = False

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
                print('4bpp image generated successfully in ' + str(int((end - start) * 1000)) + ' milliseconds')
            elif bpp_mode != 'bpp4_manual':
                raise ValueError('Invalid BPP mode: ' + bpp_mode)

    def write_header(self):
        name = self.__file_name_no_ext
        grit_file_path = self.__build_folder_path + '/' + name + '_btn_graphics.h'
        header_file_path = self.__build_folder_path + '/btn_bg_items_' + name + '.h'

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
            include_guard = 'BTN_BG_ITEMS_' + name.upper() + '_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "btn_regular_bg_item.h"' + '\n')
            header_file.write(grit_data)
            header_file.write('\n')
            header_file.write('namespace btn::bg_items' + '\n')
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

        print('bg_item file written in ' + header_file_path)

    def process(self):
        command = ['grit', self.__file_path]

        if self.__bpp8:
            command.append('-gB8 -mR8')
        else:
            command.append('-gB4 -mR4')

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


def list_graphics_folder_infos(graphics_folder_paths, build_folder_path):
    graphics_folder_path_list = graphics_folder_paths.split(' ')
    graphics_folder_infos = []
    graphics_json_file_paths = []

    for graphics_folder_path in graphics_folder_path_list:
        graphics_json_file_path = graphics_folder_path + '/graphics.json'
        graphics_json_file_paths.append(graphics_json_file_path)

        try:
            with open(graphics_json_file_path) as file:
                data = json.load(file)
                sprites = data['sprites']
                bgs = data['bgs']
        except Exception as ex:
            raise ValueError(graphics_json_file_path + ' parse failed: ' + str(ex))

        graphics_file_names = sorted(os.listdir(graphics_folder_path))
        graphics_file_paths = []

        for graphics_file_name in graphics_file_names:
            if FileInfo.validate(graphics_file_name):
                graphics_file_name_ext = os.path.splitext(graphics_file_name)[-1]

                if graphics_file_name_ext.lower() == '.bmp':
                    graphics_file_path = graphics_folder_path + '/' + graphics_file_name

                    if os.path.isfile(graphics_file_path):
                        graphics_file_paths.append(graphics_file_path)
            else:
                print('Graphics file skipped: ' + graphics_file_name)

        graphics_folder_infos.append(GraphicsFolderInfo(sprites, bgs, graphics_file_paths))

    file_info_path = build_folder_path + '/_btn_graphics_json_file_info.txt'
    old_file_info = FileInfo.read(file_info_path)
    new_file_info = FileInfo.build_from_files(graphics_json_file_paths)

    if old_file_info != new_file_info:
        new_file_info.write(file_info_path)

        for graphics_folder_info in graphics_folder_infos:
            graphics_folder_info.set_new_graphics_json(True)

    return graphics_folder_infos


def remove_old_graphics_items(new_graphics_file_names_set, build_folder_path):
    graphics_file_names_set_file_path = build_folder_path + '/_btn_graphics_file_names_set.pickle'

    if os.path.isfile(graphics_file_names_set_file_path):
        with open(graphics_file_names_set_file_path, 'rb') as graphics_file_names_set_file:
            old_graphics_file_names_set = pickle.load(graphics_file_names_set_file)

            for old_graphics_file_name in old_graphics_file_names_set:
                if old_graphics_file_name not in new_graphics_file_names_set:
                    print('Removing old graphics item build files: ' + old_graphics_file_name)
                    remove_file(build_folder_path + '/' + old_graphics_file_name + '_btn_graphics.o')
                    remove_file(build_folder_path + '/' + old_graphics_file_name + '_btn_graphics.d')
                    remove_file(build_folder_path + '/' + old_graphics_file_name + '_btn_graphics.s')
                    remove_file(build_folder_path + '/btn_sprite_items_' + old_graphics_file_name + '.h')
                    remove_file(build_folder_path + '/btn_bg_items_' + old_graphics_file_name + '.h')
                    remove_file(build_folder_path + '/_' + old_graphics_file_name + '_file_info.txt')

    with open(graphics_file_names_set_file_path, 'wb') as graphics_file_names_set_file:
        pickle.dump(new_graphics_file_names_set, graphics_file_names_set_file)


def process(graphics_folder_paths, build_folder_path):
    graphics_folder_infos = list_graphics_folder_infos(graphics_folder_paths, build_folder_path)
    graphics_file_names_set = set()

    for graphics_folder_info in graphics_folder_infos:
        for graphics_file_path in graphics_folder_info.file_paths():
            graphics_file_name = os.path.basename(graphics_file_path)
            graphics_file_name_no_ext = os.path.splitext(graphics_file_name)[0]
            if graphics_file_name_no_ext in graphics_file_names_set:
                raise ValueError('There\'s two or more graphics items with the same name: ' + graphics_file_name_no_ext)

            graphics_file_names_set.add(graphics_file_name_no_ext)
            file_info_path = build_folder_path + '/_btn_' + graphics_file_name_no_ext + '_file_info.txt'
            old_file_info = FileInfo.read(file_info_path)
            new_file_info = FileInfo.build_from_file(graphics_file_path)
            new_graphics_json = old_file_info != new_file_info

            if new_graphics_json or graphics_folder_info.new_graphics_json():
                print('Processing graphics file: ' + graphics_file_path)

                try:
                    item_info = graphics_folder_info.get_sprite(graphics_file_name_no_ext)
                    item = SpriteItem(graphics_file_path, graphics_file_name_no_ext, build_folder_path, item_info)
                except KeyError:
                    try:
                        item_info = graphics_folder_info.get_bg(graphics_file_name_no_ext)
                        item = BgItem(graphics_file_path, graphics_file_name_no_ext, build_folder_path, item_info)
                    except KeyError:
                        raise ValueError(graphics_file_name_no_ext + ' not found in graphics.json')

                item.process()
                item.write_header()

                if new_graphics_json:
                    new_file_info.write(file_info_path)

    remove_old_graphics_items(graphics_file_names_set, build_folder_path)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='butano graphics tool.')
    parser.add_argument('--graphics', required=True, help='graphics folder paths')
    parser.add_argument('--build', required=True, help='build folder path')
    args = parser.parse_args()
    process(args.graphics, args.build)
