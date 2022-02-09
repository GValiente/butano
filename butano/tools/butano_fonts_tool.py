"""
Copyright (c) 2022 laqieer laqieer@126.com
zlib License, see LICENSE file.
"""

# Doc on BMFont: https://angelcode.com/products/bmfont/doc/file_format.html

import os
import sys
import codecs

from file_info import FileInfo
from PIL import Image


def list_fonts_files(fonts_folder_paths):
    fonts_folder_path_list = fonts_folder_paths.split(' ')
    fonts_file_names = []
    fonts_file_paths = []

    for fonts_folder_path in fonts_folder_path_list:
        folder_fonts_file_names = sorted(os.listdir(fonts_folder_path))

        for fonts_file_name in folder_fonts_file_names:
            if fonts_file_name.endswith('.fnt'):
                fonts_file_path = fonts_folder_path + '/' + fonts_file_name

                if os.path.isfile(fonts_file_path) and FileInfo.validate(fonts_file_name):
                    fonts_file_names.append(fonts_file_name)
                    fonts_file_paths.append(fonts_file_path)

    return fonts_file_names, fonts_file_paths


def process_fonts_files(fonts_file_paths, build_folder_path):
    total_number = 0
    for fonts_file_path in fonts_file_paths:
        fonts_file_path_no_ext = os.path.splitext(fonts_file_path)[0]
        fonts_folder_path, fonts_file_name_no_ext = os.path.split(fonts_file_path_no_ext)
        font_name = fonts_file_name_no_ext + '_sprite_font'
        fonts_header_path = build_folder_path + '/' + font_name + '.h'

        with open(fonts_file_path, 'r') as fonts_file, open(fonts_header_path, 'w', encoding='utf-8') as header_file:
            font_chars = []
            font_widths = [0] * 95

            for fonts_line in fonts_file:
                line_type, *pair_tokens = fonts_line.split()
                line_conf = dict(pair_token.split("=", 1) for pair_token in pair_tokens)

                if line_type == "info":
                    padding_up, padding_right, padding_down, padding_left = [int(x) for x in line_conf['padding'].split(',')]
                    header_file.write('// ' + line_conf['face'].replace('"', '') + ' ' + line_conf['size'] + 'px')
                    if line_conf['bold'] != '0':
                        header_file.write(' bold')
                    if line_conf['italic'] != '0':
                        header_file.write(' italic')
                    header_file.write('\n')
                    header_file.write('\n')
                elif line_type == "common":
                    font_height = int(line_conf['lineHeight'])
                    if font_height > 64:
                        raise ValueError('Font is too large')
                    elif font_height > 32:
                        font_height = 64
                    elif font_height > 16:
                        font_height = 32
                    elif font_height > 8:
                        font_height = 16
                    else:
                        font_height = 8
                    # Assume a font's width is not more than its height
                    font_width = font_height
                    font_pages = [None] * int(line_conf['pages'])
                elif line_type == "page":
                    page_file_path = fonts_folder_path + '/' + line_conf['file'].replace('"', '')
                    font_pages[int(line_conf['id'])] = Image.open(page_file_path)
                elif line_type == "chars":
                    font_number = int(line_conf['count'])
                    total_number += font_number
                    #fonts_image = Image.new('RGBA', (font_width, font_height * font_number))
                    transparent_color = font_pages[0].getpixel((0, 0))
                    fonts_image = Image.new('RGB', (font_width, font_height * (font_number + 94)), transparent_color)
                elif line_type == "char":
                    src_left = int(line_conf['x']) + padding_left
                    src_upper = int(line_conf['y']) + padding_up
                    src_right = int(line_conf['x']) + int(line_conf['width']) - padding_right
                    if src_right < src_left:
                        src_right = src_left
                    src_lower = int(line_conf['y']) + int(line_conf['height']) - padding_down
                    if src_lower < src_upper:
                        src_lower = src_upper
                    dst_left = round(float(line_conf['xoffset'])) + padding_left
                    if dst_left < 0:
                        dst_left = 0
                    dst_upper = round(float(line_conf['yoffset'])) + padding_up
                    if dst_upper < 0:
                        dst_upper = 0
                    font_code = int(line_conf['id'])
                    font_w = int(line_conf['xadvance'])
                    if font_w > font_width:
                        font_w = font_width
                    if font_code > 126:
                        font_chars.append(chr(font_code))
                        font_widths.append(font_w)
                        dst_upper += font_height * (len(font_widths) - 2)
                    elif font_code > 31:
                        font_widths[font_code - 32] = font_w
                        dst_upper += font_height * (font_code - 33)
                    if font_code > 32:
                        fonts_image.paste(font_pages[int(line_conf['page'])].crop((src_left, src_upper, src_right, src_lower)), (dst_left, dst_upper))

            header_file.write('#ifndef ' + font_name.upper() + '_H\n')
            header_file.write('#define ' + font_name.upper() + '_H\n')
            header_file.write('\n')
            header_file.write('#include "bn_sprite_font.h"\n')
            header_file.write('#include "bn_sprite_items_' + fonts_file_name_no_ext + '.h"\n')
            header_file.write('\n')
            header_file.write('constexpr bn::string_view ' + font_name + '_utf8_characters[] = {\n')
            header_file.write('    "' + '", "'.join(font_chars) + '"\n')
            header_file.write('};\n')
            header_file.write('\n')
            header_file.write('constexpr int8_t ' + font_name + '_character_widths[] = {\n')
            header_file.write('    ' + ', '.join([str(x) for x in font_widths]) + '\n')
            header_file.write('};\n')
            header_file.write('\n')
            header_file.write('constexpr bn::sprite_font ' + font_name + '(\n')
            header_file.write('        bn::sprite_items::' + fonts_file_name_no_ext + ',\n')
            header_file.write('        ' + font_name + '_utf8_characters,\n')
            header_file.write('        ' + font_name + '_character_widths);\n')
            header_file.write('\n')
            header_file.write('#endif')
            fonts_image_path_no_ext = build_folder_path + '/fonts/' + fonts_file_name_no_ext
            fonts_image_trimmed = Image.new('RGB', (font_width, font_height * (len(font_widths) - 1)), transparent_color)
            fonts_image_trimmed.paste(fonts_image)
            #fonts_image_trimmed.save(fonts_image_path_no_ext + '.png')
            fonts_image_trimmed = fonts_image_trimmed.convert("P", palette=Image.ADAPTIVE, colors=16)
            transparent_color_index = fonts_image_trimmed.getpixel((0, 0))
            if transparent_color_index > 0:
                dest_map = list(range(16))
                dest_map[0], dest_map[transparent_color_index] = transparent_color_index, 0
                fonts_image_trimmed = fonts_image_trimmed.remap_palette(dest_map)
            fonts_image_trimmed.save(fonts_image_path_no_ext + '.bmp')
            with open(fonts_image_path_no_ext + '.json', 'w') as json_file:
                json_file.write('{\n')
                json_file.write('    "type": "sprite",\n')
                json_file.write('    "height": ' + str(font_height) + '\n')
                json_file.write('}\n')
            print('    ' + fonts_file_path + ' font header written in ' + fonts_header_path + ' (character number: ' + str(font_number) + ')')

    return total_number


def process_fonts(fonts_folder_paths, build_folder_path):
    fonts_file_names, fonts_file_paths = list_fonts_files(fonts_folder_paths)
    file_info_path = build_folder_path + '/_bn_fonts_files_info.txt'
    old_file_info = FileInfo.read(file_info_path)
    new_file_info = FileInfo.build_from_files(fonts_file_paths)

    if old_file_info == new_file_info:
        return

    for fonts_file_name in fonts_file_names:
        print(fonts_file_name)

    sys.stdout.flush()

    total_number = process_fonts_files(fonts_file_paths, build_folder_path)
    print('    Processed character number: ' + str(total_number))
    new_file_info.write(file_info_path)
