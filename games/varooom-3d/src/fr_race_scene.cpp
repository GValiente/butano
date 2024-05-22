/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_race_scene.h"

#include "bn_profiler.h"

#include "fr_stage.h"
#include "fr_common_stuff.h"

#if FR_PROFILE
    static_assert(BN_CFG_PROFILER_ENABLED);
#endif

#if FR_DETAILED_PROFILE
    #define FR_GLOBAL_PROFILER_START(id) \
        do \
        { \
        } while(false)

    #define FR_GLOBAL_PROFILER_STOP() \
        do \
        { \
        } while(false)

    #define FR_LOCAL_PROFILER_START BN_PROFILER_START
    #define FR_LOCAL_PROFILER_STOP BN_PROFILER_STOP
#else
    #define FR_LOCAL_PROFILER_START(id) \
        do \
        { \
        } while(false)

    #define FR_LOCAL_PROFILER_STOP() \
        do \
        { \
        } while(false)

    #define FR_GLOBAL_PROFILER_START BN_PROFILER_START
    #define FR_GLOBAL_PROFILER_STOP BN_PROFILER_STOP
#endif

namespace fr
{

race_scene::race_scene(common_stuff& common_stuff) :
    _stage(stage::stage_1(common_stuff.storage.difficulty(), common_stuff.storage.reverse_race())),
    _common_stuff(common_stuff),
    _background(_stage),
    _state(_stage),
    _player_car(_stage, _state, _models, _camera, _announcer, _background),
    _rival_cars(_stage, _models),
    _foreground(_stage, _camera, _models),
    _scoreboard(_stage, common_stuff),
    _pause(common_stuff),
    _intro(_models)
{
    _models.load_colors(_stage.model_colors());
}

bn::optional<scene_type> race_scene::update()
{
    bn::optional<scene_type> result;
    bool intro_done = _intro.done();
    bool outro_active = _outro.active();
    bool in_game = intro_done && ! outro_active;

    if(in_game)
    {
        result = _pause.update(_common_stuff, _announcer);
    }

    if(! _pause.paused())
    {
        FR_GLOBAL_PROFILER_START("race_scene");

        if(in_game)
        {
            FR_LOCAL_PROFILER_START("rival_cars_update");
            _rival_cars.update(_stage, _state, _player_car, _announcer);
            FR_LOCAL_PROFILER_STOP();
        }

        if(! outro_active)
        {
            FR_LOCAL_PROFILER_START("player_car_update");
            _player_car.update(_stage, _state, intro_done, _common_stuff.storage.rumble_allowed(),
                               _announcer, _background);
            FR_LOCAL_PROFILER_STOP();
        }

        FR_LOCAL_PROFILER_START("player_car_camera");
        _player_car.update_camera(_stage, true, _camera);
        FR_LOCAL_PROFILER_STOP();

        FR_LOCAL_PROFILER_START("intro");
        _intro.update(_stage, _camera);
        FR_LOCAL_PROFILER_STOP();

        FR_LOCAL_PROFILER_START("background");
        _background.update(_stage, _camera);
        FR_LOCAL_PROFILER_STOP();

        if(_frame_index % 2)
        {
            FR_LOCAL_PROFILER_START("foreground");
            _foreground.update(_stage, _camera, _models);
            FR_LOCAL_PROFILER_STOP();
        }
        else if(in_game)
        {
            FR_LOCAL_PROFILER_START("rest");
            _foreground.check_collision(_stage, _player_car, _announcer);
            _state.update(_stage, _player_car, _rival_cars, _announcer);
            FR_LOCAL_PROFILER_STOP();
        }

        FR_LOCAL_PROFILER_START("outro");
        result = _outro.update(_stage, _state, _player_car, _common_stuff, _announcer);
        FR_LOCAL_PROFILER_STOP();

        FR_LOCAL_PROFILER_START("scoreboard");
        _scoreboard.update(_stage, _state, _player_car.turbo_energy(), intro_done);
        FR_LOCAL_PROFILER_STOP();

        _models.update(_camera);

        FR_GLOBAL_PROFILER_STOP();

        ++_frame_index;

        #if FR_PROFILE
            if(_frame_index == 2100)
            {
                bn::profiler::show();
            }
        #endif
    }

    FR_LOCAL_PROFILER_START("announcer");
    _announcer.update();
    FR_LOCAL_PROFILER_STOP();

    return result;
}

}
