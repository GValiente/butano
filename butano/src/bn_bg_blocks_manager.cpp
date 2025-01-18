/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_bg_blocks_manager.h"

#include "bn_limits.h"
#include "bn_string_view.h"
#include "bn_bgs_manager.h"
#include "bn_config_bg_blocks.h"
#include "bn_affine_bg_big_map_canvas_size.h"
#include "../hw/include/bn_hw_dma.h"
#include "../hw/include/bn_hw_memory.h"
#include "../hw/include/bn_hw_bg_blocks.h"

#include "bn_bg_maps.cpp.h"
#include "bn_bg_tiles.cpp.h"
#include "bn_regular_bg_map_ptr.cpp.h"
#include "bn_regular_bg_map_item.cpp.h"
#include "bn_regular_bg_tiles_ptr.cpp.h"
#include "bn_regular_bg_tiles_item.cpp.h"
#include "bn_affine_bg_map_ptr.cpp.h"
#include "bn_affine_bg_map_item.cpp.h"
#include "bn_affine_bg_tiles_ptr.cpp.h"
#include "bn_affine_bg_tiles_item.cpp.h"

#if BN_CFG_BG_BLOCKS_LOG_ENABLED
    #include "bn_log.h"

    static_assert(BN_CFG_LOG_ENABLED, "Log is not enabled");
#elif BN_CFG_LOG_ENABLED
    #include "bn_log.h"
#endif

namespace bn::bg_blocks_manager
{

namespace
{
    static_assert(BN_CFG_BG_BLOCKS_MAX_ITEMS > 0 && BN_CFG_BG_BLOCKS_MAX_ITEMS <= hw::bg_tiles::blocks_count());


    #if BN_CFG_LOG_ENABLED
        constexpr string_view _status_log_message = "\nBG VRAM status has been logged.";
    #else
        constexpr string_view _status_log_message = "";
    #endif


    [[nodiscard]] constexpr int _tiles_to_half_words(int tiles)
    {
        return tiles * int(sizeof(tile) / 2);
    }

    [[nodiscard]] constexpr int _half_words_to_tiles(int half_words)
    {
        return half_words / int(sizeof(tile) / 2);
    }

    [[nodiscard]] constexpr int _blocks_to_half_words(int blocks)
    {
        return blocks * hw::bg_blocks::half_words_per_block();
    }

    [[nodiscard]] constexpr int _blocks_to_tiles(int blocks)
    {
        int half_words = _blocks_to_half_words(blocks);
        return _half_words_to_tiles(half_words);
    }

    [[nodiscard]] constexpr int _ceil_half_words_to_blocks(int half_words)
    {
        int result = half_words / hw::bg_blocks::half_words_per_block();

        if(half_words % hw::bg_blocks::half_words_per_block())
        {
            ++result;
        }

        return result;
    }

    void _hw_commit(const uint16_t* source_ptr, compression_type compression, int count, bool use_dma,
                    uint16_t* destination_ptr)
    {
        switch(compression)
        {

        case compression_type::NONE:
            if(use_dma)
            {
                hw::dma::copy_words(source_ptr, count / 2, destination_ptr);
            }
            else
            {
                hw::memory::copy_words(source_ptr, count / 2, destination_ptr);
            }
            break;

        case compression_type::LZ77:
            hw::decompress::lz77(source_ptr, destination_ptr);
            break;

        case compression_type::RUN_LENGTH:
            hw::decompress::rl_vram(source_ptr, destination_ptr);
            break;

        case compression_type::HUFFMAN:
            hw::decompress::huff(source_ptr, destination_ptr);
            break;

        default:
            BN_ERROR("Unknown compression type: ", int(compression));
            break;
        }
    }

    void _hw_commit_offset(const uint16_t* source_data_ptr, unsigned half_words, uint16_t offset,
                           uint16_t* destination_vram_ptr)
    {
        if(half_words % 2 == 1)
        {
            *destination_vram_ptr = *source_data_ptr + offset;
            ++source_data_ptr;
            ++destination_vram_ptr;
            --half_words;
        }

        if(aligned<4>(source_data_ptr) && aligned<4>(destination_vram_ptr))
        {
            unsigned word_offset = (unsigned(offset) << 16) + offset;

            if(unsigned blocks = half_words / 16)
            {
                bn_hw_bg_blocks_commit_blocks(
                            reinterpret_cast<const unsigned*>(source_data_ptr), blocks, word_offset,
                            reinterpret_cast<unsigned*>(destination_vram_ptr));
                source_data_ptr += blocks * 16;
                destination_vram_ptr += blocks * 16;
                half_words -= blocks * 16;
            }

            if(unsigned words = half_words / 2)
            {
                bn_hw_bg_blocks_commit_words(
                            reinterpret_cast<const unsigned*>(source_data_ptr), words, word_offset,
                            reinterpret_cast<unsigned*>(destination_vram_ptr));
                source_data_ptr += words * 2;
                destination_vram_ptr += words * 2;
                half_words -= words * 2;
            }
        }

        if(half_words)
        {
            bn_hw_bg_blocks_commit_half_words(source_data_ptr, half_words, offset, destination_vram_ptr);
        }
    }


    constexpr int max_items = BN_CFG_BG_BLOCKS_MAX_ITEMS;
    constexpr int max_list_items = max_items + 1;


    enum class status_type
    {
        FREE,
        USED,
        TO_REMOVE
    };


    enum class create_type : uint8_t
    {
        REGULAR_TILES_WITH_OFFSET,
        AFFINE_TILES_WITH_OFFSET,
        TILES_WITHOUT_OFFSET,
        MAP
    };


    class item_type
    {

    public:
        const uint16_t* data = nullptr;
        unsigned usages = 0;
        optional<regular_bg_tiles_ptr> regular_tiles;
        optional<affine_bg_tiles_ptr> affine_tiles;
        optional<bg_palette_ptr> palette;
        uint16_t width = 0; // If is_tiles == true, it stores half_words.
        uint16_t height = 0;
        uint8_t start_block = 0;
        uint8_t blocks_count = 0;
        uint8_t next_index = max_list_items;

    private:
        uint8_t _status: 2 = uint8_t(status_type::FREE);
        uint8_t _compression: 2 = uint8_t(compression_type::NONE);
        uint8_t _big_map_canvas_size: 2 = uint8_t(affine_bg_big_map_canvas_size::NORMAL);

    public:
        bool is_tiles: 1 = false;
        bool is_big: 1 = false;
        bool is_affine: 1 = false;
        bool commit: 1 = false;

        [[nodiscard]] status_type status() const
        {
            return static_cast<status_type>(_status);
        }

        void set_status(status_type status)
        {
            _status = uint8_t(status);
        }

        [[nodiscard]] compression_type compression() const
        {
            return static_cast<compression_type>(_compression);
        }

        void set_compression(compression_type compression)
        {
            _compression = uint8_t(compression);
        }

        [[nodiscard]] affine_bg_big_map_canvas_size big_map_canvas_size() const
        {
            return static_cast<affine_bg_big_map_canvas_size>(_big_map_canvas_size);
        }

        void set_big_map_canvas_size(affine_bg_big_map_canvas_size big_map_canvas_size)
        {
            _big_map_canvas_size = uint8_t(big_map_canvas_size);
        }

        [[nodiscard]] int tiles_count() const
        {
            return _half_words_to_tiles(width);
        }

        [[nodiscard]] int regular_tiles_offset() const
        {
            int tiles_start_block = regular_tiles->id();
            int offset_blocks_count = tiles_start_block % hw::bg_blocks::tiles_alignment_blocks_count();
            int result = _blocks_to_tiles(offset_blocks_count);

            if(palette->bpp() == bpp_mode::BPP_8)
            {
                result /= 2;
            }

            return result;
        }

        [[nodiscard]] int affine_tiles_offset() const
        {
            int tiles_start_block = affine_tiles->id();
            int offset_blocks_count = tiles_start_block % hw::bg_blocks::tiles_alignment_blocks_count();
            return _blocks_to_tiles(offset_blocks_count) / 2;
        }

        [[nodiscard]] int palette_offset() const
        {
            return palette->id();
        }
    };


    class items_list
    {

    public:
        class iterator
        {

        public:
            friend class items_list;

            [[nodiscard]] int id() const
            {
                return _index;
            }

            iterator& operator++()
            {
                const item_type& item = _list->_items[_index];
                _index = item.next_index;
                return *this;
            }

            [[nodiscard]] item_type& operator*()
            {
                return _list->_items[_index];
            }

            item_type* operator->()
            {
                return _list->_items + _index;
            }

            [[nodiscard]] friend bool operator!=(const iterator& a, const iterator& b)
            {
                return a._index != b._index;
            }

        private:
            int _index;
            items_list* _list;

            iterator(int index, items_list& list) :
                _index(index),
                _list(&list)
            {
            }
        };

        items_list()
        {
            for(int index = 0; index < max_items; ++index)
            {
                _free_indices_array[index] = int8_t(index + 1);
            }
        }

        [[nodiscard]] int size() const
        {
            return max_items - _free_indices_size;
        }

        [[nodiscard]] bool full() const
        {
            return ! _free_indices_size;
        }

        [[nodiscard]] item_type& item(int index)
        {
            return _items[index];
        }

        [[nodiscard]] iterator before_begin()
        {
            return iterator(0, *this);
        }

        [[nodiscard]] iterator begin()
        {
            return iterator(_items[0].next_index, *this);
        }

        [[nodiscard]] iterator end()
        {
            return iterator(max_list_items, *this);
        }

        void push_front(const item_type& value)
        {
            insert_after(0, value);
        }

        iterator insert_after(int index, const item_type& value)
        {
            --_free_indices_size;

            int free_index = _free_indices_array[_free_indices_size];
            _items[free_index] = value;
            _insert_node_after(index, free_index);
            return iterator(free_index, *this);
        }

        iterator erase_after(int index)
        {
            _remove_node_after(index);
            return iterator(_items[index].next_index, *this);
        }

    private:
        item_type _items[max_list_items];
        alignas(int) int8_t _free_indices_array[max_items];
        int _free_indices_size = max_items;

        void _join(int index, int new_index)
        {
            _items[index].next_index = uint8_t(new_index);
        }

        void _insert_node_after(int index, int new_index)
        {
            auto next_index = int(_items[index].next_index);
            _join(new_index, next_index);
            _join(index, new_index);
        }

        void _remove_node_after(int index)
        {
            auto next_index = int(_items[index].next_index);
            _free_indices_array[_free_indices_size] = int8_t(next_index);
            ++_free_indices_size;

            auto next_next_index = int(_items[next_index].next_index);
            _join(index, next_next_index);
        }
    };


    class affine_bg_big_map_canvas_info
    {

    public:
        constexpr explicit affine_bg_big_map_canvas_info(affine_bg_big_map_canvas_size canvas_size) :
            _size_bits(5 + int(canvas_size))
        {
        }

        constexpr explicit affine_bg_big_map_canvas_info() :
            affine_bg_big_map_canvas_info(affine_bg_big_map_canvas_size::NORMAL)
        {
        }

        [[nodiscard]] constexpr affine_bg_big_map_canvas_size canvas_size() const
        {
            return affine_bg_big_map_canvas_size(_size_bits - 5);
        }

        [[nodiscard]] constexpr int size_bits() const
        {
            // Default: 5
            return _size_bits;
        }

        [[nodiscard]] constexpr int size() const
        {
            // Default: 1 << 5 = 32
            return 1 << _size_bits;
        }

        [[nodiscard]] constexpr int cells() const
        {
            // Default: 1 << 10 = 32 * 32 = 1024
            return 1 << (_size_bits * 2);
        }

        [[nodiscard]] constexpr int viewport_size() const
        {
            // Default: 32 - 1 = 31
            return size() - 1;
        }

        [[nodiscard]] constexpr int viewport_dec() const
        {
            // 32   0
            // 64   16
            // 128  48
            return (size() / 2) - 16;
        }

