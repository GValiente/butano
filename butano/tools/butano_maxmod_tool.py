"""
Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
zlib License, see LICENSE file.
"""

import os
import subprocess


def process_audio_files(tool, audio_file_paths, soundbank_bin_path, soundbank_header_path, build_folder_path):
    command = [tool]

    if not audio_file_paths:
        dummy_file_path = build_folder_path + '/_bn_dummy_audio_file.txt'
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
        tool_output = subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
    except subprocess.CalledProcessError as e:
        raise ValueError(tool + ' call failed (return code ' + str(e.returncode) + '): ' + str(e.output))

    return tool_output


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
                output_file.write('    constexpr inline ' + item_class + ' ' + item[0] + '(' + item[1] + ');' + '\n')

            output_file.write('}' + '\n')
            output_file.write('\n')
            output_file.write('#endif' + '\n')
            output_file.write('\n')

        print('    ' + item_class + 's file written in ' + output_file_path)
    else:
        if os.path.exists(output_file_path):
            os.remove(output_file_path)


def write_output_info_file(items, include_guard, include_file, namespace, item_class, output_file_path):
    with open(output_file_path, 'w') as output_file:
        output_file.write('#ifndef ' + include_guard + '\n')
        output_file.write('#define ' + include_guard + '\n')
        output_file.write('\n')
        output_file.write('#include "bn_span.h"' + '\n')
        output_file.write('#include "' + include_file + '"' + '\n')
        output_file.write('#include "bn_string_view.h"' + '\n')
        output_file.write('\n')
        output_file.write('namespace ' + namespace + '\n')
        output_file.write('{' + '\n')

        pair_class = 'pair<' + item_class + ', string_view>'

        if len(items) > 0:
            output_file.write('    constexpr inline ' + pair_class + ' array[] = {' + '\n')

            for item in items:
                output_file.write('        make_pair(' + item_class + '(' + item[1] +
                                  '), string_view("' + item[0] + '")),' + '\n')

            output_file.write('    };' + '\n')
            output_file.write('\n')
            output_file.write('    constexpr inline span<const ' + pair_class + '> span(array);' + '\n')
        else:
            output_file.write('    constexpr inline span<const ' + pair_class + '> span;' + '\n')

        output_file.write('}' + '\n')
        output_file.write('\n')
        output_file.write('#endif' + '\n')
        output_file.write('\n')

    print('    ' + item_class + 's_info file written in ' + output_file_path)


def write_output_files(audio_file_paths, audio_file_names_no_ext, tool_output, soundbank_header_path,
                       build_folder_path):
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

    audio_file_index = 0

    for audio_file_name_no_ext in audio_file_names_no_ext:
        if audio_file_name_no_ext not in music_final_names_set and audio_file_name_no_ext not in sound_final_names_set:
            audio_file_path = audio_file_paths[audio_file_index]
            raise ValueError(audio_file_path + ' import failed. Tool output: ' + tool_output.decode())

        audio_file_index += 1

    write_output_file(music_items_list, 'BN_MUSIC_ITEMS_H', 'bn_music_item.h', 'bn::music_items', 'music_item',
                      build_folder_path + '/bn_music_items.h')

    write_output_file(sound_items_list, 'BN_SOUND_ITEMS_H', 'bn_sound_item.h', 'bn::sound_items', 'sound_item',
                      build_folder_path + '/bn_sound_items.h')

    write_output_info_file(music_items_list, 'BN_MUSIC_ITEMS_INFO_H', 'bn_music_item.h', 'bn::music_items_info',
                           'music_item', build_folder_path + '/bn_music_items_info.h')

    write_output_info_file(sound_items_list, 'BN_SOUND_ITEMS_INFO_H', 'bn_sound_item.h', 'bn::sound_items_info',
                           'sound_item', build_folder_path + '/bn_sound_items_info.h')


def process_maxmod_audio(tool, audio_file_paths, audio_file_names_no_ext, build_folder_path):
    soundbank_bin_path = build_folder_path + '/bn_audio_soundbank.bin'
    soundbank_header_path = build_folder_path + '/bn_audio_soundbank.h'
    tool_output = process_audio_files(tool, audio_file_paths, soundbank_bin_path, soundbank_header_path,
                                      build_folder_path)
    write_output_files(audio_file_paths, audio_file_names_no_ext, tool_output, soundbank_header_path,
                       build_folder_path)
    print('    Processed audio size: ' + str(os.path.getsize(soundbank_bin_path)) + ' bytes')
    os.remove(soundbank_header_path)
