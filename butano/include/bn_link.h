/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_LINK_H
#define BN_LINK_H

/**
 * @file
 * bn::link header file.
 *
 * @ingroup link
 */

#include "bn_optional.h"

namespace bn
{
    class link_state;
}

/**
 * @brief Link communication related functions.
 *
 * @ingroup link
 */
namespace bn::link
{
    /**
     * @brief Send a message to the other players.
     *
     * Keep in mind that the message can be lost before being received by other players.
     *
     * @param data Data to send, in the range [0..65533].
     */
    void send(int data);

    /**
     * @brief Returns the current state of the communication with other players.
     *
     * Keep in mind that some messages can be lost before receiving them.
     */
    [[nodiscard]] optional<link_state> receive();

    /**
     * @brief Deactivates the communication with other players until send() or receive() are called.
     */
    void deactivate();
}

#endif