    private:
        int _size_bits;
    };


    class static_data
    {

    public:
        items_list items;
        alignas(int) uint8_t to_commit_uncompressed_items_array[max_items];
        alignas(int) uint8_t to_commit_compressed_items_array[max_items];
        affine_bg_big_map_canvas_info new_affine_big_map_canvas_info;
        int free_blocks_count = 0;
        int to_remove_blocks_count = 0;
        int to_commit_uncompressed_items_count = 0;
        int to_commit_compressed_items_count = 0;
        bool allow_tiles_offset = true;
        bool check_commit = false;
        bool delay_commit = false;
    };

    BN_DATA_EWRAM_BSS static_data data;


    [[nodiscard]] constexpr int _new_regular_map_blocks_count(int width, int height, bool big)
    {
        if(big)
        {
            return _ceil_half_words_to_blocks(32 * 32);
        }

        return _ceil_half_words_to_blocks(width * height);
    }

    [[nodiscard]] constexpr int _new_affine_map_blocks_count(int width, int height, bool big)
    {
        if(big)
        {
            return _ceil_half_words_to_blocks(data.new_affine_big_map_canvas_info.cells() / 2);
        }

        return _ceil_half_words_to_blocks((width * height) / 2);
    }


    struct create_data
    {
        const uint16_t* data_ptr;
        int blocks_count;
        int width;
        int height;
        optional<regular_bg_tiles_ptr> regular_tiles;
        optional<affine_bg_tiles_ptr> affine_tiles;
        optional<bg_palette_ptr> palette;
        create_type _create_type;
        bpp_mode bpp;
        compression_type compression;
        bool is_big;
        bool is_affine;

        static create_data from_regular_tiles(const uint16_t* data_ptr, int half_words, bpp_mode bpp,
                                              compression_type compression)
        {
            int blocks_count = _ceil_half_words_to_blocks(half_words);
            create_type create_type = data.allow_tiles_offset ?
                        create_type::REGULAR_TILES_WITH_OFFSET : create_type::TILES_WITHOUT_OFFSET;

            return create_data{ data_ptr, blocks_count, half_words, 1, nullopt, nullopt, nullopt,
                        create_type, bpp, compression, false, false };
        }

        static create_data from_affine_tiles(const uint16_t* data_ptr, int half_words, compression_type compression)
        {
            int blocks_count = _ceil_half_words_to_blocks(half_words);
            create_type create_type = data.allow_tiles_offset ?
                        create_type::AFFINE_TILES_WITH_OFFSET : create_type::TILES_WITHOUT_OFFSET;

            return create_data{ data_ptr, blocks_count, half_words, 1, nullopt, nullopt, nullopt,
                        create_type, bpp_mode::BPP_8, compression, false, true };
        }

        static create_data from_regular_map(
                const uint16_t* data_ptr, const size& dimensions, compression_type compression, bool big,
                regular_bg_tiles_ptr&& tiles, bg_palette_ptr&& palette)
        {
            int width = dimensions.width();
            int height = dimensions.height();
            int blocks_count = _new_regular_map_blocks_count(width, height, big);
            bpp_mode bpp = palette.bpp();

            return create_data{ data_ptr, blocks_count, width, height, move(tiles), nullopt, move(palette),
                        create_type::MAP, bpp, compression, big, false };
        }

        static create_data from_affine_map(
                const uint16_t* data_ptr, const size& dimensions, compression_type compression, bool big,
                affine_bg_tiles_ptr&& tiles, bg_palette_ptr&& palette)
        {
            int width = dimensions.width();
            int height = dimensions.height();
            int blocks_count = _new_affine_map_blocks_count(width, height, big);
            bpp_mode bpp = palette.bpp();

            return create_data{ data_ptr, blocks_count, width, height, nullopt, move(tiles), move(palette),
                        create_type::MAP, bpp, compression, big, true };
        }

        [[nodiscard]] int padding_blocks_count(int start_block) const
        {
            create_type create_type = _create_type;
            int result = 0;

            if(create_type != create_type::MAP)
            {
                int alignment_blocks_count = hw::bg_blocks::tiles_alignment_blocks_count();
                int extra_blocks_count = start_block % alignment_blocks_count;

                if(create_type == create_type::TILES_WITHOUT_OFFSET)
                {
                    if(extra_blocks_count)
                    {
                        result = alignment_blocks_count - extra_blocks_count;
                    }
                }
                else
                {
                    int max_blocks_count;

                    if(create_type == create_type::REGULAR_TILES_WITH_OFFSET)
                    {
                        max_blocks_count = bpp == bpp_mode::BPP_4 ?
                                                hw::bg_blocks::max_bpp_4_regular_tiles_blocks_count() :
                                                hw::bg_blocks::max_bpp_8_regular_tiles_blocks_count();
                    }
                    else
                    {
                        max_blocks_count = hw::bg_blocks::max_affine_tiles_blocks_count();
                    }

                    if(blocks_count + extra_blocks_count > max_blocks_count)
                    {
                        result = alignment_blocks_count - extra_blocks_count;
                    }
                }
            }

            return result;
        }
    };


    #if BN_CFG_BG_BLOCKS_SANITY_CHECK_ENABLED
        void _sanity_check()
        {
            int items_count = 0;
            int free_blocks_count = 0;
            int used_blocks_count = 0;
            int to_remove_blocks_count = 0;
            int next_start_block = 0;

            for(const item_type& item : data.items)
            {
                BN_ASSERT(item.start_block == next_start_block, item.start_block, " - ", next_start_block);
                next_start_block = item.start_block + item.blocks_count;
                ++items_count;

                switch(item.status())
                {

                case status_type::FREE:
                    free_blocks_count += item.blocks_count;
                    break;

                case status_type::USED:
                    used_blocks_count += item.blocks_count;
                    break;

                case status_type::TO_REMOVE:
                    to_remove_blocks_count += item.blocks_count;
                    break;

                default:
                    BN_ERROR("Invalid item status: ", int(item.status()));
                    break;
                }
            }

            BN_ASSERT(items_count == data.items.size(), items_count, " - ", data.items.size());
            BN_ASSERT(free_blocks_count == data.free_blocks_count, free_blocks_count, " - ", data.free_blocks_count);
            BN_ASSERT(to_remove_blocks_count == data.to_remove_blocks_count,
                      to_remove_blocks_count, " - ", data.to_remove_blocks_count);
            BN_ASSERT(free_blocks_count + used_blocks_count + to_remove_blocks_count == hw::bg_tiles::blocks_count(),
                      free_blocks_count, " - ", used_blocks_count, " - ", to_remove_blocks_count, " - ",
                      hw::bg_tiles::blocks_count());
        }

        #define BN_BG_BLOCKS_SANITY_CHECK \
            _sanity_check
    #else
        #define BN_BG_BLOCKS_SANITY_CHECK(...) \
            do \
            { \
            } while(false)
    #endif


    #if BN_CFG_BG_BLOCKS_LOG_ENABLED
        void _log_status()
        {
            BN_LOG("items: ", data.items.size());
            BN_LOG('[');

            for(const item_type& item : data.items)
            {
                if(item.status() == status_type::FREE)
                {
                    BN_LOG("    ",
                            "free",
                            " - start_block: ", item.start_block,
                            " - blocks_count: ", item.blocks_count);
                }
                else if(item.is_tiles)
                {
                    BN_LOG("    ",
                            (item.status() == status_type::USED ? "used" : "to_remove"),
                            (item.is_affine ? "_affine_tiles" : "_regular_tiles"),
                            " - start_block: ", item.start_block,
                            " - blocks_count: ", item.blocks_count,
                            " - data: ", item.data,
                            " - usages: ", item.usages,
                            " - tiles: ", item.tiles_count(),
                            " - compression: ", int(item.compression()),
                            (item.commit ? " - commit" : " - no_commit"));
                }
                else
                {
                    BN_LOG("    ",
                            (item.status() == status_type::USED ? "used" : "to_remove"),
                            (item.is_affine ? "_affine_map" : "_regular_map"),
                            " - start_block: ", item.start_block,
                            " - blocks_count: ", item.blocks_count,
                            " - data: ", item.data,
                            " - usages: ", item.usages,
                            " - width: ", item.width,
                            " - height: ", item.height,
                            " - compression: ", int(item.compression()),
                            " - big: ", item.is_big,
                            " - tiles: ", (item.is_affine ? (item.affine_tiles ? item.affine_tiles->id() : -1) :
                                                            (item.regular_tiles ? item.regular_tiles->id() : -1)),
                            " - palette: ", (item.palette ? item.palette->id() : -1),
                            " - tiles_offset: ", (item.is_affine ?
                                                      (item.affine_tiles ? item.affine_tiles_offset() : -1) :
                                                      (item.regular_tiles ? item.regular_tiles_offset() : -1)),
                            " - palette_offset: ", (item.palette ? item.palette_offset() : -1),
                            (item.commit ? " - commit" : " - no_commit"));
                }
            }

            BN_LOG(']');

            BN_LOG("free_blocks_count: ", data.free_blocks_count);
            BN_LOG("to_remove_blocks_count: ", data.to_remove_blocks_count);
            BN_LOG("allow_tiles_offset: ", (data.allow_tiles_offset ? "true" : "false"));
            BN_LOG("check_commit: ", (data.check_commit ? "true" : "false"));
            BN_LOG("delay_commit: ", (data.delay_commit ? "true" : "false"));
        }

        #define BN_BG_BLOCKS_LOG BN_LOG

        #define BN_BG_BLOCKS_LOG_STATUS \
            _log_status(); \
            BN_BG_BLOCKS_SANITY_CHECK
    #else
        #define BN_BG_BLOCKS_LOG(...) \
            do \
            { \
            } while(false)

        #define BN_BG_BLOCKS_LOG_STATUS \
            BN_BG_BLOCKS_SANITY_CHECK
    #endif

    [[nodiscard]] int _find_tiles_impl(
            const uint16_t* tiles_data, compression_type compression, int half_words, bool affine)
    {
        for(auto iterator = data.items.begin(), end = data.items.end(); iterator != end; ++iterator)
        {
            item_type& item = *iterator;

            if(item.data == tiles_data && item.is_tiles && compression == item.compression() &&
                    half_words == item.width && affine == item.is_affine)
            {
                switch(item.status())
                {

                case status_type::FREE:
                    BN_ERROR("Invalid item state");
                    break;

                case status_type::USED:
                    ++item.usages;
                    break;

                case status_type::TO_REMOVE:
                    item.usages = 1;
                    item.set_status(status_type::USED);
                    data.to_remove_blocks_count -= item.blocks_count;
                    break;

                default:
                    BN_ERROR("Invalid item status: ", int(item.status()));
                    break;
                }

                BN_BG_BLOCKS_LOG("TILES FOUND. start_block: ", item.start_block);
                BN_BG_BLOCKS_LOG_STATUS();

                return iterator.id();
            }
        }

        BN_BG_BLOCKS_LOG("TILES NOT FOUND");
        return -1;
    }

