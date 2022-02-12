"""
Copyright (c) 2022 laqieer laqieer@126.com
zlib License, see LICENSE file.
"""

# Doc on BMFont: https://angelcode.com/products/bmfont/doc/file_format.html

import os
import sys
import shlex
import codecs
import argparse

from file_info import FileInfo
from PIL import Image

trim_fonts = False
unique_characters = {' ', '!', '"', '#', '$', '%', '&', "'", '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~'}


def list_texts_files(texts_paths):
    global trim_fonts
    texts_file_names = []
    texts_file_paths = []

    if texts_paths is not None:
        texts_path_list = texts_paths.split(' ')

        for texts_path in texts_path_list:
            if os.path.isfile(texts_path):
                texts_file_name = os.path.basename(texts_path)
                if FileInfo.validate(texts_file_name):
                    texts_file_names.append(texts_file_name)
                    texts_file_paths.append(texts_path)
            elif os.path.isdir(texts_path):
                folder_texts_file_names = sorted(os.listdir(texts_path))
                for texts_file_name in folder_texts_file_names:
                    texts_file_path = texts_path + '/' + texts_file_name

                    if os.path.isfile(texts_file_path) and FileInfo.validate(texts_file_name):
                        texts_file_names.append(texts_file_name)
                        texts_file_paths.append(texts_file_path)

    trim_fonts = len(texts_file_names) > 0

    return texts_file_names, texts_file_paths


def process_texts_files(texts_file_paths):
    global unique_characters
    for texts_file_path in texts_file_paths:
        with open(texts_file_path, 'r', encoding='UTF-8') as texts_file:
            for line in texts_file:
                for char in line:
                    unique_characters.add(char)


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
    global trim_fonts, unique_characters
    fonts_graphics_path = build_folder_path + '/fonts/'
    if not os.path.exists(fonts_graphics_path):
        os.makedirs(fonts_graphics_path)
    total_number = 0
    for fonts_file_path in fonts_file_paths:
        fonts_file_path_no_ext = os.path.splitext(fonts_file_path)[0]
        fonts_folder_path, fonts_file_name_no_ext = os.path.split(fonts_file_path_no_ext)
        font_name = fonts_file_name_no_ext + '_sprite_font'
        fonts_header_path = build_folder_path + '/' + font_name + '.h'

        with open(fonts_file_path, 'r') as fonts_file, open(fonts_header_path, 'w', encoding='utf-8') as header_file:
            font_chars = []
            font_widths = [0] * 95
            unique_chars = unique_characters.copy()

            for fonts_line in fonts_file:
                line_type, *pair_tokens = shlex.split(fonts_line)
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
                    font_base = int(line_conf['base'])
                    font_y_offset = min(font_base - font_height, 0)
                    # Assume a font's width is not more than its height
                    font_width = font_height
                    font_pages = [None] * int(line_conf['pages'])
                elif line_type == "page":
                    page_file_path = fonts_folder_path + '/' + line_conf['file'].replace('"', '')
                    font_pages[int(line_conf['id'])] = Image.open(page_file_path)
                elif line_type == "chars":
                    font_number = int(line_conf['count'])
                    #fonts_image = Image.new('RGBA', (font_width, font_height * font_number))
                    transparent_color = font_pages[0].getpixel((0, 0))
                    fonts_image = Image.new('RGB', (font_width, font_height * (font_number + 94)), transparent_color)
                elif line_type == "char":
                    if len(unique_characters) == 0:
                        break
                    font_code = int(line_conf['id'])
                    if not trim_fonts or chr(font_code) in unique_chars:
                        if trim_fonts:
                            unique_chars.remove(chr(font_code))
                        src_left = int(line_conf['x']) + padding_left
                        src_upper = int(line_conf['y']) + padding_up
                        src_right = int(line_conf['x']) + int(line_conf['width']) - padding_right
                        if src_right < src_left:
                            src_right = src_left
                        src_right = min(src_right, src_left + font_width)
                        src_lower = int(line_conf['y']) + int(line_conf['height']) - padding_down
                        if src_lower < src_upper:
                            src_lower = src_upper
                        src_upper = max(src_upper, src_lower - font_height)
                        dst_left = round(float(line_conf['xoffset'])) + padding_left
                        if dst_left < 0:
                            dst_left = 0
                        dst_right = dst_left + src_right - src_left
                        if dst_right > font_width:
                            dst_left -= min(dst_left, dst_right - font_width)
                        dst_upper = round(float(line_conf['yoffset'])) + padding_up
                        if dst_upper < 0:
                            dst_upper = 0
                        dst_lower = dst_upper + src_lower - src_upper
                        if dst_lower > font_height:
                            dst_upper -= min(dst_upper, dst_lower - font_height)
                        if dst_lower > 0:
                            dst_lower -= min(dst_lower, font_y_offset)
                        font_w = max(int(line_conf['xadvance']), int(line_conf['width']) - padding_left -padding_right)
                        font_w = min(font_w, font_width)
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
            header_file.write('#include "bn_utf8_characters_map.h"\n')
            header_file.write('#include "bn_sprite_items_' + fonts_file_name_no_ext + '.h"\n')
            header_file.write('\n')
            header_file.write('constexpr bn::utf8_character ' + font_name + '_utf8_characters[] = {\n')
            header_file.write('    "' + '", "'.join(font_chars) + '"\n')
            header_file.write('};\n')
            header_file.write('\n')
            header_file.write('constexpr bn::span<const bn::utf8_character> ' + font_name + '_utf8_characters_span(\n')
            header_file.write('        ' + font_name + '_utf8_characters);\n')
            header_file.write('\n')
            header_file.write('constexpr auto ' + font_name + '_utf8_characters_map =\n')
            header_file.write('        bn::utf8_characters_map<' + font_name + '_utf8_characters_span>();\n')
            header_file.write('\n')
            header_file.write('constexpr int8_t ' + font_name + '_character_widths[] = {\n')
            header_file.write('    ' + ', '.join([str(x) for x in font_widths]) + '\n')
            header_file.write('};\n')
            header_file.write('\n')
            header_file.write('constexpr bn::sprite_font ' + font_name + '(\n')
            header_file.write('        bn::sprite_items::' + fonts_file_name_no_ext + ',\n')
            header_file.write('        ' + font_name + '_utf8_characters_map.reference(),\n')
            header_file.write('        ' + font_name + '_character_widths);\n')
            header_file.write('\n')
            header_file.write('#endif')
            fonts_image_path_no_ext = fonts_graphics_path + fonts_file_name_no_ext
            font_number = len(font_widths) - 1
            total_number += font_number
            fonts_image_trimmed = Image.new('RGB', (font_width, font_height * total_number), transparent_color)
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


