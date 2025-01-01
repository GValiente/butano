"""
Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
zlib License, see LICENSE file.
"""

import os
import json
import sys

from file_info import FileInfo
from pool import create_pool


class DmgAudioFileInfo:

    def __init__(self, json_file_path, file_path, file_name, file_name_no_ext, file_name_ext, file_info_path):
        self.__json_file_path = json_file_path
        self.__file_path = file_path
        self.__file_name = file_name
        self.__file_name_no_ext = file_name_no_ext
        self.__file_name_ext = file_name_ext
        self.__file_info_path = file_info_path
        self.__import_instruments = False
        self.__mod_speed_conversion = True

    def print_file_name(self):
        print(self.__file_name)

    def process(self, build_folder_path):
        output_tag = self.__file_name_no_ext + '_bn_dmg'
        output_file_name = output_tag + '.c'
        output_file_path = build_folder_path + '/' + output_file_name

        try:
            if self.__json_file_path is not None:
                try:
                    with open(self.__json_file_path) as json_file:
                        info = json.load(json_file)
                except Exception as exception:
                    raise ValueError(self.__json_file_path + ' DMG audio json file parse failed: ' + str(exception))

                try:
                    self.__import_instruments = bool(info['import_instruments'])
                except KeyError:
                    pass

                try:
                    self.__mod_speed_conversion = bool(info['mod_speed_conversion'])
                except KeyError:
                    pass

            music_type = 'GBT_PLAYER'
            file_size = -1

            if self.__file_name_ext == '.mod':
                self.__execute_mod2gbt_command(output_tag)
                self.__move_output_file(output_file_name, output_file_path)
            elif self.__file_name_ext == '.s3m':
                self.__execute_s3m2gbt_command(output_tag, output_file_path)
            else:
                file_size = self.__execute_vgm2gba_command(output_tag, output_file_path)
                music_type = 'VGM'

            header_file_path = self.__write_header(build_folder_path, output_tag, music_type)

            with open(self.__file_info_path, 'w') as file_info:
                file_info.write('')

            return [self.__file_name, header_file_path, file_size]
        except Exception as exc:
            if os.path.exists(output_file_name):
                os.remove(output_file_name)

            return [self.__file_name, exc]

    def __execute_mod2gbt_command(self, output_tag):
        import io
        from mod2gbt import mod2gbt

        sys.stdout = io.StringIO()

        try:
            mod2gbt.convert_file(self.__file_path, output_tag, self.__mod_speed_conversion)
            sys.stdout = sys.__stdout__
        except Exception:
            sys.stdout = sys.__stdout__
            raise

    def __execute_s3m2gbt_command(self, output_tag, output_file_path):
        import io
        from s3m2gbt import s3m2gbt

        sys.stdout = io.StringIO()

        try:
            s3m2gbt.convert_file(self.__file_path, output_tag, output_file_path, self.__import_instruments)
            sys.stdout = sys.__stdout__
        except Exception:
            sys.stdout = sys.__stdout__
            raise

    def __execute_vgm2gba_command(self, output_tag, output_file_path):
        import io
        from vgm2gba import vgm2gba

        sys.stdout = io.StringIO()

        try:
            vgm2gba.convert_file_c_array(self.__file_path, output_file_path, output_tag)
            sys.stdout = sys.__stdout__

            total_size = -1

            with open(output_file_path, 'r') as output_file:
                output_data = output_file.read()

                for output_line in output_data.splitlines():
                    if output_line.startswith('const unsigned int'):
                        total_size = output_line.split()[-1]
                        total_size = int(total_size[:-1])
                        break

            return total_size
        except Exception:
            sys.stdout = sys.__stdout__
            raise

    @staticmethod
    def __move_output_file(output_file_name, output_file_path):
        if os.path.exists(output_file_path):
            os.remove(output_file_path)

        os.rename(output_file_name, output_file_path)

    def __write_header(self, build_folder_path, output_tag, music_type):
        name = self.__file_name_no_ext
        header_file_path = build_folder_path + '/bn_dmg_music_items_' + name + '.h'

        with open(header_file_path, 'w') as header_file:
            include_guard = 'BN_DMG_MUSIC_ITEMS_' + name.upper() + '_H'
            header_file.write('#ifndef ' + include_guard + '\n')
            header_file.write('#define ' + include_guard + '\n')
            header_file.write('\n')
            header_file.write('#include "bn_dmg_music_item.h"' + '\n')
            header_file.write('\n')
            header_file.write('extern const uint8_t ' + output_tag + '[];' + '\n')
            header_file.write('\n')
            header_file.write('namespace bn::dmg_music_items' + '\n')
            header_file.write('{' + '\n')
            header_file.write('    constexpr inline dmg_music_item ' + name + '(*' + output_tag +
                              ', dmg_music_type::' + music_type + ');' + '\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        return header_file_path


class DmgAudioFileInfoProcessor:

    def __init__(self, build_folder_path):
        self.__build_folder_path = build_folder_path

    def __call__(self, audio_file_info):
        return audio_file_info.process(self.__build_folder_path)


def list_dmg_audio_file_infos(audio_paths, build_folder_path):
    audio_file_paths = []

    for audio_path in audio_paths.split(' '):
        if os.path.isdir(audio_path):
            audio_file_names = os.listdir(audio_path)

            for audio_file_name in audio_file_names:
                audio_file_path = audio_path + '/' + audio_file_name

                if os.path.isfile(audio_file_path):
                    audio_file_paths.append(audio_file_path)
        elif os.path.isfile(audio_path):
            audio_file_paths.append(audio_path)

    audio_file_infos = []
    file_names_set = set()

    for audio_file_path in audio_file_paths:
        audio_file_name = os.path.basename(audio_file_path)

        if os.path.isfile(audio_file_path) and FileInfo.validate(audio_file_name):
            audio_file_name_split = os.path.splitext(audio_file_name)
            audio_file_name_ext = audio_file_name_split[1]

            if audio_file_name_ext == '.mod' or audio_file_name_ext == '.s3m' or audio_file_name_ext == '.vgm':
                audio_file_name_no_ext = audio_file_name_split[0]

                if audio_file_name_no_ext in file_names_set:
                    raise ValueError('There\'s two or more DMG audio files with the same name: ' +
                                     audio_file_name_no_ext)

                file_names_set.add(audio_file_name_no_ext)
                json_file_path = audio_file_path[:-len(audio_file_name_ext)] + '.json'

                if not os.path.isfile(json_file_path):
                    json_file_path = None

                file_info_path = build_folder_path + '/_bn_' + audio_file_name_no_ext

                if json_file_path is not None:
                    file_info_path += '_dmg_audio_with_json_file_info.txt'
                else:
                    file_info_path += '_dmg_audio_without_json_file_info.txt'

                if not os.path.exists(file_info_path):
                    build = True
                else:
                    file_info_mtime = os.path.getmtime(file_info_path)
                    audio_file_mtime = os.path.getmtime(audio_file_path)
                    build = file_info_mtime < audio_file_mtime

                    if not build and json_file_path is not None:
                        json_file_mtime = os.path.getmtime(json_file_path)
                        build = file_info_mtime < json_file_mtime

                if build:
                    audio_file_infos.append(DmgAudioFileInfo(
                        json_file_path, audio_file_path, audio_file_name, audio_file_name_no_ext, audio_file_name_ext,
                        file_info_path))

    return audio_file_infos


def process_dmg_audio(audio_paths, build_folder_path):
    if len(audio_paths) == 0:
        return

    audio_file_infos = list_dmg_audio_file_infos(audio_paths, build_folder_path)

    if len(audio_file_infos) > 0:
        for audio_file_info in audio_file_infos:
            audio_file_info.print_file_name()

        sys.stdout.flush()

        pool = create_pool()
        process_results = pool.map(DmgAudioFileInfoProcessor(build_folder_path), audio_file_infos)
        pool.close()

        process_excs = []

        for process_result in process_results:
            if len(process_result) == 3:
                result = '    ' + str(process_result[0]) + ' item header written in ' + str(process_result[1])
                file_size = process_result[2]

                if file_size >= 0:
                    result += ' (music size: ' + str(file_size) + ' bytes)'

                print(result)
            else:
                process_excs.append(process_result)

        sys.stdout.flush()

        if len(process_excs) > 0:
            for process_exc in process_excs:
                sys.stderr.write(str(process_exc[0]) + ' error: ' + str(process_exc[1]) + '\n')

            exit(-1)