    [[nodiscard]] int _find_regular_map_impl(
            const regular_bg_map_item& map_item, const regular_bg_map_cell* data_ptr,
            const regular_bg_tiles_ptr& tiles, const bg_palette_ptr& palette)
    {
        for(auto iterator = data.items.begin(), end = data.items.end(); iterator != end; ++iterator)
        {
            item_type& item = *iterator;

            if(item.data == data_ptr && ! item.is_tiles && map_item.dimensions().width() == item.width &&
                    map_item.dimensions().height() == item.height && map_item.compression() == item.compression() &&
                    map_item.big() == item.is_big && ! item.is_affine)
            {
                const regular_bg_tiles_ptr* item_tiles = item.regular_tiles.get();
                const bg_palette_ptr* item_palette = item.palette.get();

                if((! item_tiles || tiles == *item_tiles) && (! item_palette || palette == *item_palette))
                {
                    int id = iterator.id();

                    switch(item.status())
                    {

                    case status_type::FREE:
                        BN_ERROR("Invalid item state");
                        break;

                    case status_type::USED:
                        ++item.usages;
                        break;

                    case status_type::TO_REMOVE:
                        {
                            bool different_tiles = tiles != item.regular_tiles;
                            bool different_palette = palette != item.palette;
                            item.usages = 1;
                            item.set_status(status_type::USED);
                            data.to_remove_blocks_count -= item.blocks_count;

                            if(different_tiles && different_palette)
                            {
                                set_regular_map_tiles_and_palette(id, regular_bg_tiles_ptr(tiles), bg_palette_ptr(palette));
                            }
                            else if(different_tiles)
                            {
                                set_regular_map_tiles(id, regular_bg_tiles_ptr(tiles));
                            }
                            else if(different_palette)
                            {
                                set_regular_map_palette(id, bg_palette_ptr(palette));
                            }
                        }
                        break;

                    default:
                        BN_ERROR("Invalid item status: ", int(item.status()));
                        break;
                    }

                    BN_BG_BLOCKS_LOG("REGULAR MAP FOUND. start_block: ", item.start_block);
                    BN_BG_BLOCKS_LOG_STATUS();

                    return id;
                }
            }
        }

        BN_BG_BLOCKS_LOG("REGULAR MAP NOT FOUND");
        return -1;
    }

    [[nodiscard]] int _find_affine_map_impl(
            const affine_bg_map_item& map_item, const affine_bg_map_cell* data_ptr,
            const affine_bg_tiles_ptr& tiles, const bg_palette_ptr& palette)
    {
        for(auto iterator = data.items.begin(), end = data.items.end(); iterator != end; ++iterator)
        {
            item_type& item = *iterator;

            if(reinterpret_cast<const affine_bg_map_cell*>(item.data) == data_ptr && ! item.is_tiles &&
                    map_item.dimensions().width() == item.width && map_item.dimensions().height() == item.height &&
                    map_item.compression() == item.compression() && map_item.big() == item.is_big && item.is_affine)
            {
                const affine_bg_tiles_ptr* item_tiles = item.affine_tiles.get();
                const bg_palette_ptr* item_palette = item.palette.get();

                if((! item_tiles || tiles == *item_tiles) && (! item_palette || palette == *item_palette))
                {
                    int id = iterator.id();

                    switch(item.status())
                    {

                    case status_type::FREE:
                        BN_ERROR("Invalid item state");
                        break;

                    case status_type::USED:
                        ++item.usages;
                        break;

                    case status_type::TO_REMOVE:
                        item.usages = 1;
                        item.set_status(status_type::USED);
                        data.to_remove_blocks_count -= item.blocks_count;

                        if(tiles != item.affine_tiles)
                        {
                            set_affine_map_tiles(id, affine_bg_tiles_ptr(tiles));
                        }

                        if(palette != item.palette)
                        {
                            set_affine_map_palette(id, bg_palette_ptr(palette));
                        }
                        break;

                    default:
                        BN_ERROR("Invalid item status: ", int(item.status()));
                        break;
                    }

                    BN_BG_BLOCKS_LOG("AFFINE MAP FOUND. start_block: ", item.start_block);
                    BN_BG_BLOCKS_LOG_STATUS();

                    return id;
                }
            }
        }

        BN_BG_BLOCKS_LOG("AFFINE MAP NOT FOUND");
        return -1;
    }

    void _commit_item(const item_type& item, bool use_dma)
    {
        const uint16_t* source_data_ptr = item.data;

        if(! source_data_ptr)
        {
            return;
        }

        if(item.is_tiles)
        {
            uint16_t* destination_vram_ptr = hw::bg_blocks::vram(item.start_block);
            _hw_commit(source_data_ptr, item.compression(), item.width, use_dma, destination_vram_ptr);
            return;
        }

        if(item.is_affine)
        {
            // Big maps are committed from bgs_manager:
            if(item.is_big)
            {
                return;
            }

            compression_type compression = item.compression();
            uint16_t* destination_vram_ptr = hw::bg_blocks::vram(item.start_block);
            auto tiles_offset = unsigned(item.affine_tiles_offset());
            int half_words = (item.width * item.height) / 2;

            if(tiles_offset)
            {
                if(compression != compression_type::NONE)
                {
                    _hw_commit(source_data_ptr, compression, half_words, use_dma, destination_vram_ptr);
                    source_data_ptr = destination_vram_ptr;
                }

                uint16_t offset = hw::bg_blocks::affine_map_cells_offset(tiles_offset);
                _hw_commit_offset(source_data_ptr, unsigned(half_words), offset, destination_vram_ptr);
            }
            else
            {
                _hw_commit(source_data_ptr, compression, half_words, use_dma, destination_vram_ptr);
            }
        }
        else
        {
            // Big maps are committed from bgs_manager:
            if(item.is_big)
            {
                return;
            }

            compression_type compression = item.compression();
            uint16_t* destination_vram_ptr = hw::bg_blocks::vram(item.start_block);
            auto tiles_offset = unsigned(item.regular_tiles_offset());
            auto palette_offset = unsigned(item.palette_offset());
            int half_words = item.width * item.height;

            if(compression != compression_type::NONE && (tiles_offset || palette_offset))
            {
                _hw_commit(source_data_ptr, compression, half_words, use_dma, destination_vram_ptr);
                source_data_ptr = destination_vram_ptr;
            }

            if(tiles_offset || palette_offset)
            {
                uint16_t offset = hw::bg_blocks::regular_map_cells_offset(tiles_offset, palette_offset);
                _hw_commit_offset(source_data_ptr, unsigned(half_words), offset, destination_vram_ptr);
            }
            else
            {
                _hw_commit(source_data_ptr, compression, half_words, use_dma, destination_vram_ptr);
            }
        }
    }

    void _fix_blocks_count(const item_type& item, int new_item_blocks_count)
    {
        switch(item.status())
        {

        case status_type::FREE:
            break;

        case status_type::USED:
            BN_ERROR("Invalid item state");
            break;

        case status_type::TO_REMOVE:
            data.free_blocks_count += new_item_blocks_count;
            data.to_remove_blocks_count -= new_item_blocks_count;
            break;

        default:
            BN_ERROR("Invalid item status: ", int(item.status()));
            break;
        }
    }

    [[nodiscard]] int _create_item(int id, int padding_blocks_count, bool delay_commit, create_data&& create_data)
    {
        item_type* item = &data.items.item(id);
        int blocks_count = create_data.blocks_count;

        if(padding_blocks_count)
        {
            BN_BASIC_ASSERT(! data.items.full(), "No more BG block items available");

            int new_item_blocks_count = item->blocks_count - padding_blocks_count;
            item->blocks_count = uint8_t(padding_blocks_count);
            _fix_blocks_count(*item, new_item_blocks_count);

            item_type new_item;
            new_item.start_block = item->start_block + item->blocks_count;
            new_item.blocks_count = uint8_t(new_item_blocks_count);

            auto new_item_iterator = data.items.insert_after(id, new_item);
            id = new_item_iterator.id();
            item = &data.items.item(id);
        }

        bool is_tiles = ! create_data.palette;

        if(int new_item_blocks_count = item->blocks_count - blocks_count)
        {
            BN_BASIC_ASSERT(! data.items.full(), "No more BG block items available");

            int start_block = item->start_block;
            int alignment_blocks_count = hw::bg_blocks::tiles_alignment_blocks_count();
            bool create_item_at_back = ! is_tiles &&
                    (start_block % alignment_blocks_count == 0 ||
                     start_block / alignment_blocks_count != (start_block + blocks_count - 1) / alignment_blocks_count);

            if(create_item_at_back)
            {
                item->blocks_count = uint8_t(new_item_blocks_count);
                _fix_blocks_count(*item, new_item_blocks_count);

                item_type new_item;
                new_item.start_block = uint8_t(start_block + item->blocks_count);

                auto new_item_iterator = data.items.insert_after(id, new_item);
                id = new_item_iterator.id();
                item = &data.items.item(id);
            }
            else
            {
                item_type new_item;
                new_item.start_block = uint8_t(start_block + blocks_count);
                new_item.blocks_count = uint8_t(new_item_blocks_count);
                data.items.insert_after(id, new_item);
            }
        }

        switch(item->status())
        {

        case status_type::FREE:
            data.free_blocks_count -= blocks_count;
            break;

        case status_type::USED:
            BN_ERROR("Invalid item state");
            break;

        case status_type::TO_REMOVE:
            data.to_remove_blocks_count -= blocks_count;
            break;

        default:
            BN_ERROR("Invalid item status: ", int(item->status()));
            break;
        }

        const uint16_t* data_ptr = create_data.data_ptr;
        item->data = data_ptr;
        item->blocks_count = uint8_t(blocks_count);
        item->set_compression(create_data.compression);
        item->set_big_map_canvas_size(data.new_affine_big_map_canvas_info.canvas_size());
        item->regular_tiles = move(create_data.regular_tiles);
        item->affine_tiles = move(create_data.affine_tiles);
        item->palette = move(create_data.palette);
        item->width = uint16_t(create_data.width);
        item->height = uint16_t(create_data.height);
        item->usages = 1;
        item->set_status(status_type::USED);
        item->is_tiles = is_tiles;
        item->is_big = create_data.is_big;
        item->is_affine = create_data.is_affine;

        bool commit_item = false;

        if(data_ptr)
        {
            if(delay_commit)
            {
                commit_item = true;
                data.check_commit = true;
            }
            else
            {
                _commit_item(*item, false);
            }
        }

        item->commit = commit_item;

        return id;
    }

    [[nodiscard]] int _create_impl(create_data&& create_data)
    {
        auto begin = data.items.begin();
        auto end = data.items.end();
        int blocks_count = create_data.blocks_count;
        int to_remove_blocks_count = data.to_remove_blocks_count;

        if(blocks_count <= to_remove_blocks_count &&
                (data.delay_commit || create_data.compression == compression_type::NONE))
        {
            for(auto iterator = begin; iterator != end; ++iterator)
            {
                const item_type& item = *iterator;

                if(item.status() == status_type::TO_REMOVE)
                {
                    int padding_blocks_count = create_data.padding_blocks_count(item.start_block);

                    if(item.blocks_count == blocks_count + padding_blocks_count)
                    {
                        return _create_item(iterator.id(), padding_blocks_count, true, move(create_data));
                    }
                }
            }
        }

        if(blocks_count <= data.free_blocks_count)
        {
            auto smallest_iterator = end;
            int smallest_blocks_count = numeric_limits<int>::max();
            int smallest_padding_blocks_count = 0;

            for(auto iterator = begin; iterator != end; ++iterator)
            {
                const item_type& item = *iterator;

                if(item.status() == status_type::FREE)
                {
                    int padding_blocks_count = create_data.padding_blocks_count(item.start_block);
                    int requested_blocks_count = blocks_count + padding_blocks_count;

                    if(item.blocks_count > requested_blocks_count)
                    {
                        if(item.blocks_count < smallest_blocks_count)
                        {
                            smallest_iterator = iterator;
                            smallest_blocks_count = item.blocks_count;
                            smallest_padding_blocks_count = padding_blocks_count;
                        }
                    }
                    else if(item.blocks_count == requested_blocks_count)
                    {
                        return _create_item(iterator.id(), padding_blocks_count, data.delay_commit, move(create_data));
                    }
                }
            }

            if(smallest_iterator != end)
            {
                return _create_item(smallest_iterator.id(), smallest_padding_blocks_count, data.delay_commit,
                                    move(create_data));
            }
        }

        if(to_remove_blocks_count)
        {
            update();
            data.delay_commit = true;
            return _create_impl(move(create_data));
        }

        return -1;
    }

