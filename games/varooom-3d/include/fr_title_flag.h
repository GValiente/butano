/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef FR_TITLE_FLAG_H
#define FR_TITLE_FLAG_H

#include "fr_model_3d.h"

namespace fr
{

class models_3d;

class title_flag
{

public:
    explicit title_flag(models_3d& models);

    ~title_flag();

    [[nodiscard]] bool visible() const
    {
        return _model;
    }

    [[nodiscard]] bool moving() const
    {
        return _moving;
    }

    void show();

    void update();

private:
    models_3d& _models;
    bn::array<vertex_3d, 64> _vertices;
    bn::array<face_3d, 25> _faces;
    model_3d_item _model_item;
    model_3d* _model = nullptr;
    int _wave_angle = 0;
    bool _moving = true;
};

}

#endif
