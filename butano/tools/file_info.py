"""
Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
zlib License, see LICENSE file.
"""

import os
import string


class FileInfo:
    valid_characters = '_.%s%s' % (string.ascii_lowercase, string.digits)
    cpp_keywords = {'alignas', 'alignof', 'and', 'and_eq', 'asm', 'auto', 'bitand', 'bitor', 'bool', 'break', 'case',
                    'catch', 'char', 'char8_t', 'char16_t', 'char32_t', 'class', 'compl', 'concept ', 'const',
                    'consteval', 'constexpr', 'constinit', 'const_cast', 'continue', 'co_await', 'co_return',
                    'co_yield', 'decltype', 'default', 'delete', 'do', 'double', 'dynamic_cast', 'else', 'enum',
                    'explicit', 'export', 'extern', 'false', 'float', 'for', 'friend', 'goto', 'if', 'inline', 'int',
                    'long', 'mutable', 'namespace', 'new', 'noexcept', 'not', 'not_eq', 'nullptr', 'operator', 'or',
                    'or_eq', 'private', 'protected', 'public', 'register', 'reinterpret_cast', 'requires', 'return',
                    'short', 'signed', 'sizeof', 'static', 'static_assert', 'static_cast', 'struct', 'switch',
                    'template', 'this', 'thread_local', 'throw', 'true', 'try'}

    @staticmethod
    def validate(file_name):
        if file_name[0] == '.':
            return False

        if file_name[0] not in string.ascii_lowercase:
            raise ValueError('Invalid file name: ' + file_name + ' (invalid character: \'' + file_name[0] + '\')')

        file_name_parts = file_name.split('.')

        if len(file_name_parts) != 2:
            raise ValueError('Invalid file name: ' + file_name + ' (one and only one dot required)')

        for file_name_character in file_name:
            if file_name_character not in FileInfo.valid_characters:
                raise ValueError('Invalid file name: ' + file_name +
                                 ' (invalid character: \'' + file_name_character + '\')')

        if file_name_parts[0] in FileInfo.cpp_keywords:
            raise ValueError('File name is a C++ keyword: ' + file_name)

        return True

    @staticmethod
    def read(file_path):
        info = ''
        read_failed = True

        if os.path.isfile(file_path):
            with open(file_path, 'r') as file:
                info = file.read()
                read_failed = False

        return FileInfo(info, read_failed)

    @staticmethod
    def build_from_files(file_paths):
        info = []

        for file_path in file_paths:
            info.append(file_path)
            info.append(str(os.path.getmtime(file_path)))

        return FileInfo('\n'.join(info), False)

    def __init__(self, info, read_failed):
        self.__info = info
        self.__read_failed = read_failed

    def write(self, file_path):
        with open(file_path, 'w') as file:
            file.write(self.__info)

    def __eq__(self, other):
        return self.__info == other.__info and self.__read_failed == other.__read_failed

    def __ne__(self, other):
        return self.__info != other.__info or self.__read_failed != other.__read_failed

    def __repr__(self):
        if self.__read_failed:
            return '[read failed]'

        return self.__info
