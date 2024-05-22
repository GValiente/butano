/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_MODEL_VIEWER_ITEM_H
#define FR_MODEL_VIEWER_ITEM_H

#include "bn_string_view.h"

#include "fr_model_3d_item.h"

namespace fr
{

class model_viewer_item
{

public:
    constexpr model_viewer_item(const model_3d_item& model_item, const bn::string_view& name, bn::fixed y,
                                bn::fixed initial_phi, bn::fixed initial_theta, bn::fixed initial_psi) :
        _model_item(&model_item),
        _name(name),
        _y(y),
        _initial_phi(initial_phi),
        _initial_theta(initial_theta),
        _initial_psi(initial_psi)
    {
        BN_ASSERT(! name.empty(), "There's no name");
    }

    [[nodiscard]] constexpr const model_3d_item& model_item() const
    {
        return *_model_item;
    }

    [[nodiscard]] constexpr const bn::string_view& name() const
    {
        return _name;
    }

    [[nodiscard]] constexpr bn::fixed y() const
    {
        return _y;
    }

    [[nodiscard]] constexpr bn::fixed initial_phi() const
    {
        return _initial_phi;
    }

    [[nodiscard]] constexpr bn::fixed initial_theta() const
    {
        return _initial_theta;
    }

    [[nodiscard]] constexpr bn::fixed initial_psi() const
    {
        return _initial_psi;
    }

private:
    const model_3d_item* _model_item;
    bn::string_view _name;
    bn::fixed _y;
    bn::fixed _initial_phi;
    bn::fixed _initial_theta;
    bn::fixed _initial_psi;
};

}

#endif
