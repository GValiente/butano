import os
import json
import struct
import argparse
import subprocess


class Info:

    def __init__(self, file_path):
        with open(file_path) as file:
            data = json.load(file)
            self.__sprites = data['sprites']

    def get(self, file_name_no_ext):
        try:
            return self.__sprites[file_name_no_ext]
        except KeyError:
            raise ValueError(file_name_no_ext + ' not found in graphics.json')


class BMP:

    def __init__(self, file_path):
        self.width = None
        self.height = None
        self.colors = None

        with open(file_path, 'rb') as file:
            type = file.read(2).decode()

            if type != 'BM':
                raise ValueError('Invalid image type: ' + type)

            size = struct.unpack('I', file.read(4))
            reserved_1 = struct.unpack('H', file.read(2))
            reserved_2 = struct.unpack('H', file.read(2))
            offset = struct.unpack('I', file.read(4))

            header_size = struct.unpack('I', file.read(4))
            self.width = struct.unpack('I', file.read(4))[0]
            self.height = struct.unpack('I', file.read(4))[0]
            color_planes = struct.unpack('H', file.read(2))
            bits_per_pixel = struct.unpack('H', file.read(2))[0]

            if bits_per_pixel != 4 and bits_per_pixel != 8:
                raise ValueError('Invalid bits per pixel: ' + str(bits_per_pixel))

            compression_method = struct.unpack('I', file.read(4))[0]

            if compression_method != 0:
                raise ValueError('Compression method not supported: ' + str(compression_method))

            raw_image_size = struct.unpack('I', file.read(4))
            horizontal_resolution = struct.unpack('I', file.read(4))
            vertical_resolution = struct.unpack('I', file.read(4))
            colors = struct.unpack('I', file.read(4))[0]

            if colors > 256:
                raise ValueError('Invalid colors count: ' + str(colors))

            self.colors = max(colors, 16)
            important_colors = struct.unpack('I', file.read(4))


class Item:

    def __init__(self, file_path, file_name_no_ext, info):
        item_info = info.get(file_name_no_ext)
        bmp = BMP(file_path)
        self.__file_path = file_path
        self.__file_name_no_ext = file_name_no_ext
        self.__colors = bmp.colors

        try:
            height = int(item_info['height'])

            if bmp.height % height:
                raise ValueError('File height is not divisible by item height: ' + str(bmp.height) + ' - ' + str(height))
        except KeyError:
            height = bmp.height

        self.__graphics = bmp.height / height
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
                raise ValueError('Invalid sprite width and height: ' + str(width) + ' - ' + str(height))
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
                raise ValueError('Invalid sprite width and height: ' + str(width) + ' - ' + str(height))
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
                raise ValueError('Invalid sprite width and height: ' + str(width) + ' - ' + str(height))
            elif height == 16:
                raise ValueError('Invalid sprite width and height: ' + str(width) + ' - ' + str(height))
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


    def write_header(self, build_folder_path):
        name = self.__file_name_no_ext
        grit_file_path = build_folder_path + '/' + name + '_btn_graphics.h'
        header_file_path = build_folder_path + '/btn_' + name + '_sprite_item.h'

        with open(grit_file_path, 'r') as grit_file:
            grit_data = grit_file.read()
            grit_data = grit_data.replace('unsigned int', 'btn::tile')
            grit_data = grit_data.replace(']', ' / (sizeof(btn::tile) / sizeof(unsigned int))]', 1)
            grit_data = grit_data.replace('unsigned short', 'btn::color')

        os.remove(grit_file_path)

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
                    'span<const color>(' + name + '_btn_graphicsPal), ' +
                    str(self.__graphics) + ');' + '\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        print('sprite_item file written in ' + header_file_path)

    def process(self, build_folder_path):
        command = []
        command.append('grit')
        command.append(self.__file_path)
        command.append('-gt')

        if self.__colors == 16:
            command.append('-gB4')
        else:
            command.append('-gB8')

        command.append('-o' + build_folder_path + '/' + self.__file_name_no_ext + '_btn_graphics')
        command = ' '.join(command)

        try:
            subprocess.check_output([command], shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError('grit call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


def list_graphics_file_paths(graphics_folder_paths):
    graphics_folder_path_list = graphics_folder_paths.split(' ')
    graphics_file_paths = []

    for graphics_folder_path in graphics_folder_path_list:
        graphics_file_names = sorted(os.listdir(graphics_folder_path))

        for graphics_file_name in graphics_file_names:
            graphics_file_name_ext = os.path.splitext(graphics_file_name)[-1]

            if graphics_file_name_ext.lower() == '.bmp':
                graphics_file_path = graphics_folder_path + '/' + graphics_file_name

                if os.path.isfile(graphics_file_path):
                    graphics_file_paths.append(graphics_file_path)

    return graphics_file_paths


def read_graphics_desc(graphics_desc_path):
    if not os.path.isfile(graphics_desc_path):
        return None

    with open(graphics_desc_path, 'r') as graphics_desc_file:
        return graphics_desc_file.read()


def build_graphics_desc(graphics_file_path):
    graphics_desc = []
    graphics_desc.append(graphics_file_path)
    graphics_desc.append(str(os.path.getsize(graphics_file_path)))
    graphics_desc.append(str(os.path.getmtime(graphics_file_path)))
    return '\n'.join(graphics_desc)


def write_graphics_desc(graphics_desc, graphics_desc_path):
    with open(graphics_desc_path, 'w') as graphics_desc_file:
        graphics_desc_file.write(graphics_desc)


def process(info_file_path, graphics_folder_paths, build_folder_path):
    info = None
    graphics_file_paths = list_graphics_file_paths(graphics_folder_paths)

    for graphics_file_path in graphics_file_paths:
        graphics_file_name = os.path.basename(graphics_file_path)
        graphics_file_name_no_ext = os.path.splitext(graphics_file_name)[0]
        graphics_desc_path = build_folder_path + '/_btn_' + graphics_file_name_no_ext + '_graphics_desc.txt'
        old_graphics_desc = read_graphics_desc(graphics_desc_path)
        new_graphics_desc = build_graphics_desc(graphics_file_path)

        if old_graphics_desc is None or old_graphics_desc != new_graphics_desc:
            print('Processing graphics file: ' + graphics_file_path)

            if info is None:
                info = Info(info_file_path)

            item = Item(graphics_file_path, graphics_file_name_no_ext, info)
            item.process(build_folder_path)
            item.write_header(build_folder_path)
            write_graphics_desc(new_graphics_desc, graphics_desc_path)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='butano graphics tool.')
    parser.add_argument('--info', required=True, help='graphics info file path')
    parser.add_argument('--graphics', required=True, help='graphics folder paths')
    parser.add_argument('--build', required=True, help='build folder path')
    args = parser.parse_args()
    process(args.info, args.graphics, args.build)
