"""
Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
zlib License, see LICENSE file.
"""

import argparse

from bmp import BMP


def process(input_file_path, output_file_path):
    bmp = BMP(input_file_path)
    bmp.quantize(output_file_path)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Butano BMP quantizer.')
    parser.add_argument('--input', required=True, help='input file path')
    parser.add_argument('--output', required=True, help='output file path')
    args = parser.parse_args()
    process(args.input, args.output)