    [[nodiscard]] int _allocate_impl(create_data&& create_data)
    {
        if(data.delay_commit)
        {
            return -1;
        }

        int blocks_count = create_data.blocks_count;

        if(blocks_count <= data.free_blocks_count)
        {
            auto end = data.items.end();
            auto smallest_iterator = end;
            int smallest_blocks_count = numeric_limits<int>::max();
            int smallest_padding_blocks_count = 0;

            for(auto iterator = data.items.begin(); iterator != end; ++iterator)
            {
                const item_type& item = *iterator;

                if(item.status() == status_type::FREE)
                {
                    int padding_blocks_count = create_data.padding_blocks_count(item.start_block);
                    int requested_blocks_count = blocks_count + padding_blocks_count;

                    if(item.blocks_count > requested_blocks_count)
                    {
                        if(item.blocks_count < smallest_blocks_count)
                        {
                            smallest_iterator = iterator;
                            smallest_blocks_count = item.blocks_count;
                            smallest_padding_blocks_count = padding_blocks_count;
                        }
                    }
                    else if(item.blocks_count == requested_blocks_count)
                    {
                        return _create_item(iterator.id(), padding_blocks_count, false, move(create_data));
                    }
                }
            }

            if(smallest_iterator != end)
            {
                return _create_item(smallest_iterator.id(), smallest_padding_blocks_count, false, move(create_data));
            }
        }

        return -1;
    }

    [[nodiscard]] bool _remove_adjacent_item(int adjacent_id, item_type& current_item)
    {
        const item_type& adjacent_item = data.items.item(adjacent_id);
        status_type adjacent_item_status = adjacent_item.status();
        bool remove = adjacent_item_status != status_type::USED;

        if(remove)
        {
            current_item.blocks_count += adjacent_item.blocks_count;

            if(adjacent_item_status == status_type::TO_REMOVE)
            {
                data.free_blocks_count += adjacent_item.blocks_count;
            }
        }

        return remove;
    }

    [[nodiscard]] int _fix_map_x(int map_x, int map_width)
    {
        map_x %= map_width;

        if(map_x < 0)
        {
            map_x += map_width;
        }

        return map_x;
    }

