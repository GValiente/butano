import argparse
import sys
import traceback

from butano_audio_tool import process_audio
from butano_graphics_tool import process_graphics


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Butano assets tool.')
    parser.add_argument('--audio', required=True, help='audio folder paths')
    parser.add_argument('--graphics', required=True, help='graphics folder paths')
    parser.add_argument('--build', required=True, help='build folder path')

    try:
        args = parser.parse_args()
        process_audio(args.audio, args.build)
        process_graphics(args.graphics, args.build)
    except Exception as ex:
        sys.stderr.write('Error: ' + str(ex) + '\n')
        traceback.print_exc()
        exit(-1)
