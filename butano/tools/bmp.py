"""
Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
zlib License, see LICENSE file.
"""

import shutil
import struct


class BMP:

    def __init__(self, file_path):
        self.width = None
        self.height = None
        self.colors_count = None
        self.__file_path = file_path

        with open(file_path, 'rb') as file:
            def read_int():
                return struct.unpack('I', file.read(4))[0]

            def read_short():
                return struct.unpack('H', file.read(2))[0]

            file.read(10)
            self.__pixels_offset = read_int()
            header_size = read_int()

            if header_size == 108:
                raise ValueError('Invalid header size: ' + str(header_size) +
                                 ' (BMP files with color space information are not supported)')

            if header_size != 40:
                raise ValueError('Invalid header size: ' + str(header_size))

            self.width = read_int()

            if self.width == 0 or self.width % 8 != 0:
                raise ValueError('Invalid width: ' + str(self.width))

            self.height = read_int()

            if self.height == 0 or self.height % 8 != 0:
                raise ValueError('Invalid height: ' + str(self.height))

            file.read(2)
            bits_per_pixel = read_short()

            if bits_per_pixel != 4 and bits_per_pixel != 8:
                raise ValueError('Invalid bits per pixel: ' + str(bits_per_pixel))

            compression_method = read_int()

            if compression_method != 0:
                raise ValueError('Compression method not supported: ' + str(compression_method))

            file.read(20)
            self.__colors_offset = file.tell()

            if bits_per_pixel == 4:
                colors_count = 16
            else:
                colors_count = int((self.__pixels_offset - self.__colors_offset) / 4)

                if colors_count < 1:
                    raise ValueError('Invalid input colors count: ' + str(colors_count))

                if colors_count <= 16:
                    colors_count = 16
                else:
                    palette_colors_count = max(colors_count, 256)
                    self.__colors = struct.unpack(str(palette_colors_count) + 'I', file.read(palette_colors_count * 4))

                    file.seek(self.__pixels_offset)
                    pixels_count = self.width * self.height  # no padding, multiple of 8.
                    self.__pixels = [ord(pixel) for pixel in
                                     struct.unpack(str(pixels_count) + 'c', file.read(pixels_count))]

                    colors_count = max(self.__pixels) + 1

                    if colors_count > palette_colors_count:
                        raise ValueError('Not enough palette colors: ' + str(colors_count) + ' - ' +
                                         str(palette_colors_count))

                    extra_colors = colors_count % 16

                    if extra_colors > 0:
                        colors_count += 16 - extra_colors

                    if colors_count > 256:
                        raise ValueError('Invalid calculated colors count: ' + str(colors_count))

            self.colors_count = colors_count

    def quantize(self, output_file_path):
        if self.colors_count == 16:
            shutil.copyfile(self.__file_path, output_file_path)
            return 16

        palette_colors_count = int((self.__pixels_offset - self.__colors_offset) / 4)

        if palette_colors_count < 256:
            raise ValueError('Quantization is not supported is the input image has less than 256 colors: ' +
                             str(palette_colors_count))

        width = self.width
        height = self.height
        colors = self.__colors
        pixels = self.__pixels

        # Store used pixels for all tiles:
        tile_pixel_sets = []

        for ty in range(0, height, 8):
            for tx in range(0, width, 8):
                new_tile_pixel_set = set()

                for y in range(ty, ty + 8):
                    row = width * y

                    for x in range(tx, tx + 8):
                        pixel = pixels[row + x]

                        if pixel > 0:
                            new_tile_pixel_set.add(pixel)

                new_tile_pixel_set_len = len(new_tile_pixel_set)

                if new_tile_pixel_set_len > 0:
                    if new_tile_pixel_set_len > 15:
                        raise ValueError('There\'s a tile with more than 15 colors: ' + str(tx) + ' - ' +
                                         str(height - ty - 8) + ' - ' + str(new_tile_pixel_set_len) + ': ' +
                                         str(new_tile_pixel_set))

                    append = True

                    for tile_pixel_set in tile_pixel_sets:
                        if new_tile_pixel_set.issubset(tile_pixel_set):
                            append = False
                            break
                        elif tile_pixel_set.issubset(new_tile_pixel_set):
                            tile_pixel_set.update(new_tile_pixel_set)
                            append = False
                            break

                    if append:
                        tile_pixel_sets.append(new_tile_pixel_set)

        tile_pixel_sets_count = len(tile_pixel_sets)

        if tile_pixel_sets_count == 0:
            shutil.copyfile(self.__file_path, output_file_path)
            return 16

        # Merge pixel sets:
        while True:
            best_i = None
            best_j = None
            best_u_set = None
            best_quality = -1000
            best_erased_tile_pixel_sets = 0
            i = 0
            merged = False

            while i < tile_pixel_sets_count - 1:
                i_set = tile_pixel_sets[i]
                i_set_length = len(i_set)
                j = i + 1

                while j < tile_pixel_sets_count:
                    j_set = tile_pixel_sets[j]
                    j_set_length = len(j_set)
                    u_set = i_set.union(j_set)
                    u_set_length = len(u_set)

                    if u_set_length == i_set_length or u_set_length == j_set_length:
                        if u_set_length == j_set_length:
                            tile_pixel_sets[i] = j_set
                            i_set = j_set
                            i_set_length = j_set_length

                        tile_pixel_sets.pop(j)
                        j -= 1
                        tile_pixel_sets_count -= 1

                        best_u_set = None
                        merged = True
                    elif not merged and len(u_set) < 16:
                        quality = 0

                        for pixel in u_set:
                            if pixel in i_set and pixel in j_set:
                                quality += 1
                            else:
                                quality -= 1

                        if quality > best_quality:
                            best_quality = quality
                            best_erased_tile_pixel_sets = BMP._count_erased_tile_pixel_sets(tile_pixel_sets, u_set)
                            best_u_set = u_set
                            best_i = i
                            best_j = j
                        elif quality == best_quality:
                            erased_tile_pixel_sets = BMP._count_erased_tile_pixel_sets(tile_pixel_sets, u_set)

                            if erased_tile_pixel_sets > best_erased_tile_pixel_sets:
                                best_erased_tile_pixel_sets = erased_tile_pixel_sets
                                best_u_set = u_set
                                best_i = i
                                best_j = j

                    j += 1

                i += 1

            if best_u_set is not None:
                if best_i < best_j:
                    best_i, best_j = best_j, best_i

                tile_pixel_sets[best_i] = best_u_set
                tile_pixel_sets.pop(best_j)
                tile_pixel_sets_count -= 1
            elif not merged:
                break

        if tile_pixel_sets_count > 16:
            raise ValueError('There\'s more than 16 4bpp palettes: ' + str(tile_pixel_sets_count))

        # Generate new colors:
        transparent_color = colors[0]

        for i in range(tile_pixel_sets_count):
            for i_pixel in tile_pixel_sets[i]:
                i_color = colors[i_pixel]

                if i_color == transparent_color:
                    raise ValueError('There\'s an used color like the transparent one in: ' + str(i_pixel))

                for j in range(i + 1, tile_pixel_sets_count):
                    for j_pixel in tile_pixel_sets[j]:
                        if i_pixel != j_pixel and i_color == colors[j_pixel]:
                            raise ValueError('There\'s two used identical colors in: ' + str(i_pixel) + ' and ' +
                                             str(j_pixel))

        new_colors = [transparent_color] * 256

        for tpi in range(tile_pixel_sets_count):
            new_tile_pixel_set = tile_pixel_sets[tpi]
            ci = (tpi * 16) + 1

            for tile_pixel in new_tile_pixel_set:
                new_colors[ci] = colors[tile_pixel]
                ci += 1

        # Generate new pixels:
        new_pixels = list(pixels)
        tile_pixel_lists = []

        for new_tile_pixel_set in tile_pixel_sets:
            tile_pixel_lists.append(list(new_tile_pixel_set))

        for ty in range(0, height, 8):
            for tx in range(0, width, 8):
                valid_tile_pixel_set = False

                for tpi in range(tile_pixel_sets_count):
                    new_tile_pixel_set = tile_pixel_sets[tpi]
                    valid_tile_pixel_set = True

                    for y in range(ty, ty + 8):
                        row = width * y
    
                        for x in range(tx, tx + 8):
                            pixel = pixels[row + x]
    
                            if pixel > 0:
                                if pixel not in new_tile_pixel_set:
                                    valid_tile_pixel_set = False
                                    break

                        if not valid_tile_pixel_set:
                            break

                    if valid_tile_pixel_set:
                        tile_pixel_list = tile_pixel_lists[tpi]

                        for y in range(ty, ty + 8):
                            row = width * y

                            for x in range(tx, tx + 8):
                                pixel = pixels[row + x]
                                new_pixel = tpi * 16

                                if pixel > 0:
                                    new_pixel += tile_pixel_list.index(pixel) + 1

                                new_pixels[row + x] = new_pixel

                        break

                if not valid_tile_pixel_set:
                    raise ValueError('No valid palette found for tile: ' + str(tx) + ' - ' + str(height - ty - 8))

        # Write output file:
        with open(self.__file_path, 'rb') as input_file:
            input_file_content = input_file.read()
            b = bytearray()
            b.extend(input_file_content)
            input_file_content = b
            offset = self.__colors_offset

            for new_color in new_colors:
                new_color_bytes = struct.unpack('4B', struct.pack('I', new_color))

                for new_color_byte in new_color_bytes:
                    input_file_content[offset] = new_color_byte
                    offset += 1

            offset = self.__pixels_offset

            for new_pixel in new_pixels:
                input_file_content[offset] = new_pixel
                offset += 1

            with open(output_file_path, 'wb') as output_file:
                output_file.write(input_file_content)

        return tile_pixel_sets_count * 16

    @staticmethod
    def _count_erased_tile_pixel_sets(tile_pixel_sets, u_set):
        tile_pixel_sets_count = len(tile_pixel_sets)
        result = 0
        i = 0

        while i < tile_pixel_sets_count - 1:
            i_set = tile_pixel_sets[i]

            if i_set.issubset(u_set):
                j = i + 1

                while j < tile_pixel_sets_count:
                    j_set = tile_pixel_sets[j]

                    if j_set.issubset(u_set):
                        result += 1

                    j += 1

            i += 1

        return result