def process_fonts(fonts_folder_paths, build_folder_path, texts_paths):
    texts_file_names, texts_file_paths = list_texts_files(texts_paths)
    text_file_info_path = build_folder_path + '/_bn_texts_files_info.txt'
    old_text_file_info = FileInfo.read(text_file_info_path)
    new_text_file_info = FileInfo.build_from_files(texts_file_paths)

    fonts_file_names, fonts_file_paths = list_fonts_files(fonts_folder_paths)
    font_file_info_path = build_folder_path + '/_bn_fonts_files_info.txt'
    old_font_file_info = FileInfo.read(font_file_info_path)
    new_font_file_info = FileInfo.build_from_files(fonts_file_paths)

    if old_font_file_info == new_font_file_info and old_text_file_info == new_text_file_info:
        return

    for fonts_file_name in fonts_file_names:
        print(fonts_file_name)

    sys.stdout.flush()

    process_texts_files(texts_file_paths)
    total_number = process_fonts_files(fonts_file_paths, build_folder_path)
    print('    Processed character number: ' + str(total_number))
    new_font_file_info.write(font_file_info_path)
    new_text_file_info.write(text_file_info_path)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Butano fonts tool.')
    parser.add_argument('--build', required=True, help='build folder path')
    parser.add_argument('--fonts', required=True, help='fonts folder paths')
    parser.add_argument('--texts', required=False, help='texts folder or files paths')

    try:
        args = parser.parse_args()
        process_fonts(args.fonts, args.build, args.texts)
    except Exception as ex:
        sys.stderr.write('Error: ' + str(ex) + '\n')
        traceback.print_exc()
        exit(-1)
