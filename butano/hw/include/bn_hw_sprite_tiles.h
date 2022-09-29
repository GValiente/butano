/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_SPRITE_TILES_H
#define BN_HW_SPRITE_TILES_H

#include "bn_tile.h"
#include "bn_assert.h"
#include "bn_compression_type.h"
#include "bn_hw_dma.h"
#include "bn_hw_memory.h"
#include "bn_hw_decompress.h"

namespace bn::hw::sprite_tiles
{
    namespace
    {
        static_assert(sizeof(TILE) == sizeof(tile));
        static_assert(alignof(TILE) == alignof(tile));

        [[nodiscard]] inline tile* tile_vram(int index)
        {
            return reinterpret_cast<tile*>(MEM_VRAM_OBJ) + index;
        }
    }

    inline void copy_tiles(const tile* source_tiles_ptr, int count, tile* destination_tiles_ptr)
    {
        hw::memory::copy_words(source_tiles_ptr, count * int(sizeof(tile) / 4), destination_tiles_ptr);
    }

    inline void clear_tiles(int count, tile* tiles_ptr)
    {
        hw::memory::set_words(0, count * int(sizeof(tile) / 4), tiles_ptr);
    }

    [[nodiscard]] inline tile* vram(int index)
    {
        return tile_vram(index);
    }

    inline void commit_with_cpu(const tile* source_tiles_ptr, int index, int count)
    {
        hw::memory::copy_words(source_tiles_ptr, count * int(sizeof(tile) / 4), tile_vram(index));
    }

    inline void commit_with_dma(const tile* source_tiles_ptr, int index, int count)
    {
        hw::dma::copy_words(source_tiles_ptr, count * int(sizeof(tile) / 4), tile_vram(index));
    }

    inline void commit(const tile* source_tiles_ptr, compression_type compression, int index, int count)
    {
        switch(compression)
        {

        case compression_type::NONE:
            commit_with_cpu(source_tiles_ptr, index, count);
            break;

        case compression_type::LZ77:
            hw::decompress::lz77_vram(source_tiles_ptr, tile_vram(index));
            break;

        case compression_type::RUN_LENGTH:
            hw::decompress::rl_vram(source_tiles_ptr, tile_vram(index));
            break;

        case compression_type::HUFFMAN:
            hw::decompress::huff(source_tiles_ptr, tile_vram(index));
            break;

        default:
            BN_ERROR("Unknown compression type: ", int(compression));
            break;
        }
    }

    void plot_tiles(int width, const tile* source_tiles_ptr, int source_y, int destination_y,
                    tile* destination_tiles_ptr);

    BN_CODE_IWRAM void _plot_hideous_tiles(int width, const unsigned* srcD, int dstX0, unsigned* dstD);
}

#endif
