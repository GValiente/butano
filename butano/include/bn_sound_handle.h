/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SOUND_HANDLE_H
#define BN_SOUND_HANDLE_H

/**
 * @file
 * bn::sound_handle header file.
 *
 * @ingroup sound
 */

#include "bn_fixed.h"

namespace bn
{

class sound_item;

/**
 * @brief Sound effect handle.
 *
 * @ingroup sound
 */
class sound_handle
{

public:
    /**
     * @brief Returns the internal id.
     */
    [[nodiscard]] int id() const
    {
        return _id;
    }

    /**
     * @brief Indicates if this sound effect is active or not.
     */
    [[nodiscard]] bool active() const;

    /**
     * @brief Returns the sound_item of this sound effect.
     */
    [[nodiscard]] sound_item item() const;

    /**
     * @brief Stops playback of this sound effect.
     */
    void stop();

    /**
     * @brief Releases this sound effect, allowing its interruption.
     */
    void release();

    /**
     * @brief Returns the playback rate of this sound effect.
     */
    [[nodiscard]] fixed speed() const;

    /**
     * @brief Sets the playback rate of this sound effect.
     * @param speed Playback rate, in the range [0..64].
     */
    void set_speed(fixed speed);

    /**
     * @brief Returns the panning level of this sound effect.
     */
    [[nodiscard]] fixed panning() const;

    /**
     * @brief Sets the panning level of this sound effect.
     * @param panning Panning level, in the range [-1..1].
     */
    void set_panning(fixed panning);

private:
    friend class sound_handle_generator;

    uint16_t _id;

    explicit sound_handle(uint16_t id) :
        _id(id)
    {
    }
};


/**
 * @brief Hash support for sound_handle.
 *
 * @ingroup sound
 * @ingroup functional
 */
template<>
struct hash<sound_handle>
{
    /**
     * @brief Returns the hash of the given sound_handle.
     */
    [[nodiscard]] unsigned operator()(sound_handle value) const
    {
        return make_hash(value.id());
    }
};

}

#endif

