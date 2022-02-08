import argparse
import sys
import traceback

from butano_audio_tool import process_audio
from butano_graphics_tool import process_graphics
from importlib import import_module


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Butano assets tool.')
    parser.add_argument('--audio', required=True, help='audio folder paths')
    parser.add_argument('--graphics', required=True, help='graphics folder paths')
    parser.add_argument('--fonts', required=False, help='fonts folder paths')
    parser.add_argument('--build', required=True, help='build folder path')

    try:
        args = parser.parse_args()
        process_audio(args.audio, args.build)
        if len(args.fonts) > 0:
            process_fonts = getattr(import_module('butano_fonts_tool'), 'process_fonts')
            process_fonts(args.fonts, args.build)
        process_graphics(args.graphics, args.build)
    except Exception as ex:
        sys.stderr.write('Error: ' + str(ex) + '\n')
        traceback.print_exc()
        exit(-1)
