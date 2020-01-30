import os
import argparse
import subprocess

from file_info import FileInfo


def list_audio_file_paths(audio_folder_paths):
    audio_folder_path_list = audio_folder_paths.split(' ')
    audio_file_paths = []

    for audio_folder_path in audio_folder_path_list:
        audio_file_names = sorted(os.listdir(audio_folder_path))

        for audio_file_name in audio_file_names:
            audio_file_path = audio_folder_path + '/' + audio_file_name

            if os.path.isfile(audio_file_path):
                audio_file_paths.append(audio_file_path)

    return audio_file_paths


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
        subprocess.check_output([command], shell=True, stderr=subprocess.STDOUT)
    except subprocess.CalledProcessError as e:
        raise ValueError('mmutil call failed (return code ' + str(e.returncode) + '): ' + str(e.output))


def write_output_file(items, include_guard, include_file, namespace, item_class, output_file_path):
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

    print(item_class + 's file written in ' + output_file_path)


def write_output_files(soundbank_header_path, build_folder_path):
    music_items = []
    sound_items = []

    with open(soundbank_header_path, 'r') as soundbank_file:
        for soundbank_line in soundbank_file:
            soundbank_words = soundbank_line.split()
            soundbank_name = soundbank_words[1]
            final_name = soundbank_name[4:].lower()

            if soundbank_name.startswith('MOD_'):
                music_items.append([final_name, soundbank_words[2]])
            elif soundbank_name.startswith('SFX_'):
                sound_items.append([final_name, soundbank_words[2]])

    write_output_file(music_items, 'BTN_MUSIC_ITEMS_H', 'btn_music_item.h', 'btn::music_items', 'music_item',
                      build_folder_path + '/btn_music_items.h')

    write_output_file(sound_items, 'BTN_SOUND_ITEMS_H', 'btn_sound_item.h', 'btn::sound_items', 'sound_item',
                      build_folder_path + '/btn_sound_items.h')


def process(audio_folder_paths, build_folder_path):
    audio_file_paths = list_audio_file_paths(audio_folder_paths)
    file_info_path = build_folder_path + '/_btn_file_info.txt'
    old_file_info = FileInfo.read(file_info_path)
    new_file_info = FileInfo.build_from_files(audio_file_paths)

    if old_file_info == new_file_info:
        return

    print('Processing audio files: ' + str(audio_file_paths))
    soundbank_bin_path = build_folder_path + '/_btn_audio_soundbank.bin'
    soundbank_header_path = build_folder_path + '/_btn_audio_soundbank.h'
    process_audio_files(audio_file_paths, soundbank_bin_path, soundbank_header_path, build_folder_path)
    write_output_files(soundbank_header_path, build_folder_path)
    os.remove(soundbank_header_path)
    new_file_info.write(file_info_path)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='butano audio tool.')
    parser.add_argument('--audio', required=True, help='audio folder paths')
    parser.add_argument('--build', required=True, help='build folder path')
    args = parser.parse_args()
    process(args.audio, args.build)
