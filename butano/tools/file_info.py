"""
Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
MIT License, see LICENSE file.
"""

import os


class FileInfo:

    @staticmethod
    def read(file_path):
        info = ''

        if os.path.isfile(file_path):
            with open(file_path, 'r') as file:
                info = file.read()

        return FileInfo(info)

    @staticmethod
    def build_from_file(file_path):
        return FileInfo.build_from_files([file_path])

    @staticmethod
    def build_from_files(file_paths):
        info = []

        for file_path in file_paths:
            info.append(file_path)
            info.append(str(os.path.getsize(file_path)))
            info.append(str(os.path.getmtime(file_path)))

        return FileInfo('\n'.join(info))

    def __init__(self, info):
        self.__info = info

    def write(self, file_path):
        with open(file_path, 'w') as file:
            file.write(self.__info)

    def __eq__(self, other):
        return self.__info == other.__info

    def __ne__(self, other):
        return self.__info != other.__info
