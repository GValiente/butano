"""
Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
zlib License, see LICENSE file.
"""

import os
import datetime
import argparse


def process(build_folder_path):
    print('External tool example')
    print('Build folder path: ' + build_folder_path)

    output_file_path = os.path.join(build_folder_path, 'external_tool_file.h')
    print('Output file path: ' + output_file_path)

    current_dt = datetime.datetime.now().strftime("%A, %d %b %Y")
    print('Current date: ' + current_dt)

    with open(output_file_path, 'w') as output_file:
        output_file.write('#ifndef EXTERNAL_TOOL_FILE_H' + '\n')
        output_file.write('#define EXTERNAL_TOOL_FILE_H' + '\n')
        output_file.write('\n')
        output_file.write('namespace etf' + '\n')
        output_file.write('{' + '\n')
        output_file.write('    constexpr const char* current_date = "' + current_dt + '";' + '\n')
        output_file.write('}' + '\n')
        output_file.write('\n')
        output_file.write('#endif' + '\n')
        output_file.write('\n')

    print('Output file written')


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='External tool example.')
    parser.add_argument('--build', required=True, help='build folder path')
    args = parser.parse_args()
    process(args.build)
