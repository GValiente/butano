import os
import json
import argparse
import subprocess
import time

from bmp import BMP
from file_info import FileInfo


class GraphicsFolderInfo:

    def __init__(self, sprites, bgs, file_paths):
        self.__sprites = sprites
        self.__bgs = bgs
        self.__file_paths = file_paths

    def get_sprite(self, file_name_no_ext):
        return self.__sprites[file_name_no_ext]

    def get_bg(self, file_name_no_ext):
        return self.__bgs[file_name_no_ext]

    def file_paths(self):
        return self.__file_paths


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
        header_file_path = self.__build_folder_path + '/btn_' + name + '_sprite_item.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned int', 'btn::tile')
            grit_data = grit_data.replace(']', ' / (sizeof(btn::tile) / sizeof(uint32_t))]', 1)
            grit_data = grit_data.replace('unsigned short', 'btn::color')

        os.remove(grit_file_path)

        if self.__bpp8:
            bpp_mode_label = 'palette_bpp_mode::BPP_8'
        else:
            bpp_mode_label = 'palette_bpp_mode::BPP_4'

        with open(header_file_path, 'w') as header_file:
            include_guard = 'BTN_' + name.upper() + '_SPRITE_ITEM_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "btn_sprite_item.h"' + '\n')
            header_file.write(grit_data)
            header_file.write('\n')
            header_file.write('namespace btn::sprite_items' + '\n')
            header_file.write('{' + '\n')
            header_file.write('    constexpr const sprite_item ' + name + '(' +
                              'sprite_shape::' + self.__shape + ', ' +
                              'sprite_size::' + self.__size + ', ' + '\n            ' +
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
            subprocess.check_output([command], shell=True, stderr=subprocess.STDOUT)
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
        header_file_path = self.__build_folder_path + '/btn_' + name + '_bg_item.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned int', 'btn::tile', 1)
            grit_data = grit_data.replace(']', ' / (sizeof(btn::tile) / sizeof(uint32_t))]', 1)
            grit_data = grit_data.replace('unsigned short', 'btn::bg_map_cell', 1)
            grit_data = grit_data.replace('unsigned short', 'btn::color', 1)

        os.remove(grit_file_path)

        if self.__bpp8:
            bpp_mode_label = 'palette_bpp_mode::BPP_8'
        else:
            bpp_mode_label = 'palette_bpp_mode::BPP_4'

        with open(header_file_path, 'w') as header_file:
            include_guard = 'BTN_' + name.upper() + '_BG_ITEM_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "btn_bg_item.h"' + '\n')
            header_file.write(grit_data)
            header_file.write('\n')
            header_file.write('namespace btn::bg_items' + '\n')
            header_file.write('{' + '\n')
            header_file.write('    constexpr const bg_item ' + name + '(' +
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
            subprocess.check_output([command], shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError('grit call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


def list_graphics_folder_infos(graphics_folder_paths):
    graphics_folder_path_list = graphics_folder_paths.split(' ')
    graphics_folder_infos = []

    for graphics_folder_path in graphics_folder_path_list:
        graphics_json_file_path = graphics_folder_path + '/graphics.json'

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
            graphics_file_name_ext = os.path.splitext(graphics_file_name)[-1]

            if graphics_file_name_ext.lower() == '.bmp':
                graphics_file_path = graphics_folder_path + '/' + graphics_file_name

                if os.path.isfile(graphics_file_path):
                    graphics_file_paths.append(graphics_file_path)

        graphics_folder_infos.append(GraphicsFolderInfo(sprites, bgs, graphics_file_paths))

    return graphics_folder_infos


def process(graphics_folder_paths, build_folder_path):
    graphics_folder_infos = list_graphics_folder_infos(graphics_folder_paths)

    for graphics_folder_info in graphics_folder_infos:
        for graphics_file_path in graphics_folder_info.file_paths():
            graphics_file_name = os.path.basename(graphics_file_path)
            graphics_file_name_no_ext = os.path.splitext(graphics_file_name)[0]
            file_info_path = build_folder_path + '/_btn_' + graphics_file_name_no_ext + '_file_info.txt'
            old_file_info = FileInfo.read(file_info_path)
            new_file_info = FileInfo.build_from_file(graphics_file_path)

            if old_file_info != new_file_info:
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
                new_file_info.write(file_info_path)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='butano graphics tool.')
    parser.add_argument('--graphics', required=True, help='graphics folder paths')
    parser.add_argument('--build', required=True, help='build folder path')
    args = parser.parse_args()
    process(args.graphics, args.build)
