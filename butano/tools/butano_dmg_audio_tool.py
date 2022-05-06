"""
Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
zlib License, see LICENSE file.
"""

import os
import sys
import subprocess
from multiprocessing import Pool

from file_info import FileInfo


class DmgAudioFileInfo:

    def __init__(self, file_path, file_name, file_name_no_ext, file_info_path, is_mod):
        self.__file_path = file_path
        self.__file_name = file_name
        self.__file_name_no_ext = file_name_no_ext
        self.__file_info_path = file_info_path
        self.__is_mod = is_mod

    def print_file_name(self):
        print(self.__file_name)

    def process(self, mod2gbt_file_path, build_folder_path):
        output_tag = self.__file_name_no_ext + '_bn_dmg'
        output_file_name = output_tag + '.c'
        output_file_path = build_folder_path + '/' + output_file_name

        try:
            if self.__is_mod:
                self.__execute_mod2gbt_command(mod2gbt_file_path, output_tag)
                self.__move_output_file(output_file_name, output_file_path)
            else:
                self.__execute_s3m2gbt_command(output_tag, output_file_path)

            header_file_path = self.__write_header(build_folder_path, output_tag)

            with open(self.__file_info_path, 'w') as file_info:
                file_info.write('')

            return [self.__file_name, header_file_path, None]
        except Exception as exc:
            if os.path.exists(output_file_name):
                os.remove(output_file_name)

            return [self.__file_name, exc]

    def __execute_mod2gbt_command(self, mod2gbt_file_path, output_tag):
        command = [mod2gbt_file_path, self.__file_path, output_tag]
        command = ' '.join(command)

        try:
            subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise ValueError('mod2gbt call failed (return code ' + str(e.returncode) + '): ' + str(e.output))

    def __execute_s3m2gbt_command(self, output_tag, output_file_path):
        import io
        from s3m2gbt import s3m2gbt

        sys.stdout = io.StringIO()

        try:
            s3m2gbt.convert_file(self.__file_path, output_tag, output_file_path, False)
            sys.stdout = sys.__stdout__
        except Exception:
            sys.stdout = sys.__stdout__
            raise

    @staticmethod
    def __move_output_file(output_file_name, output_file_path):
        if os.path.exists(output_file_path):
            os.remove(output_file_path)

        os.rename(output_file_name, output_file_path)

    def __write_header(self, build_folder_path, output_tag):
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
            header_file.write('    constexpr inline dmg_music_item ' + name + '(*' + output_tag + ');' + '\n')
            header_file.write('}' + '\n')
            header_file.write('\n')
            header_file.write('#endif' + '\n')
            header_file.write('\n')

        return header_file_path


class DmgAudioFileInfoProcessor:

    def __init__(self, mod2gbt_file_path, build_folder_path):
        self.__mod2gbt_file_path = mod2gbt_file_path
        self.__build_folder_path = build_folder_path

    def __call__(self, audio_file_info):
        return audio_file_info.process(self.__mod2gbt_file_path, self.__build_folder_path)


def list_dmg_audio_file_infos(audio_folder_paths, build_folder_path):
    audio_folder_path_list = audio_folder_paths.split(' ')
    audio_file_infos = []
    file_names_set = set()
    mods_found = False

    for audio_folder_path in audio_folder_path_list:
        audio_file_names = os.listdir(audio_folder_path)

        for audio_file_name in audio_file_names:
            audio_file_path = audio_folder_path + '/' + audio_file_name

            if os.path.isfile(audio_file_path) and FileInfo.validate(audio_file_name):
                audio_file_name_split = os.path.splitext(audio_file_name)
                audio_file_name_ext = audio_file_name_split[1]
                mod_extension = audio_file_name_ext == '.mod'

                if mod_extension or audio_file_name_ext == '.s3m':
                    audio_file_name_no_ext = audio_file_name_split[0]

                    if audio_file_name_no_ext in file_names_set:
                        raise ValueError('There\'s two or more DMG audio files with the same name: ' +
                                         audio_file_name_no_ext)

                    file_names_set.add(audio_file_name_no_ext)
                    file_info_path = build_folder_path + '/_bn_' + audio_file_name_no_ext + '_dmg_audio_file_info.txt'

                    if not os.path.exists(file_info_path):
                        build = True
                    else:
                        file_info_mtime = os.path.getmtime(file_info_path)
                        audio_file_mtime = os.path.getmtime(audio_file_path)
                        build = file_info_mtime < audio_file_mtime

                    if build:
                        audio_file_infos.append(DmgAudioFileInfo(
                            audio_file_path, audio_file_name, audio_file_name_no_ext, file_info_path, mod_extension))
                        
                        if mod_extension:
                            mods_found = True

    return audio_file_infos, mods_found


def process_dmg_audio(mod2gbt_file_path, audio_folder_paths, build_folder_path):
    if len(audio_folder_paths) == 0:
        return

    audio_file_infos, mods_found = list_dmg_audio_file_infos(audio_folder_paths, build_folder_path)

    if len(audio_file_infos) > 0:
        for audio_file_info in audio_file_infos:
            audio_file_info.print_file_name()

        sys.stdout.flush()

        if mods_found:
            if len(mod2gbt_file_path) == 0:
                raise ValueError('mod2gbt file path not specified. You must specify it in the MOD2GBT variable '
                                 'of your project\'s Makefile.')

            if not os.path.exists(mod2gbt_file_path):
                raise ValueError('mod2gbt file does not exist: ' + mod2gbt_file_path)

        pool = Pool()
        processor = DmgAudioFileInfoProcessor(mod2gbt_file_path, build_folder_path)
        process_results = pool.map(processor, audio_file_infos)
        pool.close()

        process_excs = []

        for process_result in process_results:
            if len(process_result) == 3:
                print('    ' + str(process_result[0]) + ' item header written in ' + str(process_result[1]))
            else:
                process_excs.append(process_result)

        sys.stdout.flush()

        if len(process_excs) > 0:
            for process_exc in process_excs:
                sys.stderr.write(str(process_exc[0]) + ' error: ' + str(process_exc[1]) + '\n')

            exit(-1)
