"""
Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
zlib License, see LICENSE file.
"""

import os
import wave
import shutil
import subprocess

def generate_temp_folder(audio_file_paths, build_folder_path):
    temp_folder_path = build_folder_path + '/aas'

    if os.path.exists(temp_folder_path):
        shutil.rmtree(temp_folder_path)

    os.mkdir(temp_folder_path)

    for audio_file_path in audio_file_paths:
        shutil.copy(audio_file_path, temp_folder_path)

    return temp_folder_path


def process_audio_files(tool, temp_folder_path, soundbank_header_path, soundbank_bin_path):
    command = [tool, temp_folder_path]
    command = ' '.join(command)

    try:
        tool_output = subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT)
    except subprocess.CalledProcessError as e:
        raise ValueError(tool + ' call failed (return code ' + str(e.returncode) + '): ' + str(e.output))

    shutil.move('AAS_Data.h', soundbank_header_path)
    shutil.move('AAS_Data.s', soundbank_bin_path)
    return tool_output


def write_info_table_file(music_items, sound_items, sound_item_frequencies, output_file_path):
    with open(output_file_path, 'w') as output_file:
        output_file.write('#include <stdint.h>' + '\n')
        output_file.write('\n')

        for music_item in music_items:
            output_file.write('extern const uint8_t AAS_DATA_MOD_' + music_item + ';' + '\n')

        for sound_item in sound_items:
            output_file.write('extern const int8_t* const AAS_DATA_SFX_START_' + sound_item + ';' + '\n')
            output_file.write('extern const int8_t* const AAS_DATA_SFX_END_' + sound_item + ';' + '\n')

        output_file.write('\n')

        output_file.write('const uint8_t* bn_aas_music_ids[] =' + '\n')
        output_file.write('{' + '\n')

        if len(music_items) == 0:
            output_file.write('    0,' + '\n')
        else:
            for music_item in music_items:
                output_file.write('    &AAS_DATA_MOD_' + music_item + ',' + '\n')

        output_file.write('};' + '\n')
        output_file.write('\n')

        output_file.write('const int8_t* const* bn_aas_sound_starts[] =' + '\n')
        output_file.write('{' + '\n')

        if len(sound_items) == 0:
            output_file.write('    0,' + '\n')
        else:
            for sound_item in sound_items:
                output_file.write('    &AAS_DATA_SFX_START_' + sound_item + ',' + '\n')

        output_file.write('};' + '\n')
        output_file.write('\n')

        output_file.write('const int8_t* const* bn_aas_sound_ends[] =' + '\n')
        output_file.write('{' + '\n')

        if len(sound_items) == 0:
            output_file.write('    0,' + '\n')
        else:
            for sound_item in sound_items:
                output_file.write('    &AAS_DATA_SFX_END_' + sound_item + ',' + '\n')

        output_file.write('};' + '\n')
        output_file.write('\n')

        output_file.write('const int bn_aas_sound_frequencies[] =' + '\n')
        output_file.write('{' + '\n')

        if len(sound_item_frequencies) == 0:
            output_file.write('    0,' + '\n')
        else:
            for sound_item_frequency in sound_item_frequencies:
                output_file.write('    ' + str(sound_item_frequency) + ',' + '\n')

        output_file.write('};' + '\n')


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

            for index, item in enumerate(items):
                output_file.write('    constexpr inline ' + item_class + ' ' + item + '(' + str(index) + ');' + '\n')

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

            for index, item in enumerate(items):
                output_file.write('        make_pair(' + item_class + '(' + str(index) +
                                  '), string_view("' + item + '")),' + '\n')

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


def write_output_files(audio_file_paths, audio_file_names_no_ext, tool_output, soundbank_header_path, temp_folder_path,
                       build_folder_path):
    music_items_list = []
    sound_items_list = []
    sound_item_frequencies = []
    music_final_names_set = set()
    sound_final_names_set = set()

    with open(soundbank_header_path, 'r') as soundbank_file:
        for soundbank_line in soundbank_file:
            if soundbank_line.startswith('extern const AAS_u8 AAS_DATA_MOD_'):
                final_name = soundbank_line[len('extern const AAS_u8 AAS_DATA_MOD_'):-2]

                if final_name not in audio_file_names_no_ext:
                    print('    Music item not present in files. Skipped: ' + final_name)
                else:
                    if final_name in music_final_names_set:
                        raise ValueError('There\'s two or more music items with the same name: ' + final_name)

                    music_final_names_set.add(final_name)
                    music_items_list.append(final_name)
            elif soundbank_line.startswith('extern const AAS_s8* const AAS_DATA_SFX_START_'):
                final_name = soundbank_line[len('extern const AAS_s8* const AAS_DATA_SFX_START_'):-2]

                if final_name not in audio_file_names_no_ext:
                    print('    Sound item not present in files. Skipped: ' + final_name)
                else:
                    if final_name in sound_final_names_set:
                        raise ValueError('There\'s two or more sound items with the same name: ' + final_name)

                    with wave.open(temp_folder_path + '/' + final_name + '.wav') as wav_file:
                        sound_final_names_set.add(final_name)
                        sound_items_list.append(final_name)
                        sound_item_frequencies.append(wav_file.getframerate())

    audio_file_index = 0

    for audio_file_name_no_ext in audio_file_names_no_ext:
        if audio_file_name_no_ext not in music_final_names_set and audio_file_name_no_ext not in sound_final_names_set:
            audio_file_path = audio_file_paths[audio_file_index]
            raise ValueError(audio_file_path + ' import failed. Tool output: ' + tool_output.decode())

        audio_file_index += 1

    write_info_table_file(music_items_list, sound_items_list, sound_item_frequencies,
                          build_folder_path + '/bn_aas_info.c')

    write_output_file(music_items_list, 'BN_MUSIC_ITEMS_H', 'bn_music_item.h', 'bn::music_items', 'music_item',
                      build_folder_path + '/bn_music_items.h')

    write_output_file(sound_items_list, 'BN_SOUND_ITEMS_H', 'bn_sound_item.h', 'bn::sound_items', 'sound_item',
                      build_folder_path + '/bn_sound_items.h')

    write_output_info_file(music_items_list, 'BN_MUSIC_ITEMS_INFO_H', 'bn_music_item.h', 'bn::music_items_info',
                           'music_item', build_folder_path + '/bn_music_items_info.h')

    write_output_info_file(sound_items_list, 'BN_SOUND_ITEMS_INFO_H', 'bn_sound_item.h', 'bn::sound_items_info',
                           'sound_item', build_folder_path + '/bn_sound_items_info.h')


def aas_audio_file_name_exts():
    return ['.mod', '.wav']


def process_aas_audio(tool, audio_file_paths, audio_file_names_no_ext, build_folder_path):
    soundbank_header_path = build_folder_path + '/AAS_Data.h'
    soundbank_bin_path = build_folder_path + '/AAS_Data.s'
    temp_folder_path = generate_temp_folder(audio_file_paths, build_folder_path)
    tool_output = process_audio_files(tool, temp_folder_path, soundbank_header_path, soundbank_bin_path)
    write_output_files(audio_file_paths, audio_file_names_no_ext, tool_output, soundbank_header_path, temp_folder_path,
                       build_folder_path)
    os.remove(soundbank_header_path)