    [[nodiscard]] int _fix_map_y(int map_y, int map_height)
    {
        return _fix_map_x(map_y, map_height);
    }
}

void init()
{
    ::new(static_cast<void*>(&data)) static_data();

    BN_BG_BLOCKS_LOG("bg_blocks_manager - INIT");

    item_type new_item;
    new_item.blocks_count = hw::bg_tiles::blocks_count();
    data.items.push_front(new_item);
    data.free_blocks_count = new_item.blocks_count;

    BN_BG_BLOCKS_LOG_STATUS();
}

int used_tiles_count()
{
    return _blocks_to_tiles(used_tile_blocks_count());
}

int available_tiles_count()
{
    return _blocks_to_tiles(available_tile_blocks_count());
}

int used_tile_blocks_count()
{
    int result = 0;

    for(const item_type& item : data.items)
    {
        if(item.status() != status_type::FREE && item.is_tiles)
        {
            result += item.blocks_count;
        }
    }

    return result;
}

int available_tile_blocks_count()
{
    return data.free_blocks_count;
}

int used_map_cells_count()
{
    return _blocks_to_half_words(used_map_blocks_count());
}

int available_map_cells_count()
{
    return _blocks_to_half_words(available_map_blocks_count());
}

int used_map_blocks_count()
{
    int result = 0;

    for(const item_type& item : data.items)
    {
        if(item.status() != status_type::FREE && ! item.is_tiles)
        {
            result += item.blocks_count;
        }
    }

    return result;
}

int available_map_blocks_count()
{
    return data.free_blocks_count;
}

affine_bg_big_map_canvas_size new_affine_big_map_canvas_size()
{
    return data.new_affine_big_map_canvas_info.canvas_size();
}

void set_new_affine_big_map_canvas_size(affine_bg_big_map_canvas_size affine_big_map_canvas_size)
{
    data.new_affine_big_map_canvas_info = affine_bg_big_map_canvas_info(affine_big_map_canvas_size);
}

bool allow_tiles_offset()
{
    return data.allow_tiles_offset;
}

void set_allow_tiles_offset(bool allow_tiles_offset)
{
    data.allow_tiles_offset = allow_tiles_offset;
}

#if BN_CFG_LOG_ENABLED
    void log_status()
    {
        #if BN_CFG_BG_BLOCKS_LOG_ENABLED
            BN_BG_BLOCKS_LOG_STATUS();
        #else
            BN_LOG("items: ", data.items.size());
            BN_LOG('[');

            for(const item_type& item : data.items)
            {
                if(item.status() == status_type::FREE)
                {
                    BN_LOG("    ",
                            "free",
                            " - start_block: ", item.start_block,
                            " - blocks_count: ", item.blocks_count);
                }
                else if(item.is_tiles)
                {
                    BN_LOG("    ",
                            (item.status() == status_type::USED ? "used" : "to_remove"),
                            (item.is_affine ? "_affine_tiles" : "_regular_tiles"),
                            " - start_block: ", item.start_block,
                            " - blocks_count: ", item.blocks_count,
                            " - data: ", item.data,
                            " - usages: ", item.usages,
                            " - tiles: ", item.tiles_count());
                }
                else
                {
                    BN_LOG("    ",
                            (item.status() == status_type::USED ? "used" : "to_remove"),
                            (item.is_affine ? "_affine_map" : "_regular_map"),
                            " - start_block: ", item.start_block,
                            " - blocks_count: ", item.blocks_count,
                            " - data: ", item.data,
                            " - usages: ", item.usages,
                            " - width: ", item.width,
                            " - height: ", item.height,
                            " - big: ", item.is_big,
                            " - tiles: ", (item.is_affine ? (item.affine_tiles ? item.affine_tiles->id() : -1) :
                                                            (item.regular_tiles ? item.regular_tiles->id() : -1)),
                            " - palette: ", (item.palette ? item.palette->id() : -1));
                }
            }

            BN_LOG(']');

            BN_LOG("free_blocks_count: ", data.free_blocks_count);
            BN_LOG("to_remove_blocks_count: ", data.to_remove_blocks_count);
        #endif
    }
#endif

int find_regular_tiles(const regular_bg_tiles_item& tiles_item)
{
    const span<const tile>& tiles_ref = tiles_item.tiles_ref();
    auto tiles_data = reinterpret_cast<const uint16_t*>(tiles_ref.data());
    int tiles_count = tiles_ref.size();
    compression_type compression = tiles_item.compression();

    BN_BG_BLOCKS_LOG("bg_blocks_manager - FIND REGULAR TILES: ", tiles_data, " - ", tiles_count, " - ",
                     int(compression));

    return _find_tiles_impl(tiles_data, compression, _tiles_to_half_words(tiles_count), false);
}

int find_affine_tiles(const affine_bg_tiles_item& tiles_item)
{
    const span<const tile>& tiles_ref = tiles_item.tiles_ref();
    auto tiles_data = reinterpret_cast<const uint16_t*>(tiles_ref.data());
    int tiles_count = tiles_ref.size();
    compression_type compression = tiles_item.compression();

    BN_BG_BLOCKS_LOG("bg_blocks_manager - FIND AFFINE TILES: ", tiles_data, " - ", tiles_count, " - ",
                     int(compression));

    return _find_tiles_impl(tiles_data, compression, _tiles_to_half_words(tiles_count), true);
}

int find_regular_map(const regular_bg_map_item& map_item, const regular_bg_map_cell* data_ptr,
                     const regular_bg_tiles_ptr& tiles, const bg_palette_ptr& palette)
{
    BN_BG_BLOCKS_LOG("bg_blocks_manager - FIND REGULAR MAP: ", data_ptr, " - ",
                     map_item.dimensions().width(), " - ", map_item.dimensions().height(), " - ",
                     palette.id(), " - ", int(map_item.compression()), " - ", map_item.big());

    return _find_regular_map_impl(map_item, data_ptr, tiles, palette);
}

int find_affine_map(const affine_bg_map_item& map_item, const affine_bg_map_cell* data_ptr,
                    const affine_bg_tiles_ptr& tiles, const bg_palette_ptr& palette)
{
    BN_BG_BLOCKS_LOG("bg_blocks_manager - FIND AFFINE MAP: ", data_ptr, " - ",
                     map_item.dimensions().width(), " - ", map_item.dimensions().height(), " - ",
                     palette.id(), " - ", int(map_item.compression()), " - ", map_item.big());

    return _find_affine_map_impl(map_item, data_ptr, tiles, palette);
}

int create_regular_tiles(const regular_bg_tiles_item& tiles_item, bool optional)
{
    const span<const tile>& tiles_ref = tiles_item.tiles_ref();
    auto tiles_data = reinterpret_cast<const uint16_t*>(tiles_ref.data());
    int tiles_count = tiles_ref.size();
    int half_words = _tiles_to_half_words(tiles_count);
    bpp_mode bpp = tiles_item.bpp();
    compression_type compression = tiles_item.compression();

    BN_BG_BLOCKS_LOG("bg_blocks_manager - CREATE REGULAR TILES", (optional ? " OPTIONAL: " : ": "),
                     tiles_data, " - ", tiles_count, " - ", _ceil_half_words_to_blocks(half_words), " - ",
                     int(bpp), " - ", int(compression));

    int result = _find_tiles_impl(tiles_data, compression, half_words, false);

    if(result >= 0)
    {
        return result;
    }

    result = _create_impl(create_data::from_regular_tiles(tiles_data, half_words, bpp, compression));

    if(result >= 0)
    {
        BN_BG_BLOCKS_LOG("CREATED. start_block: ", data.items.item(result).start_block);
        BN_BG_BLOCKS_LOG_STATUS();
    }
    else
    {
        BN_BG_BLOCKS_LOG("NOT CREATED");

        if(! optional)
        {
            #if BN_CFG_LOG_ENABLED
                log_status();
            #endif

            BN_ERROR("Regular BG tiles create failed:",
                     "\n\tTiles data: ", tiles_data,
                     "\n\tTiles count: ", tiles_count,
                     "\n\tBlocks count: ", _ceil_half_words_to_blocks(half_words),
                     "\n\nThere's no more available VRAM.",
                     _status_log_message);
        }
    }

    return result;
}

int create_affine_tiles(const affine_bg_tiles_item& tiles_item, bool optional)
{
    const span<const tile>& tiles_ref = tiles_item.tiles_ref();
    auto tiles_data = reinterpret_cast<const uint16_t*>(tiles_ref.data());
    int tiles_count = tiles_ref.size();
    int half_words = _tiles_to_half_words(tiles_count);
    compression_type compression = tiles_item.compression();

    BN_BG_BLOCKS_LOG("bg_blocks_manager - CREATE AFFINE TILES", (optional ? " OPTIONAL: " : ": "),
                     tiles_data, " - ", tiles_count, " - ", _ceil_half_words_to_blocks(half_words), " - ",
                     int(compression));

    int result = _find_tiles_impl(tiles_data, compression, half_words, true);

    if(result >= 0)
    {
        return result;
    }

    result = _create_impl(create_data::from_affine_tiles(tiles_data, half_words, compression));

    if(result >= 0)
    {
        BN_BG_BLOCKS_LOG("CREATED. start_block: ", data.items.item(result).start_block);
        BN_BG_BLOCKS_LOG_STATUS();
    }
    else
    {
        BN_BG_BLOCKS_LOG("NOT CREATED");

        if(! optional)
        {
            #if BN_CFG_LOG_ENABLED
                log_status();
            #endif

            BN_ERROR("Affine BG tiles create failed:",
                     "\n\tTiles data: ", tiles_data,
                     "\n\tTiles count: ", tiles_count,
                     "\n\tBlocks count: ", _ceil_half_words_to_blocks(half_words),
                     "\n\nThere's no more available VRAM.",
                     _status_log_message);
        }
    }

    return result;
}

int create_regular_map(const regular_bg_map_item& map_item, const regular_bg_map_cell* data_ptr,
                       regular_bg_tiles_ptr&& tiles, bg_palette_ptr&& palette, bool optional)
{
    const size& dimensions = map_item.dimensions();
    compression_type compression = map_item.compression();
    bool big = map_item.big();

    BN_BG_BLOCKS_LOG("bg_blocks_manager - CREATE REGULAR MAP", (optional ? " OPTIONAL: " : ": "), data_ptr, " - ",
                     dimensions.width(), " - ", dimensions.height(), " - ", tiles.id(), " - ", palette.id(), " - ",
                     int(compression), " - ", big);

    int result = _find_regular_map_impl(map_item, data_ptr, tiles, palette);

    if(result >= 0)
    {
        return result;
    }

    BN_ASSERT(aligned<4>(data_ptr), "Map cells are not aligned");
    BN_ASSERT(regular_bg_tiles_item::valid_tiles_count(tiles.tiles_count(), palette.bpp()),
              "Invalid tiles count: ", tiles.tiles_count(), " - ", int(palette.bpp()));
    BN_BASIC_ASSERT(compression == compression_type::NONE || ! big, "Compressed big maps are not supported");

    result = _create_impl(
                create_data::from_regular_map(data_ptr, dimensions, compression, big, move(tiles), move(palette)));

    if(result >= 0)
    {
        BN_BG_BLOCKS_LOG("CREATED. start_block: ", data.items.item(result).start_block);
        BN_BG_BLOCKS_LOG_STATUS();
    }
    else
    {
        BN_BG_BLOCKS_LOG("NOT CREATED");

        if(! optional)
        {
            #if BN_CFG_LOG_ENABLED
                log_status();
            #endif

            BN_ERROR("Regular BG map create failed:",
                     "\n\tMap data: ", data_ptr,
                     "\n\tMap width: ", dimensions.width(),
                     "\n\tMap height: ", dimensions.height(),
                     "\n\tBlocks count: ", _new_regular_map_blocks_count(dimensions.width(), dimensions.height(), big),
                     "\n\nThere's no more available VRAM.",
                     _status_log_message);
        }
    }

    return result;
}

int create_affine_map(const affine_bg_map_item& map_item, const affine_bg_map_cell* data_ptr,
                      affine_bg_tiles_ptr&& tiles, bg_palette_ptr&& palette, bool optional)
{
    const size& dimensions = map_item.dimensions();
    compression_type compression = map_item.compression();
    bool big = map_item.big();

    BN_BG_BLOCKS_LOG("bg_blocks_manager - CREATE AFFINE MAP", (optional ? " OPTIONAL: " : ": "), data_ptr, " - ",
                     dimensions.width(), " - ", dimensions.height(), " - ", tiles.id(), " - ", palette.id(), " - ",
                     int(compression), " - ", big);

    int result = _find_affine_map_impl(map_item, data_ptr, tiles, palette);

    if(result >= 0)
    {
        return result;
    }

    BN_ASSERT(aligned<4>(data_ptr), "Map cells are not aligned");
    BN_ASSERT(palette.bpp() == bpp_mode::BPP_8, "4BPP affine maps not supported");
    BN_BASIC_ASSERT(compression == compression_type::NONE || ! big, "Compressed big maps are not supported");
    BN_BASIC_ASSERT(! big || dimensions.width() % data.new_affine_big_map_canvas_info.size() == 0,
                    "Big maps width must be divisible by canvas width: ",
                    dimensions.width(), " - ", data.new_affine_big_map_canvas_info.size());
    BN_BASIC_ASSERT(! big || dimensions.height() % data.new_affine_big_map_canvas_info.size() == 0,
                    "Big maps height must be divisible by canvas height: ",
                    dimensions.height(), " - ", data.new_affine_big_map_canvas_info.size());

    auto u16_data_ptr = reinterpret_cast<const uint16_t*>(data_ptr);
    result = _create_impl(
                create_data::from_affine_map(u16_data_ptr, dimensions, compression, big, move(tiles), move(palette)));

    if(result >= 0)
    {
        BN_BG_BLOCKS_LOG("CREATED. start_block: ", data.items.item(result).start_block);
        BN_BG_BLOCKS_LOG_STATUS();
    }
    else
    {
        BN_BG_BLOCKS_LOG("NOT CREATED");

        if(! optional)
        {
            #if BN_CFG_LOG_ENABLED
                log_status();
            #endif

            BN_ERROR("Affine BG map create failed:",
                     "\n\tMap data: ", data_ptr,
                     "\n\tMap width: ", dimensions.width(),
                     "\n\tMap height: ", dimensions.height(),
                     "\n\tBlocks count: ", _new_affine_map_blocks_count(dimensions.width(), dimensions.height(), big),
                     "\n\nThere's no more available VRAM.",
                     _status_log_message);
        }
    }

    return result;
}

int allocate_regular_tiles(int tiles_count, bpp_mode bpp, bool optional)
{
    int half_words = _tiles_to_half_words(tiles_count);

    BN_BG_BLOCKS_LOG("bg_blocks_manager - ALLOCATE REGULAR TILES", (optional ? " OPTIONAL: " : ": "),
                     tiles_count, " - ", _ceil_half_words_to_blocks(half_words), " - ", int(bpp));

    BN_ASSERT(regular_bg_tiles_item::valid_tiles_count(tiles_count, bpp),
              "Invalid tiles count: ", tiles_count, " - ", int(bpp));

    int result = _allocate_impl(create_data::from_regular_tiles(nullptr, half_words, bpp, compression_type::NONE));

    if(result >= 0)
    {
        BN_BG_BLOCKS_LOG("ALLOCATED. start_block: ", data.items.item(result).start_block);
        BN_BG_BLOCKS_LOG_STATUS();
    }
    else
    {
        BN_BG_BLOCKS_LOG("NOT ALLOCATED");

        if(! optional)
        {
            #if BN_CFG_LOG_ENABLED
                log_status();
            #endif

            BN_ERROR("Regular BG tiles allocate failed:"
                     "\n\tTiles count: ", tiles_count,
                     "\n\tBlocks count: ", _ceil_half_words_to_blocks(half_words),
                     "\n\nThere's no more available VRAM.",
                     _status_log_message);
        }
    }

    return result;
}

int allocate_affine_tiles(int tiles_count, bool optional)
{
    int half_words = _tiles_to_half_words(tiles_count);

    BN_BG_BLOCKS_LOG("bg_blocks_manager - ALLOCATE AFFINE TILES", (optional ? " OPTIONAL: " : ": "),
                     tiles_count, " - ", _ceil_half_words_to_blocks(half_words));

    BN_ASSERT(affine_bg_tiles_item::valid_tiles_count(tiles_count), "Invalid tiles count: ", tiles_count);

    int result = _allocate_impl(create_data::from_affine_tiles(nullptr, half_words, compression_type::NONE));

    if(result >= 0)
    {
        BN_BG_BLOCKS_LOG("ALLOCATED. start_block: ", data.items.item(result).start_block);
        BN_BG_BLOCKS_LOG_STATUS();
    }
    else
    {
        BN_BG_BLOCKS_LOG("NOT ALLOCATED");

        if(! optional)
        {
            #if BN_CFG_LOG_ENABLED
                log_status();
            #endif

            BN_ERROR("Affine BG tiles allocate failed:"
                     "\n\tTiles count: ", tiles_count,
                     "\n\tBlocks count: ", _ceil_half_words_to_blocks(half_words),
                     "\n\nThere's no more available VRAM.",
                     _status_log_message);
        }
    }

    return result;
}

int allocate_regular_map(const size& map_dimensions, regular_bg_tiles_ptr&& tiles,
                         bg_palette_ptr&& palette, bool optional)
{
    BN_BG_BLOCKS_LOG("bg_blocks_manager - ALLOCATE REGULAR MAP", (optional ? " OPTIONAL: " : ": "),
                     map_dimensions.width(), " - ", map_dimensions.height(), " - ", tiles.id(), " - ", palette.id());

    BN_ASSERT(map_dimensions.width() == 32 || map_dimensions.width() == 64,
              "Invalid map width: ", map_dimensions.width());
    BN_ASSERT(map_dimensions.height() == 32 || map_dimensions.height() == 64,
              "Invalid map height: ", map_dimensions.height());
    BN_ASSERT(regular_bg_tiles_item::valid_tiles_count(tiles.tiles_count(), palette.bpp()),
              "Invalid tiles count: ", tiles.tiles_count(), " - ", int(palette.bpp()));

    int result = _allocate_impl(
                create_data::from_regular_map(nullptr, map_dimensions, compression_type::NONE, false,
                                              move(tiles), move(palette)));

    if(result >= 0)
    {
        BN_BG_BLOCKS_LOG("ALLOCATED. start_block: ", data.items.item(result).start_block);
        BN_BG_BLOCKS_LOG_STATUS();
    }
    else
    {
        BN_BG_BLOCKS_LOG("NOT ALLOCATED");

        if(! optional)
        {
            #if BN_CFG_LOG_ENABLED
                log_status();
            #endif

            BN_ERROR("Regular BG map allocate failed:",
                     "\n\tMap width: ", map_dimensions.width(),
                     "\n\tMap height: ", map_dimensions.height(),
                     "\n\tBlocks count: ", _new_regular_map_blocks_count(
                            map_dimensions.width(), map_dimensions.height(), false),
                     "\n\nThere's no more available VRAM.",
                     _status_log_message);
        }
    }

    return result;
}

int allocate_affine_map(const size& map_dimensions, affine_bg_tiles_ptr&& tiles,
                        bg_palette_ptr&& palette, bool optional)
{
    BN_BG_BLOCKS_LOG("bg_blocks_manager - ALLOCATE AFFINE MAP", (optional ? " OPTIONAL: " : ": "),
                     map_dimensions.width(), " - ", map_dimensions.height(), " - ", tiles.id(), " - ", palette.id());

    BN_ASSERT(map_dimensions.width() == 16 || map_dimensions.width() == 32 ||
              map_dimensions.width() == 64 || map_dimensions.width() == 128,
              "Invalid map width: ", map_dimensions.width());
    BN_ASSERT(map_dimensions.height() == map_dimensions.width(),
              "Map height is different from map width: ", map_dimensions.height(), " - ", map_dimensions.width());
    BN_ASSERT(palette.bpp() == bpp_mode::BPP_8, "4BPP affine maps not supported");

    int result = _allocate_impl(
                create_data::from_affine_map(nullptr, map_dimensions, compression_type::NONE, false,
                                             move(tiles), move(palette)));

    if(result >= 0)
    {
        BN_BG_BLOCKS_LOG("ALLOCATED. start_block: ", data.items.item(result).start_block);
        BN_BG_BLOCKS_LOG_STATUS();
    }
    else
    {
        BN_BG_BLOCKS_LOG("NOT ALLOCATED");

        if(! optional)
        {
            #if BN_CFG_LOG_ENABLED
                log_status();
            #endif

            BN_ERROR("Affine BG map allocate failed:",
                     "\n\tMap width: ", map_dimensions.width(),
                     "\n\tMap height: ", map_dimensions.height(),
                     "\n\tBlocks count: ", _new_affine_map_blocks_count(
                            map_dimensions.width(), map_dimensions.height(), false),
                     "\n\nThere's no more available VRAM.",
                     _status_log_message);
        }
    }

    return result;
}

void increase_usages(int id)
{
    item_type& item = data.items.item(id);

    BN_BG_BLOCKS_LOG("bg_blocks_manager - INCREASE USAGES: ", id, " - ", item.start_block);

    ++item.usages;

    BN_BG_BLOCKS_LOG_STATUS();
}

void decrease_usages(int id)
{
    item_type& item = data.items.item(id);

    BN_BG_BLOCKS_LOG("bg_blocks_manager - DECREASE USAGES: ", id, " - ", item.start_block);

    --item.usages;

    if(! item.usages)
    {
        item.set_status(status_type::TO_REMOVE);
        data.to_remove_blocks_count += item.blocks_count;

        item.regular_tiles.reset();
        item.affine_tiles.reset();
        item.palette.reset();
    }

    BN_BG_BLOCKS_LOG_STATUS();
}

int hw_id(int id)
{
    return data.items.item(id).start_block;
}

int hw_tiles_cbb(int id)
{
    return hw_id(id) / hw::bg_blocks::tiles_alignment_blocks_count();
}

int tiles_count(int id)
{
    return data.items.item(id).tiles_count();
}

size map_dimensions(int id)
{
    const item_type& item = data.items.item(id);
    return size(item.width, item.height);
}

bool big_map(int id)
{
    return data.items.item(id).is_big;
}

affine_bg_big_map_canvas_size affine_big_map_canvas_size(int id)
{
    const item_type& item = data.items.item(id);
    BN_BASIC_ASSERT(item.is_big, "Map isn't big");

    return item.big_map_canvas_size();
}

int regular_tiles_offset(int id)
{
    return data.items.item(id).regular_tiles_offset();
}

int affine_tiles_offset(int id)
{
    return data.items.item(id).affine_tiles_offset();
}

int palette_offset(int id)
{
    return data.items.item(id).palette_offset();
}

compression_type compression(int id)
{
    return data.items.item(id).compression();
}

[[nodiscard]] optional<span<const tile>> tiles_ref(int id)
{
    const item_type& item = data.items.item(id);
    optional<span<const tile>> result;

    if(const uint16_t* data_ptr = item.data)
    {
        auto tiles = reinterpret_cast<const tile*>(data_ptr);
        result.emplace(tiles, item.tiles_count());
    }

    return result;
}

optional<span<const regular_bg_map_cell>> regular_map_cells_ref(int id)
{
    const item_type& item = data.items.item(id);
    optional<span<const regular_bg_map_cell>> result;

    if(const uint16_t* item_data = item.data)
    {
        result.emplace(item_data, item.width * item.height);
    }

    return result;
}

optional<span<const affine_bg_map_cell>> affine_map_cells_ref(int id)
{
    const item_type& item = data.items.item(id);
    optional<span<const affine_bg_map_cell>> result;

    if(const uint16_t* item_data = item.data)
    {
        result.emplace(reinterpret_cast<const affine_bg_map_cell*>(item_data), item.width * item.height);
    }

    return result;
}

void set_regular_tiles_ref(int id, const regular_bg_tiles_item& tiles_item)
{
    const span<const tile>& tiles_ref = tiles_item.tiles_ref();
    auto data_ptr = reinterpret_cast<const uint16_t*>(tiles_ref.data());
    compression_type compression = tiles_item.compression();
    item_type& item = data.items.item(id);

    BN_BG_BLOCKS_LOG("bg_blocks_manager - SET REGULAR TILES REF: ", id, " - ", item.start_block, " - ",
                     data_ptr, " - ", tiles_ref.size(), " - ", int(compression));

    const uint16_t* item_data = item.data;
    BN_BASIC_ASSERT(tiles_ref.size() == item.tiles_count(),
                    "Tiles count does not match item tiles count: ", tiles_ref.size(), " - ", item.tiles_count());

    if(item_data != data_ptr)
    {
        BN_BASIC_ASSERT(item_data, "Item has no data");

        item.data = data_ptr;
        item.set_compression(compression);
        item.commit = true;
        data.check_commit = true;

        BN_BG_BLOCKS_LOG_STATUS();
    }
    else if(compression != item.compression())
    {
        item.set_compression(compression);
        item.commit = true;
        data.check_commit = true;

        BN_BG_BLOCKS_LOG_STATUS();
    }
}

void set_affine_tiles_ref(int id, const affine_bg_tiles_item& tiles_item)
{
    const span<const tile>& tiles_ref = tiles_item.tiles_ref();
    auto data_ptr = reinterpret_cast<const uint16_t*>(tiles_ref.data());
    compression_type compression = tiles_item.compression();
    item_type& item = data.items.item(id);

    BN_BG_BLOCKS_LOG("bg_blocks_manager - SET AFFINE TILES REF: ", id, " - ", item.start_block, " - ",
                     data_ptr, " - ", tiles_ref.size(), " - ", int(compression));

    const uint16_t* item_data = item.data;
    BN_BASIC_ASSERT(tiles_ref.size() == item.tiles_count(),
                    "Tiles count does not match item tiles count: ", tiles_ref.size(), " - ", item.tiles_count());

    if(item_data != data_ptr)
    {
        BN_BASIC_ASSERT(item_data, "Item has no data");

        item.data = data_ptr;
        item.set_compression(compression);
        item.commit = true;
        data.check_commit = true;

        BN_BG_BLOCKS_LOG_STATUS();
    }
    else if(compression != item.compression())
    {
        item.set_compression(compression);
        item.commit = true;
        data.check_commit = true;

        BN_BG_BLOCKS_LOG_STATUS();
    }
}

void set_regular_map_cells_ref(int id, const regular_bg_map_item& map_item, const regular_bg_map_cell* data_ptr)
{
    compression_type compression = map_item.compression();
    item_type& item = data.items.item(id);

    BN_BG_BLOCKS_LOG("bg_blocks_manager - SET REGULAR MAP CELLS REF: ", id, " - ", item.start_block, " - ",
                     data_ptr, " - ", map_item.dimensions().width(), " - ", map_item.dimensions().height(), " - ",
                     int(compression), " - ", map_item.big());

    const uint16_t* item_data = item.data;
    BN_BASIC_ASSERT(map_item.dimensions().width() == item.width,
                    "Map width does not match item map width: ", map_item.dimensions().width(), " - ", item.width);
    BN_BASIC_ASSERT(map_item.dimensions().height() == item.height,
                    "Map height does not match item map height: ", map_item.dimensions().height(), " - ", item.height);
    BN_BASIC_ASSERT(map_item.big() == item.is_big, "Map big does not match item map big: ",
                    map_item.big(), " - ", item.is_big);
    BN_BASIC_ASSERT(compression == compression_type::NONE || ! map_item.big(),
                    "Compressed big maps are not supported");

    if(item_data != data_ptr)
    {
        BN_BASIC_ASSERT(item_data, "Item has no data");
        BN_ASSERT(aligned<4>(data_ptr), "Map cells are not aligned");

        item.data = data_ptr;
        item.set_compression(compression);
        item.commit = true;
        data.check_commit = true;

        BN_BG_BLOCKS_LOG_STATUS();
    }
    else if(compression != item.compression())
    {
        item.set_compression(compression);
        item.commit = true;
        data.check_commit = true;

        BN_BG_BLOCKS_LOG_STATUS();
    }
}

void set_affine_map_cells_ref(int id, const affine_bg_map_item& map_item, const affine_bg_map_cell* data_ptr)
{
    compression_type compression = map_item.compression();
    item_type& item = data.items.item(id);

    BN_BG_BLOCKS_LOG("bg_blocks_manager - SET AFFINE MAP CELLS REF: ", id, " - ", item.start_block, " - ",
                     data_ptr, " - ", map_item.dimensions().width(), " - ", map_item.dimensions().height(), " - ",
                     int(compression), " - ", map_item.big());

    const uint16_t* item_data = item.data;
    BN_BASIC_ASSERT(map_item.dimensions().width() == item.width,
                    "Map width does not match item map width: ", map_item.dimensions().width(), " - ", item.width);
    BN_BASIC_ASSERT(map_item.dimensions().height() == item.height,
                    "Map height does not match item map height: ", map_item.dimensions().height(), " - ", item.height);
    BN_BASIC_ASSERT(map_item.big() == item.is_big, "Map big does not match item map big: ",
                    map_item.big(), " - ", item.is_big);
    BN_BASIC_ASSERT(compression == compression_type::NONE || ! map_item.big(),
                    "Compressed big maps are not supported");

    auto u16_data_ptr = reinterpret_cast<const uint16_t*>(data_ptr);

    if(item_data != u16_data_ptr)
    {
        BN_BASIC_ASSERT(item_data, "Item has no data");
        BN_ASSERT(aligned<4>(data_ptr), "Map cells are not aligned");

        item.data = u16_data_ptr;
        item.set_compression(compression);
        item.commit = true;
        data.check_commit = true;

        BN_BG_BLOCKS_LOG_STATUS();
    }
    else if(compression != item.compression())
    {
        item.set_compression(compression);
        item.commit = true;
        data.check_commit = true;

        BN_BG_BLOCKS_LOG_STATUS();
    }
}

void reload(int id)
{
    item_type& item = data.items.item(id);

    BN_BG_BLOCKS_LOG("bg_blocks_manager - RELOAD: ", id, " - ", item.start_block);

    BN_BASIC_ASSERT(item.data, "Item has no data");

    item.commit = true;
    data.check_commit = true;

    BN_BG_BLOCKS_LOG_STATUS();
}

const regular_bg_tiles_ptr& regular_map_tiles(int id)
{
    const item_type& item = data.items.item(id);
    return *item.regular_tiles;
}

const affine_bg_tiles_ptr& affine_map_tiles(int id)
{
    const item_type& item = data.items.item(id);
    return *item.affine_tiles;
}

void set_regular_map_tiles(int id, regular_bg_tiles_ptr&& tiles)
{
    item_type& item = data.items.item(id);

    if(tiles != item.regular_tiles)
    {
        BN_ASSERT(regular_bg_tiles_item::valid_tiles_count(tiles.tiles_count(), item.palette->bpp()),
                  "Invalid tiles count: ", tiles.tiles_count(), " - ", int(item.palette->bpp()));

        int old_tiles_cbb;
        int old_tiles_offset;

        if(const regular_bg_tiles_ptr* item_regular_tiles = item.regular_tiles.get())
        {
            old_tiles_cbb = item_regular_tiles->cbb();
            old_tiles_offset = item.regular_tiles_offset();
        }
        else
        {
            old_tiles_cbb = -1;
            old_tiles_offset = -1;
        }

        int new_tiles_cbb = tiles.cbb();

        if(new_tiles_cbb != old_tiles_cbb)
        {
            bgs_manager::update_regular_map_tiles_cbb(item.start_block, new_tiles_cbb);
        }

        item.regular_tiles = move(tiles);

        if(item.regular_tiles_offset() != old_tiles_offset)
        {
            item.commit = true;
            data.check_commit = true;
        }
    }
}

void set_affine_map_tiles(int id, affine_bg_tiles_ptr&& tiles)
{
    item_type& item = data.items.item(id);

    if(tiles != item.affine_tiles)
    {
        int old_tiles_cbb;
        int old_tiles_offset;

        if(const affine_bg_tiles_ptr* item_affine_tiles = item.affine_tiles.get())
        {
            old_tiles_cbb = item_affine_tiles->cbb();
            old_tiles_offset = item.affine_tiles_offset();
        }
        else
        {
            old_tiles_cbb = -1;
            old_tiles_offset = -1;
        }

        int new_tiles_cbb = tiles.cbb();

        if(new_tiles_cbb != old_tiles_cbb)
        {
            bgs_manager::update_affine_map_tiles_cbb(item.start_block, new_tiles_cbb);
        }

        item.affine_tiles = move(tiles);

        if(item.affine_tiles_offset() != old_tiles_offset)
        {
            item.commit = true;
            data.check_commit = true;
        }
    }
}

void remove_regular_map_tiles(int id)
{
    item_type& item = data.items.item(id);
    item.regular_tiles.reset();
}

void remove_affine_map_tiles(int id)
{
    item_type& item = data.items.item(id);
    item.affine_tiles.reset();
}

const bg_palette_ptr& map_palette(int id)
{
    const item_type& item = data.items.item(id);
    return *item.palette;
}

void set_regular_map_palette(int id, bg_palette_ptr&& palette)
{
    item_type& item = data.items.item(id);

    if(palette != item.palette)
    {
        bpp_mode new_palette_bpp = palette.bpp();
        BN_ASSERT(regular_bg_tiles_item::valid_tiles_count(item.regular_tiles->tiles_count(), new_palette_bpp),
                  "Invalid palette BPP: ", item.regular_tiles->tiles_count(), " - ", int(new_palette_bpp));

        int old_palette_bpp;
        int old_tiles_offset;
        int old_palette_offset;

        if(item.palette)
        {
            old_palette_bpp = int(item.palette->bpp());
            old_tiles_offset = item.regular_tiles_offset();
            old_palette_offset = item.palette_offset();
        }
        else
        {
            old_palette_bpp = -1;
            old_tiles_offset = -1;
            old_palette_offset = -1;
        }

        if(int(new_palette_bpp) != old_palette_bpp)
        {
            bgs_manager::update_regular_map_palette_bpp(item.start_block, new_palette_bpp);
        }

        item.palette = move(palette);

        if(item.regular_tiles_offset() != old_tiles_offset || item.palette_offset() != old_palette_offset)
        {
            item.commit = true;
            data.check_commit = true;
        }
    }
}

void set_affine_map_palette(int id, bg_palette_ptr&& palette)
{
    item_type& item = data.items.item(id);

    if(palette != item.palette)
    {
        BN_ASSERT(palette.bpp() == bpp_mode::BPP_8, "4BPP affine maps not supported");

        item.palette = move(palette);
    }
}

void remove_map_palette(int id)
{
    item_type& item = data.items.item(id);
    item.palette.reset();
}

void set_regular_map_tiles_and_palette(int id, regular_bg_tiles_ptr&& tiles, bg_palette_ptr&& palette)
{
    item_type& item = data.items.item(id);
    bpp_mode new_palette_bpp = palette.bpp();
    BN_ASSERT(regular_bg_tiles_item::valid_tiles_count(tiles.tiles_count(), new_palette_bpp),
              "Invalid tiles count or palette BPP: ", tiles.tiles_count(), " - ", int(new_palette_bpp));

    int old_tiles_offset;
    int old_palette_offset;

    if(item.regular_tiles && item.palette)
    {
        old_tiles_offset = item.regular_tiles_offset();
        old_palette_offset = item.palette_offset();
    }
    else
    {
        old_tiles_offset = -1;
        old_palette_offset = -1;
    }

    if(tiles != item.regular_tiles)
    {
        const regular_bg_tiles_ptr* item_regular_tiles = item.regular_tiles.get();
        int old_tiles_cbb = item_regular_tiles ? item_regular_tiles->cbb() : -1;
        int new_tiles_cbb = tiles.cbb();

        if(new_tiles_cbb != old_tiles_cbb)
        {
            bgs_manager::update_regular_map_tiles_cbb(item.start_block, new_tiles_cbb);
        }

        item.regular_tiles = move(tiles);
    }

    if(palette != item.palette)
    {
        const bg_palette_ptr* item_palette = item.palette.get();
        int old_palette_bpp = item_palette ? int(item_palette->bpp()) : -1;

        if(int(new_palette_bpp) != old_palette_bpp)
        {
            bgs_manager::update_regular_map_palette_bpp(item.start_block, new_palette_bpp);
        }

        item.palette = move(palette);
    }

    if(item.regular_tiles_offset() != old_tiles_offset || item.palette_offset() != old_palette_offset)
    {
        item.commit = true;
        data.check_commit = true;
    }
}

optional<span<tile>> tiles_vram(int id)
{
    const item_type& item = data.items.item(id);
    optional<span<tile>> result;

    if(! item.data)
    {
        auto vram_ptr = reinterpret_cast<tile*>(hw::bg_blocks::vram(item.start_block));
        result.emplace(vram_ptr, item.tiles_count());
    }

    return result;
}

optional<span<regular_bg_map_cell>> regular_map_vram(int id)
{
    const item_type& item = data.items.item(id);
    optional<span<regular_bg_map_cell>> result;

    if(! item.data)
    {
        regular_bg_map_cell* vram_ptr = hw::bg_blocks::vram(item.start_block);
        result.emplace(vram_ptr, item.width * item.height);
    }

    return result;
}

optional<span<affine_bg_map_cell>> affine_map_vram(int id)
{
    const item_type& item = data.items.item(id);
    optional<span<affine_bg_map_cell>> result;

    if(! item.data)
    {
        auto vram_ptr = reinterpret_cast<affine_bg_map_cell*>(hw::bg_blocks::vram(item.start_block));
        result.emplace(vram_ptr, item.width * item.height);
    }

    return result;
}

bool must_commit(int id)
{
    return data.items.item(id).commit;
}

void update_regular_map_col(int id, int x, int y)
{
    const item_type& item = data.items.item(id);
    const uint16_t* item_data = item.data;

    if(! item_data)
    {
        return;
    }

    int map_width = item.width;
    int map_height = item.height;
    x = _fix_map_x(x, map_width);
    y = _fix_map_y(y, map_height);

    const uint16_t* first_source_data = item_data + ((y * map_width) + x);
    int y_separator = y & 31;
    int second_y = _fix_map_y(y + 32 - y_separator, map_height);

    const uint16_t* second_source_data = item_data + ((second_y * map_width) + x);
    uint16_t* dest_data = hw::bg_blocks::vram(item.start_block) + (y_separator * 32) + (x & 31);
    auto tiles_offset = unsigned(item.regular_tiles_offset());
    auto palette_offset = unsigned(item.palette_offset());

    if(tiles_offset || palette_offset)
    {
        uint16_t offset = hw::bg_blocks::regular_map_cells_offset(tiles_offset, palette_offset);

        for(int iy = y_separator; iy < 32; ++iy)
        {
            *dest_data = *first_source_data + offset;
            dest_data += 32;
            first_source_data += map_width;
        }

        dest_data -= 1024;

        for(int iy = 0; iy < y_separator; ++iy)
        {
            *dest_data = *second_source_data + offset;
            dest_data += 32;
            second_source_data += map_width;
        }
    }
    else
    {
        for(int iy = y_separator; iy < 32; ++iy)
        {
            *dest_data = *first_source_data;
            dest_data += 32;
            first_source_data += map_width;
        }

        dest_data -= 1024;

        for(int iy = 0; iy < y_separator; ++iy)
        {
            *dest_data = *second_source_data;
            dest_data += 32;
            second_source_data += map_width;
        }
    }
}

void update_affine_map_col(int id, int x, int y)
{
    const item_type& item = data.items.item(id);
    auto item_data = reinterpret_cast<const uint8_t*>(item.data);

    if(! item_data)
    {
        return;
    }

    affine_bg_big_map_canvas_info canvas_info(item.big_map_canvas_size());
    int canvas_size = canvas_info.size();
    int canvas_cells = canvas_info.cells();
    int canvas_size_bits = canvas_info.size_bits();
    int canvas_viewport_size = canvas_info.viewport_size();
    int canvas_viewport_dec = canvas_info.viewport_dec();
    x -= canvas_viewport_dec;
    y -= canvas_viewport_dec;

    int map_width = item.width;
    int map_height = item.height;
    x = _fix_map_x(x, map_width);
    y = _fix_map_y(y, map_height);

    const uint8_t* first_source_data = item_data + ((y * map_width) + x);
    int y_separator = y & canvas_viewport_size;
    int second_y = _fix_map_y(y + canvas_size - y_separator, map_height);

    const uint8_t* second_source_data = item_data + ((second_y * map_width) + x);
    auto dest_data = reinterpret_cast<uint8_t*>(hw::bg_blocks::vram(item.start_block));
    dest_data += ((y_separator << canvas_size_bits) + (x & canvas_viewport_size));

    if(auto tiles_offset = unsigned(item.affine_tiles_offset()))
    {
        if(x % 2)
        {
            for(int iy = y_separator; iy < canvas_size; ++iy)
            {
                auto u16_dest_data = reinterpret_cast<uint16_t*>(dest_data - 1);
                auto joined_value = uint16_t(((*first_source_data + tiles_offset) << 8) | (*u16_dest_data & 0xFF));
                *u16_dest_data = joined_value;
                dest_data += canvas_size;
                first_source_data += map_width;
            }

            dest_data -= canvas_cells;

            for(int iy = 0; iy < y_separator; ++iy)
            {
                auto u16_dest_data = reinterpret_cast<uint16_t*>(dest_data - 1);
                auto joined_value = uint16_t(((*second_source_data + tiles_offset) << 8) | (*u16_dest_data & 0xFF));
                *u16_dest_data = joined_value;
                dest_data += canvas_size;
                second_source_data += map_width;
            }
        }
        else
        {
            for(int iy = y_separator; iy < canvas_size; ++iy)
            {
                auto u16_dest_data = reinterpret_cast<uint16_t*>(dest_data);
                auto joined_value = uint16_t((*u16_dest_data & 0xFF00) | (*first_source_data + tiles_offset));
                *u16_dest_data = joined_value;
                dest_data += canvas_size;
                first_source_data += map_width;
            }

            dest_data -= canvas_cells;

            for(int iy = 0; iy < y_separator; ++iy)
            {
                auto u16_dest_data = reinterpret_cast<uint16_t*>(dest_data);
                auto joined_value = uint16_t((*u16_dest_data & 0xFF00) | (*second_source_data + tiles_offset));
                *u16_dest_data = joined_value;
                dest_data += canvas_size;
                second_source_data += map_width;
            }
        }
    }
    else
    {
        if(x % 2)
        {
            for(int iy = y_separator; iy < canvas_size; ++iy)
            {
                auto u16_dest_data = reinterpret_cast<uint16_t*>(dest_data - 1);
                auto joined_value = uint16_t((unsigned(*first_source_data) << 8) | (*u16_dest_data & 0xFF));
                *u16_dest_data = joined_value;
                dest_data += canvas_size;
                first_source_data += map_width;
            }

            dest_data -= canvas_cells;

            for(int iy = 0; iy < y_separator; ++iy)
            {
                auto u16_dest_data = reinterpret_cast<uint16_t*>(dest_data - 1);
                uint16_t joined_value = uint16_t((unsigned(*second_source_data) << 8) | (*u16_dest_data & 0xFF));
                *u16_dest_data = joined_value;
                dest_data += canvas_size;
                second_source_data += map_width;
            }
        }
        else
        {
            for(int iy = y_separator; iy < canvas_size; ++iy)
            {
                auto u16_dest_data = reinterpret_cast<uint16_t*>(dest_data);
                uint16_t joined_value = uint16_t((*u16_dest_data & 0xFF00) | *first_source_data);
                *u16_dest_data = joined_value;
                dest_data += canvas_size;
                first_source_data += map_width;
            }

            dest_data -= canvas_cells;

            for(int iy = 0; iy < y_separator; ++iy)
            {
                auto u16_dest_data = reinterpret_cast<uint16_t*>(dest_data);
                uint16_t joined_value = uint16_t((*u16_dest_data & 0xFF00) | *second_source_data);
                *u16_dest_data = joined_value;
                dest_data += canvas_size;
                second_source_data += map_width;
            }
        }
    }
}

void update_affine_map_right_col(int id, int x, int y)
{
    const item_type& item = data.items.item(id);
    affine_bg_big_map_canvas_info canvas_info(item.big_map_canvas_size());
    update_affine_map_col(id, x + canvas_info.viewport_size(), y);
}

void update_regular_map_row(int id, int x, int y)
{
    const item_type& item = data.items.item(id);
    const uint16_t* item_data = item.data;

    if(! item_data)
    {
        return;
    }

    int map_width = item.width;
    int map_height = item.height;
    x = _fix_map_x(x, map_width);
    y = _fix_map_y(y, map_height);

    const uint16_t* first_source_data = item_data + ((y * map_width) + x);
    int x_separator = x & 31;
    int elements = 32 - x_separator;
    int second_x = _fix_map_x(x + elements, map_width);

    const uint16_t* second_source_data = item_data + ((y * map_width) + second_x);
    uint16_t* dest_data = hw::bg_blocks::vram(item.start_block) + (((y & 31) * 32) + x_separator);
    auto tiles_offset = unsigned(item.regular_tiles_offset());
    auto palette_offset = unsigned(item.palette_offset());

    if(tiles_offset || palette_offset)
    {
        uint16_t offset = hw::bg_blocks::regular_map_cells_offset(tiles_offset, palette_offset);
        _hw_commit_offset(first_source_data, unsigned(elements), offset, dest_data);
        dest_data -= x_separator;
        _hw_commit_offset(second_source_data, unsigned(x_separator), offset, dest_data);
    }
    else
    {
        hw::memory::copy_half_words(first_source_data, elements, dest_data);
        dest_data -= x_separator;
        hw::memory::copy_half_words(second_source_data, x_separator, dest_data);
    }
}

void update_affine_map_row(int id, int x, int y)
{
    // BN_ASSERT(x % 2 == 0, "Invalid x: ", x);

    const item_type& item = data.items.item(id);
    auto item_data = reinterpret_cast<const uint8_t*>(item.data);

    if(! item_data)
    {
        return;
    }

    affine_bg_big_map_canvas_info canvas_info(item.big_map_canvas_size());
    int canvas_size_bits = canvas_info.size_bits();
    int canvas_viewport_size = canvas_info.viewport_size();
    int canvas_viewport_dec = canvas_info.viewport_dec();
    x -= canvas_viewport_dec;
    y -= canvas_viewport_dec;

    int map_width = item.width;
    int map_height = item.height;
    x = _fix_map_x(x, map_width);
    y = _fix_map_y(y, map_height);

    const uint8_t* first_source_data = item_data + ((y * map_width) + x);
    int x_separator = x & canvas_viewport_size;
    int elements = canvas_info.size() - x_separator;
    int second_x = _fix_map_x(x + elements, map_width);

    const uint8_t* second_source_data = item_data + ((y * map_width) + second_x);
    auto dest_data = reinterpret_cast<uint8_t*>(hw::bg_blocks::vram(item.start_block));
    dest_data += ((y & canvas_viewport_size) << canvas_size_bits) + x_separator;

    if(auto tiles_offset = unsigned(item.affine_tiles_offset()))
    {
        uint16_t offset = hw::bg_blocks::affine_map_cells_offset(tiles_offset);
        _hw_commit_offset(reinterpret_cast<const uint16_t*>(first_source_data), unsigned(elements) / 2, offset,
                          reinterpret_cast<uint16_t*>(dest_data));
        dest_data -= x_separator;
        _hw_commit_offset(reinterpret_cast<const uint16_t*>(second_source_data), unsigned(x_separator) / 2, offset,
                          reinterpret_cast<uint16_t*>(dest_data));
    }
    else
    {
        hw::memory::copy_half_words(first_source_data, elements / 2, dest_data);
        dest_data -= x_separator;
        hw::memory::copy_half_words(second_source_data, x_separator / 2, dest_data);
    }
}

void update_affine_map_bottom_row(int id, int x, int y)
{
    const item_type& item = data.items.item(id);
    affine_bg_big_map_canvas_info canvas_info(item.big_map_canvas_size());
    update_affine_map_row(id, x, y + canvas_info.viewport_size());
}

void set_regular_map_position(int id, int x, int y)
{
    const item_type& item = data.items.item(id);
    const uint16_t* item_data = item.data;

    if(! item_data)
    {
        return;
    }

    int map_width = item.width;
    int map_height = item.height;
    x = _fix_map_x(x, map_width);
    y = _fix_map_y(y, map_height);

    uint16_t* vram_data = hw::bg_blocks::vram(item.start_block);
    int x_separator = x & 31;
    int elements = 32 - x_separator;
    int second_x = _fix_map_x(x + elements, map_width);
    auto tiles_offset = unsigned(item.regular_tiles_offset());
    auto palette_offset = unsigned(item.palette_offset());

    if(tiles_offset || palette_offset)
    {
        uint16_t offset = hw::bg_blocks::regular_map_cells_offset(tiles_offset, palette_offset);

        for(int row = y, row_limit = y + 31; row <= row_limit; ++row)
        {
            int fixed_row = row;

            if(fixed_row >= map_height)
            {
                fixed_row -= map_height;
            }

            const uint16_t* first_source_data = item_data + ((fixed_row * map_width) + x);
            uint16_t* dest_data = vram_data + (((fixed_row & 31) * 32) + x_separator);
            _hw_commit_offset(first_source_data, unsigned(elements), offset, dest_data);

            const uint16_t* second_source_data = item_data + ((fixed_row * map_width) + second_x);
            dest_data -= x_separator;
            _hw_commit_offset(second_source_data, unsigned(x_separator), offset, dest_data);
        }
    }
    else
    {
        for(int row = y, row_limit = y + 31; row <= row_limit; ++row)
        {
            int fixed_row = row;

            if(fixed_row >= map_height)
            {
                fixed_row -= map_height;
            }

            const uint16_t* first_source_data = item_data + ((fixed_row * map_width) + x);
            uint16_t* dest_data = vram_data + (((fixed_row & 31) * 32) + x_separator);
            hw::memory::copy_half_words(first_source_data, elements, dest_data);

            const uint16_t* second_source_data = item_data + ((fixed_row * map_width) + second_x);
            dest_data -= x_separator;
            hw::memory::copy_half_words(second_source_data, x_separator, dest_data);
        }
    }
}

void set_affine_map_position(int id, int x, int y)
{
    // BN_ASSERT(x % 2 == 0, "Invalid x: ", x);

    const item_type& item = data.items.item(id);
    auto item_data = reinterpret_cast<const uint8_t*>(item.data);

    if(! item_data)
    {
        return;
    }

    affine_bg_big_map_canvas_info canvas_info(item.big_map_canvas_size());
    int canvas_size_bits = canvas_info.size_bits();
    int canvas_viewport_size = canvas_info.viewport_size();
    int canvas_viewport_dec = canvas_info.viewport_dec();
    x -= canvas_viewport_dec;
    y -= canvas_viewport_dec;

    int map_width = item.width;
    int map_height = item.height;
    x = _fix_map_x(x, map_width);
    y = _fix_map_y(y, map_height);

    auto vram_data = reinterpret_cast<uint8_t*>(hw::bg_blocks::vram(item.start_block));
    int x_separator = x & canvas_viewport_size;
    int elements = canvas_info.size() - x_separator;
    int second_x = _fix_map_x(x + elements, map_width);

    if(auto tiles_offset = unsigned(item.affine_tiles_offset()))
    {
        uint16_t offset = hw::bg_blocks::affine_map_cells_offset(tiles_offset);

        for(int row = y, row_limit = y + canvas_viewport_size; row <= row_limit; ++row)
        {
            int fixed_row = row;

            if(fixed_row >= map_height)
            {
                fixed_row -= map_height;
            }

            const uint8_t* first_source_data = item_data + ((fixed_row * map_width) + x);
            uint8_t* dest_data = vram_data + (((row & canvas_viewport_size) << canvas_size_bits) + x_separator);
            _hw_commit_offset(reinterpret_cast<const uint16_t*>(first_source_data), unsigned(elements) / 2,
                              offset, reinterpret_cast<uint16_t*>(dest_data));

            const uint8_t* second_source_data = item_data + ((fixed_row * map_width) + second_x);
            dest_data -= x_separator;
            _hw_commit_offset(reinterpret_cast<const uint16_t*>(second_source_data), unsigned(x_separator) / 2,
                              offset, reinterpret_cast<uint16_t*>(dest_data));
        }
    }
    else
    {
        for(int row = y, row_limit = y + canvas_viewport_size; row <= row_limit; ++row)
        {
            int fixed_row = row;

            if(fixed_row >= map_height)
            {
                fixed_row -= map_height;
            }

            const uint8_t* first_source_data = item_data + ((fixed_row * map_width) + x);
            uint8_t* dest_data = vram_data + (((row & canvas_viewport_size) << canvas_size_bits) + x_separator);
            hw::memory::copy_half_words(first_source_data, elements / 2, dest_data);

            const uint8_t* second_source_data = item_data + ((fixed_row * map_width) + second_x);
            dest_data -= x_separator;
            hw::memory::copy_half_words(second_source_data, x_separator / 2, dest_data);
        }
    }
}

void update()
{
    if(data.to_remove_blocks_count || data.check_commit)
    {
        data.to_remove_blocks_count = 0;
        data.check_commit = false;

        BN_BG_BLOCKS_LOG("bg_blocks_manager - UPDATE");

        auto end = data.items.end();
        auto before_previous_iterator = end;
        auto previous_iterator = data.items.before_begin();
        auto iterator = previous_iterator;
        ++iterator;

        int commit_uncompressed_items_count = 0;
        int commit_compressed_items_count = 0;

        while(iterator != end)
        {
            item_type& item = *iterator;
            status_type item_status = item.status();

            if(item_status == status_type::TO_REMOVE)
            {
                item.data = nullptr;
                item.width = 0;
                item.height = 0;
                item.set_status(status_type::FREE);
                item.commit = false;
                data.free_blocks_count += item.blocks_count;

                auto next_iterator = iterator;
                ++next_iterator;

                while(next_iterator != end)
                {
                    if(_remove_adjacent_item(next_iterator.id(), item))
                    {
                        next_iterator = data.items.erase_after(iterator.id());
                    }
                    else
                    {
                        break;
                    }
                }

                if(before_previous_iterator != end)
                {
                    if(_remove_adjacent_item(previous_iterator.id(), item))
                    {
                        item.start_block = previous_iterator->start_block;
                        data.items.erase_after(before_previous_iterator.id());
                        previous_iterator = before_previous_iterator;
                    }
                }
            }
            else if(item.commit)
            {
                if(item.compression() == compression_type::NONE)
                {
                    data.to_commit_uncompressed_items_array[commit_uncompressed_items_count] = uint8_t(iterator.id());
                    ++commit_uncompressed_items_count;
                }
                else
                {
                    data.to_commit_compressed_items_array[commit_compressed_items_count] = uint8_t(iterator.id());
                    ++commit_compressed_items_count;
                }
            }

            before_previous_iterator = previous_iterator;
            previous_iterator = iterator;
            ++iterator;
        }

        data.to_commit_uncompressed_items_count = commit_uncompressed_items_count;
        data.to_commit_compressed_items_count = commit_compressed_items_count;

        BN_BG_BLOCKS_LOG_STATUS();
    }

    data.delay_commit = false;
}

void commit_uncompressed(bool use_dma)
{
    if(int commit_items_count = data.to_commit_uncompressed_items_count)
    {
        BN_BG_BLOCKS_LOG("bg_blocks_manager - COMMIT UNCOMPRESSED");

        for(int index = 0; index < commit_items_count; ++index)
        {
            int item_index = data.to_commit_uncompressed_items_array[index];
            item_type& item = data.items.item(item_index);
            item.commit = false;
            _commit_item(item, use_dma);
        }

        data.to_commit_uncompressed_items_count = 0;

        BN_BG_BLOCKS_LOG_STATUS();
    }
}

void commit_compressed()
{
    if(int commit_items_count = data.to_commit_compressed_items_count)
    {
        BN_BG_BLOCKS_LOG("bg_blocks_manager - COMMIT COMPRESSED");

        for(int index = 0; index < commit_items_count; ++index)
        {
            int item_index = data.to_commit_compressed_items_array[index];
            item_type& item = data.items.item(item_index);
            item.commit = false;
            _commit_item(item, false);
        }

        data.to_commit_compressed_items_count = 0;

        BN_BG_BLOCKS_LOG_STATUS();
    }
}

}
