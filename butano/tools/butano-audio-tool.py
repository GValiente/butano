"""
Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
zlib License, see LICENSE file.
"""

import os
import argparse
import subprocess
import sys
import traceback

from file_info import FileInfo


def list_audio_files(audio_folder_paths):
    audio_folder_path_list = audio_folder_paths.split(' ')
    audio_file_names = []
    audio_file_names_no_ext = []
    audio_file_paths = []

    for audio_folder_path in audio_folder_path_list:
        folder_audio_file_names = sorted(os.listdir(audio_folder_path))

        for audio_file_name in folder_audio_file_names:
            if FileInfo.validate(audio_file_name):
                audio_file_path = audio_folder_path + '/' + audio_file_name

                if os.path.isfile(audio_file_path):
                    audio_file_name_split = os.path.splitext(audio_file_name)
                    audio_file_name_no_ext = audio_file_name_split[0]
                    audio_file_names.append(audio_file_name)
                    audio_file_names_no_ext.append(audio_file_name_no_ext)
                    audio_file_paths.append(audio_file_path)

    return audio_file_names, audio_file_names_no_ext, audio_file_paths


def process_audio_files(audio_file_paths, soundbank_bin_path, soundbank_header_path, build_folder_path):
    command = ['mmutil']

    if not audio_file_paths:
        dummy_file_path = build_folder_path + '/_btn_dummy_audio_file.txt'
        command.append(dummy_file_path)

        with open(dummy_file_path, 'w') as dummy_file:
            dummy_file.write('')
    else:
        for audio_file_path in audio_file_paths:
            command.append(audio_file_path)

    command.append('-o' + soundbank_bin_path)
    command.append('-h' + soundbank_header_path)
    command = ' '.join(command)

    try:
        subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
    except subprocess.CalledProcessError as e:
        raise ValueError('mmutil call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


def write_output_file(items, include_guard, include_file, namespace, item_class, output_file_path):
    if len(items) > 0:
        with open(output_file_path, 'w') as output_file:
            output_file.write('#ifndef ' + include_guard + '\n')
            output_file.write('#define ' + include_guard + '\n')
            output_file.write('\n')
            output_file.write('#include "' + include_file + '"' + '\n')
            output_file.write('\n')
            output_file.write('namespace ' + namespace + '\n')
            output_file.write('{' + '\n')

            for item in items:
                output_file.write('    constexpr const ' + item_class + ' ' + item[0] + '(' + item[1] + ');' + '\n')

            output_file.write('}' + '\n')
            output_file.write('\n')
            output_file.write('#endif' + '\n')
            output_file.write('\n')

        print('    ' + item_class + 's file written in ' + output_file_path)
    else:
        if os.path.exists(output_file_path):
            os.remove(output_file_path)


def write_output_files(audio_file_names_no_ext, soundbank_header_path, build_folder_path):
    music_items_list = []
    sound_items_list = []
    music_final_names_set = set()
    sound_final_names_set = set()

    with open(soundbank_header_path, 'r') as soundbank_file:
        for soundbank_line in soundbank_file:
            soundbank_words = soundbank_line.split()
            soundbank_name = soundbank_words[1]
            final_name = soundbank_name[4:].lower()

            if soundbank_name.startswith('MOD_'):
                if final_name not in audio_file_names_no_ext:
                    print('    Music item not present in files. Skipped: ' + final_name)
                else:
                    if final_name in music_final_names_set:
                        raise ValueError('There\'s two or more music items with the same name: ' + final_name)

                    music_final_names_set.add(final_name)
                    music_items_list.append([final_name, soundbank_words[2]])
            elif soundbank_name.startswith('SFX_'):
                if final_name not in audio_file_names_no_ext:
                    print('    Sound item not present in files. Skipped: ' + final_name)
                else:
                    if final_name in sound_final_names_set:
                        raise ValueError('There\'s two or more sound items with the same name: ' + final_name)

                    sound_final_names_set.add(final_name)
                    sound_items_list.append([final_name, soundbank_words[2]])

    write_output_file(music_items_list, 'BTN_MUSIC_ITEMS_H', 'btn_music_item.h', 'btn::music_items', 'music_item',
                      build_folder_path + '/btn_music_items.h')

    write_output_file(sound_items_list, 'BTN_SOUND_ITEMS_H', 'btn_sound_item.h', 'btn::sound_items', 'sound_item',
                      build_folder_path + '/btn_sound_items.h')


def process(audio_folder_paths, build_folder_path):
    audio_file_names, audio_file_names_no_ext, audio_file_paths = list_audio_files(audio_folder_paths)
    file_info_path = build_folder_path + '/_btn_audio_files_info.txt'
    old_file_info = FileInfo.read(file_info_path)
    new_file_info = FileInfo.build_from_files(audio_file_paths)

    if old_file_info == new_file_info:
        return

    for audio_file_name in audio_file_names:
        print(audio_file_name)

    soundbank_bin_path = build_folder_path + '/_btn_audio_soundbank.bin'
    soundbank_header_path = build_folder_path + '/_btn_audio_soundbank.h'
    process_audio_files(audio_file_paths, soundbank_bin_path, soundbank_header_path, build_folder_path)
    write_output_files(audio_file_names_no_ext, soundbank_header_path, build_folder_path)
    os.remove(soundbank_header_path)
    new_file_info.write(file_info_path)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='butano audio tool.')
    parser.add_argument('--audio', required=True, help='audio folder paths')
    parser.add_argument('--build', required=True, help='build folder path')

    try:
        args = parser.parse_args()
        process(args.audio, args.build)
    except Exception as ex:
        sys.stderr.write('Error: ' + str(ex) + '\n')
        traceback.print_exc()
        exit(-1)
