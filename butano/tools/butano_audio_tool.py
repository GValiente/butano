"""
Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
zlib License, see LICENSE file.
"""

import os
import sys

from file_info import FileInfo


def list_audio_files(audio_paths):
    temp_audio_file_paths = []

    for audio_path in audio_paths.split(' '):
        if os.path.isdir(audio_path):
            audio_file_names = os.listdir(audio_path)

            for audio_file_name in audio_file_names:
                audio_file_path = audio_path + '/' + audio_file_name

                if os.path.isfile(audio_file_path):
                    temp_audio_file_paths.append(audio_file_path)
        elif os.path.isfile(audio_path):
            temp_audio_file_paths.append(audio_path)

    audio_file_names = []
    audio_file_names_no_ext = []
    audio_file_paths = []

    for audio_file_path in sorted(temp_audio_file_paths):
        audio_file_name = os.path.basename(audio_file_path)

        if FileInfo.validate(audio_file_name):
            audio_file_name_split = os.path.splitext(audio_file_name)
            audio_file_name_no_ext = audio_file_name_split[0]
            audio_file_names.append(audio_file_name)
            audio_file_names_no_ext.append(audio_file_name_no_ext)
            audio_file_paths.append(audio_file_path)

    return audio_file_names, audio_file_names_no_ext, audio_file_paths


def process_audio(backend, tool, audio_paths, build_folder_path):
    maxmod = False
    aas = False

    if backend == 'maxmod':
        maxmod = True
    elif backend == 'aas':
        aas = True
    else:
        return

    audio_file_names, audio_file_names_no_ext, audio_file_paths = list_audio_files(audio_paths)
    file_info_path = build_folder_path + '/_bn_' + backend + '_audio_files_info.txt'
    old_file_info = FileInfo.read(file_info_path)
    new_file_info = FileInfo.build_from_files(audio_file_paths)

    if old_file_info == new_file_info:
        return

    for audio_file_name in audio_file_names:
        print(audio_file_name)

    sys.stdout.flush()

    if maxmod:
        from butano_maxmod_tool import process_maxmod_audio
        process_maxmod_audio(tool, audio_file_paths, audio_file_names_no_ext, build_folder_path)

    if aas:
        from butano_aas_tool import process_aas_audio
        process_aas_audio(tool, audio_file_paths, audio_file_names_no_ext, build_folder_path)

    new_file_info.write(file_info_path)
