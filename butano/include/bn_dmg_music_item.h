/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DMG_MUSIC_ITEM_H
#define BN_DMG_MUSIC_ITEM_H

/**
 * @file
 * bn::dmg_music_item header file.
 *
 * @ingroup dmg_music
 * @ingroup tool
 */

#include "bn_functional.h"

namespace bn
{

/**
 * @brief Contains the required information to play DMG music.
 *
 * The assets conversion tools generate an object of this type in the build folder for each module file.
 *
 * Module files with `*.mod` and `*.s3m` extensions are supported.
 *
 * @ingroup dmg_music
 * @ingroup tool
 */
class dmg_music_item
{

public:
    /**
     * @brief Constructor.
     * @param data_ref Reference to the song data.
     *
     * Song data is not copied but referenced, so it should outlive the dmg_music_item
     * to avoid dangling references.
     */
    constexpr explicit dmg_music_item(const uint8_t& data_ref) :
        _data_ptr(&data_ref)
    {
    }

    /**
     * @brief Returns a pointer to the referenced song data.
     */
    [[nodiscard]] constexpr const uint8_t* data_ptr() const
    {
        return _data_ptr;
    }

    /**
     * @brief Returns the referenced song data.
     */
    [[nodiscard]] constexpr const uint8_t& data_ref() const
    {
        return *_data_ptr;
    }

    /**
     * @brief Plays the DMG music specified by this item with default settings.
     *
     * Default settings are speed = 1 and loop enabled.
     */
    void play() const;

    /**
     * @brief Plays the DMG music specified by this item.
     * @param speed Playback speed, in the range [1..256].
     */
    void play(int speed) const;

    /**
     * @brief Plays the DMG music specified by this item.
     * @param speed Playback speed, in the range [1..256].
     * @param loop Indicates if it must be played until it is stopped manually or until end.
     */
    void play(int speed, bool loop) const;

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(dmg_music_item a, dmg_music_item b) = default;

private:
    const uint8_t* _data_ptr;
};


/**
 * @brief Hash support for dmg_music_item.
 *
 * @ingroup dmg_music
 * @ingroup functional
 */
template<>
struct hash<dmg_music_item>
{
    /**
     * @brief Returns the hash of the given dmg_music_item.
     */
    [[nodiscard]] constexpr unsigned operator()(dmg_music_item value) const
    {
        return make_hash(value.data_ptr());
    }
};

}

#endif

