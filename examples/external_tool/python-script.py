"""
Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
zlib License, see LICENSE file.
"""

import os
import datetime
import argparse


def process(build_folder_path):
    print('External tool example')
    print('Build folder path: ' + build_folder_path)

    include_folder_path = os.path.join(build_folder_path, 'include')
    print('Include folder path: ' + include_folder_path)

    src_folder_path = os.path.join(build_folder_path, 'src')
    print('Source folder path: ' + src_folder_path)

    if not os.path.exists(include_folder_path):
        os.makedirs(include_folder_path)

    if not os.path.exists(src_folder_path):
        os.makedirs(src_folder_path)

    output_header_path = os.path.join(include_folder_path, 'external_tool_file.h')
    print('Output header file path: ' + output_header_path)

    with open(output_header_path, 'w') as output_header:
        output_header.write('#ifndef EXTERNAL_TOOL_FILE_H' + '\n')
        output_header.write('#define EXTERNAL_TOOL_FILE_H' + '\n')
        output_header.write('\n')
        output_header.write('namespace etf' + '\n')
        output_header.write('{' + '\n')
        output_header.write('    extern const char* current_date;' + '\n')
        output_header.write('}' + '\n')
        output_header.write('\n')
        output_header.write('#endif' + '\n')
        output_header.write('\n')

    print('Output header file written')

    current_dt = datetime.datetime.now().strftime("%A, %d %b %Y")
    print('Current date: ' + current_dt)

    output_cpp_path = os.path.join(src_folder_path, 'external_tool_file.cpp')
    print('Output source file path: ' + output_cpp_path)

    with open(output_cpp_path, 'w') as output_cpp:
        output_cpp.write('#include "external_tool_file.h"' + '\n')
        output_cpp.write('\n')
        output_cpp.write('namespace etf' + '\n')
        output_cpp.write('{' + '\n')
        output_cpp.write('    const char* current_date = "' + current_dt + '";' + '\n')
        output_cpp.write('}' + '\n')
        output_cpp.write('\n')

    print('Output source file written')


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='External tool example.')
    parser.add_argument('--build', required=True, help='build folder path')
    args = parser.parse_args()
    process(args.build)
