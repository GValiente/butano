import argparse
import sys
import traceback


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Butano awful tool.')
    parser.add_argument('--file', required=True, help='c file path')

    try:
        args = parser.parse_args()
        file_path = args.file

        with open(file_path, 'r') as read_file:
            data = read_file.read()

        data = data.replace('#include <wonderful.h>', '')

        with open(file_path, 'w') as write_file:
            write_file.write(data)
    except Exception as ex:
        sys.stderr.write('Error: ' + str(ex) + '\n')
        traceback.print_exc()
        exit(-1)
