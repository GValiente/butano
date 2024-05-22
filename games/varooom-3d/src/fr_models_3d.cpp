/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_models_3d.h"

#if FR_LOG_POLYGONS_PER_SECOND
    #include "bn_log.h"
#endif

namespace fr
{

void models_3d::set_static_model_items(const model_3d_item** static_model_items_ptr, int static_models_count)
{
    BN_ASSERT(static_models_count <= constants_3d::max_static_models, "There's no space for more static models");

    int static_vertices_count = 0;
    int static_faces_count = 0;
    _static_models_count = static_models_count;

    for(int index = 0; index < static_models_count; ++index)
    {
        const model_3d_item* static_model_item = static_model_items_ptr[index];
        static_vertices_count += static_model_item->vertices().size();
        static_faces_count += static_model_item->faces().size();
    }

    _vertices_count = _vertices_count - _static_vertices_count + static_vertices_count;
    _static_vertices_count = static_vertices_count;
    BN_ASSERT(_vertices_count <= _max_vertices, "There's no space for more vertices");

    _faces_count = _faces_count - _static_faces_count + static_faces_count;
    _static_faces_count = static_faces_count;
    BN_ASSERT(_faces_count <= _max_faces, "There's no space for more faces");

    _static_model_items_ptr = static_model_items_ptr;
}

model_3d& models_3d::create_dynamic_model(const model_3d_item& model_item)
{
    int model_vertices_count = model_item.vertices().size();
    int model_faces_count = model_item.faces().size();
    BN_ASSERT(! _dynamic_models_pool.full(), "There's no space for more dynamic models");
    BN_ASSERT(model_vertices_count + _vertices_count <= _max_vertices, "There's no space for more vertices");
    BN_ASSERT(model_faces_count + _faces_count <= _max_faces, "There's no space for more faces");

    model_3d& result = _dynamic_models_pool.create(model_item);
    _dynamic_models_list.push_back(result);
    _vertices_count += model_vertices_count;
    _faces_count += model_faces_count;
    return result;
}

void models_3d::destroy_dynamic_model(model_3d& model)
{
    const model_3d_item& model_item = model.item();
    _vertices_count -= model_item.vertices().size();
    _faces_count -= model_item.faces().size();
    _dynamic_models_list.erase(model);
    _dynamic_models_pool.destroy(model);
}

sprite_3d& models_3d::create_sprite(sprite_3d_item& sprite_item)
{
    BN_ASSERT(! _sprites_pool.full(), "There's no space for more dynamic sprites");
    BN_ASSERT(1 + _vertices_count <= _max_vertices, "There's no space for more vertices");
    BN_ASSERT(1 + _faces_count <= _max_faces, "There's no space for more faces");

    sprite_3d& result = _sprites_pool.create(sprite_item);
    _sprites_list.push_back(result);
    _vertices_count += 1;
    _faces_count += 1;
    return result;
}

void models_3d::destroy_sprite(sprite_3d& sprite)
{
    _vertices_count -= 1;
    _faces_count -= 1;
    _sprites_list.erase(sprite);
    _sprites_pool.destroy(sprite);
}

void models_3d::update(const camera_3d& camera)
{
    _process_models(camera);
    _shape_groups.update();

    #if FR_LOG_POLYGONS_PER_SECOND
        _total_faces_count += _faces_count;
        ++_update_calls;

        if(_update_calls == 60)
        {
            BN_LOG("polygons per second: ", _total_faces_count);
            _total_faces_count = 0;
            _update_calls = 0;
        }
    #endif
}

}
