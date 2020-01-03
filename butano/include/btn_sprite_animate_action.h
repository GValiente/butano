#ifndef BTN_SPRITE_ANIMATE_ACTION_H
#define BTN_SPRITE_ANIMATE_ACTION_H

#include "btn_array.h"
#include "btn_action.h"
#include "btn_limits.h"
#include "btn_utility.h"
#include "btn_sprite_ptr.h"
#include "btn_sprite_item.h"

namespace btn
{

template<size_t Size>
class sprite_animate_action : public action
{
    static_assert(Size);

public:
    [[nodiscard]] static sprite_animate_action once(sprite_ptr sprite, int wait_frames,
                                                    const sprite_tiles_item& tiles_item,
                                                    const array<uint16_t, Size>& graphic_ids)
    {
        return sprite_animate_action(move(sprite), wait_frames, tiles_item, false, graphic_ids);
    }

    [[nodiscard]] static sprite_animate_action once(sprite_ptr sprite, int wait_frames, const sprite_item& item,
                                                    const array<uint16_t, Size>& graphic_ids)
    {
        return sprite_animate_action(move(sprite), wait_frames, item.tiles_item(), false, graphic_ids);
    }

    [[nodiscard]] static sprite_animate_action forever(sprite_ptr sprite, int wait_frames,
                                                       const sprite_tiles_item& tiles_item,
                                                       const array<uint16_t, Size>& graphic_ids)
    {
        return sprite_animate_action(move(sprite), wait_frames, tiles_item, true, graphic_ids);
    }

    [[nodiscard]] static sprite_animate_action forever(sprite_ptr sprite, int wait_frames, const sprite_item& item,
                                                       const array<uint16_t, Size>& graphic_ids)
    {
        return sprite_animate_action(move(sprite), wait_frames, item.tiles_item(), true, graphic_ids);
    }

    void reset()
    {
        _current_graphics_id_index = 0;
        _current_wait_frames = 0;
    }

    void update() override
    {
        BTN_ASSERT(! done(), "Action is done");

        if(_current_wait_frames)
        {
            --_current_wait_frames;
        }
        else
        {
            _current_wait_frames = _wait_frames;
            _sprite.set_tiles(_tiles_item, _graphic_ids[_current_graphics_id_index]);
            ++_current_graphics_id_index;

            if(_forever && _current_graphics_id_index == _graphic_ids.size())
            {
                _current_graphics_id_index = 0;
            }
        }
    }

    [[nodiscard]] bool done() const override
    {
        return _current_graphics_id_index == _graphic_ids.size();
    }

    [[nodiscard]] const sprite_ptr& sprite() const
    {
        return _sprite;
    }

    [[nodiscard]] int wait_frames() const
    {
        return _wait_frames;
    }

    [[nodiscard]] const sprite_tiles_item& tiles_item() const
    {
        return _tiles_item;
    }

    [[nodiscard]] const array<uint16_t, Size>& graphic_ids() const
    {
        return _graphic_ids;
    }

    [[nodiscard]] bool update_forever() const
    {
        return _forever;
    }

protected:
    sprite_ptr _sprite;
    sprite_tiles_item _tiles_item;
    array<uint16_t, Size> _graphic_ids;
    uint16_t _wait_frames = 0;
    uint16_t _current_graphics_id_index = 0;
    uint16_t _current_wait_frames = 0;
    bool _forever = true;

    sprite_animate_action(sprite_ptr sprite, int wait_frames, const sprite_tiles_item& tiles_item, bool forever,
                          const array<uint16_t, Size>& graphic_ids) :
        _sprite(move(sprite)),
        _tiles_item(tiles_item),
        _graphic_ids(graphic_ids),
        _wait_frames(uint16_t(wait_frames)),
        _forever(forever)
    {
        BTN_ASSERT(wait_frames >= 0, "Invalid wait frames: ", wait_frames);
        BTN_ASSERT(wait_frames <= integral_limits<decltype(_wait_frames)>::max, "Too much wait frames: ", wait_frames);
    }
};

template<typename ...Args>
[[nodiscard]] inline auto create_sprite_animate_action_once(sprite_ptr sprite, int wait_frames,
                                                            const sprite_tiles_item& tiles_item, Args ...graphic_ids)
{
    return sprite_animate_action<sizeof...(Args)>::once(move(sprite), wait_frames, tiles_item,
                                                        array<uint16_t, sizeof...(Args)>{{ uint16_t(graphic_ids)... }});
}

template<typename ...Args>
[[nodiscard]] inline auto create_sprite_animate_action_once(sprite_ptr sprite, int wait_frames,
                                                            const sprite_item& item, Args ...graphic_ids)
{
    return sprite_animate_action<sizeof...(Args)>::once(move(sprite), wait_frames, item,
                                                        array<uint16_t, sizeof...(Args)>{{ uint16_t(graphic_ids)... }});
}

template<typename ...Args>
[[nodiscard]] inline auto create_sprite_animate_action_forever(sprite_ptr sprite, int wait_frames,
                                                               const sprite_tiles_item& tiles_item, Args ...graphic_ids)
{
    return sprite_animate_action<sizeof...(Args)>::forever(move(sprite), wait_frames, tiles_item,
                                                           array<uint16_t, sizeof...(Args)>{{ uint16_t(graphic_ids)... }});
}

template<typename ...Args>
[[nodiscard]] inline auto create_sprite_animate_action_forever(sprite_ptr sprite, int wait_frames,
                                                               const sprite_item& item, Args ...graphic_ids)
{
    return sprite_animate_action<sizeof...(Args)>::forever(move(sprite), wait_frames, item,
                                                           array<uint16_t, sizeof...(Args)>{{ uint16_t(graphic_ids)... }});
}

}

#endif
