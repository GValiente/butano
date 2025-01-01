/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DMG_MUSIC_POSITION_H
#define BN_DMG_MUSIC_POSITION_H

/**
 * @file
 * bn::dmg_music_position header file.
 *
 * @ingroup dmg_music
 */

#include "bn_assert.h"
#include "bn_functional.h"

namespace bn
{

/**
 * @brief Contains the required information to specify the position of an active DMG music.
 *
 * @ingroup dmg_music
 */
class dmg_music_position
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr dmg_music_position() = default;

    /**
     * @brief Constructor.
     * @param pattern Pattern order.
     * @param row Row inside the pattern.
     */
    constexpr dmg_music_position(int pattern, int row) :
        _pattern(unsigned(pattern)),
        _row(unsigned(row))
    {
        BN_ASSERT(pattern >= 0 && pattern < (1 << 26), "Invalid pattern: ", pattern);
        BN_ASSERT(row >= 0 && row < 64, "Invalid row: ", row);
    }

    /**
     * @brief Returns the pattern order.
     *
     */
    [[nodiscard]] constexpr int pattern() const
    {
        return _pattern;
    }

    /**
     * @brief Sets the pattern order.
     */
    constexpr void set_pattern(int pattern)
    {
        BN_ASSERT(pattern >= 0 && pattern < (1 << 26), "Invalid pattern: ", pattern);

        _pattern = unsigned(pattern);
    }

    /**
     * @brief Returns the row inside the pattern.
     */
    [[nodiscard]] constexpr int row() const
    {
        return _row;
    }

    /**
     * @brief Sets the row inside the pattern.
     */
    constexpr void set_row(int row)
    {
        BN_ASSERT(row >= 0 && row < 64, "Invalid row: ", row);

        _row = unsigned(row);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(
            const dmg_music_position& a, const dmg_music_position& b) = default;

private:
    unsigned _pattern: 26 = 0;
    unsigned _row: 6 = 0;
};


/**
 * @brief Hash support for dmg_music_position.
 *
 * @ingroup dmg_music
 * @ingroup functional
 */
template<>
struct hash<dmg_music_position>
{
    /**
     * @brief Returns the hash of the given dmg_music_position.
     */
    [[nodiscard]] constexpr unsigned operator()(const dmg_music_position& value) const
    {
        return make_hash((value.pattern() << 6) + value.row());
    }
};

}

#endif

