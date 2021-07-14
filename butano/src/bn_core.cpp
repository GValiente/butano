/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"

#include "bn_span.h"
#include "bn_fixed.h"
#include "bn_timer.h"
#include "bn_string.h"
#include "bn_keypad.h"
#include "bn_timers.h"
#include "bn_profiler.h"
#include "bn_string_view.h"
#include "bn_bgs_manager.h"
#include "bn_hdma_manager.h"
#include "bn_link_manager.h"
#include "bn_gpio_manager.h"
#include "bn_audio_manager.h"
#include "bn_keypad_manager.h"
#include "bn_memory_manager.h"
#include "bn_display_manager.h"
#include "bn_sprites_manager.h"
#include "bn_cameras_manager.h"
#include "bn_palettes_manager.h"
#include "bn_bg_blocks_manager.h"
#include "bn_sprite_tiles_manager.h"
#include "bn_hblank_effects_manager.h"
#include "../hw/include/bn_hw_irq.h"
#include "../hw/include/bn_hw_core.h"
#include "../hw/include/bn_hw_sram.h"
#include "../hw/include/bn_hw_timer.h"
#include "../hw/include/bn_hw_memory.h"
#include "../hw/include/bn_hw_game_pak.h"

#if BN_CFG_ASSERT_ENABLED
    #include "bn_string_view.h"
#endif

#if BN_CFG_ASSERT_ENABLED || BN_CFG_PROFILER_ENABLED
    #include "../hw/include/bn_hw_show.h"
#endif

#if BN_CFG_PROFILER_ENABLED && BN_CFG_PROFILER_LOG_ENGINE
    #define BN_PROFILER_ENGINE_START(id) \
        BN_PROFILER_START(id)

    #define BN_PROFILER_ENGINE_STOP() \
        BN_PROFILER_STOP()
#else
    #define BN_PROFILER_ENGINE_START(id) \
        do \
        { \
        } while(false)

    #define BN_PROFILER_ENGINE_STOP() \
        do \
        { \
        } while(false)
#endif

namespace bn::core
{

namespace
{
    class ticks
    {

    public:
        int cpu_usage_ticks = 0;
        int vblank_usage_ticks = 0;
    };

    class static_data
    {

    public:
        timer cpu_usage_timer;
        ticks last_ticks;
        int skip_frames = 0;
        int last_update_frames = 1;
        bool restart_cpu_usage_timer = false;
    };

    BN_DATA_EWRAM static_data data;

    void hp_vblank_function()
    {
        if(data.restart_cpu_usage_timer)
        {
            data.cpu_usage_timer.restart();
            data.restart_cpu_usage_timer = false;
        }
        else
        {
            hdma_manager::commit();
        }
    }

    void enable()
    {
        hblank_effects_manager::enable();
        link_manager::enable();
        audio_manager::enable();
        hdma_manager::enable();
    }

    void disable(bool disable_audio)
    {
        hdma_manager::disable();

        if(disable_audio)
        {
            audio_manager::disable();
        }

        link_manager::disable();
        hblank_effects_manager::disable();
    }

    void stop(bool disable_audio)
    {
        audio_manager::stop();
        audio_manager::disable_vblank_handler();
        hw::core::wait_for_vblank();
        audio_manager::commit();

        hdma_manager::force_stop();

        palettes_manager::stop();
        bgs_manager::stop();
        display_manager::stop();
        keypad_manager::stop();
        gpio_manager::stop();

        disable(disable_audio);
    }

    [[nodiscard]] ticks update_impl()
    {
        ticks result;

        BN_PROFILER_ENGINE_START("eng_cameras_update");
        cameras_manager::update();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_sprites_update");
        sprites_manager::update();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_spr_tiles_update");
        sprite_tiles_manager::update();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_bgs_update");
        bgs_manager::update();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_bg_blocks_update");
        bg_blocks_manager::update();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_palettes_update");
        palettes_manager::update();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_display_update");
        display_manager::update();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_hblank_fx_update");
        hblank_effects_manager::update();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_hdma_update");
        hdma_manager::update();
        BN_PROFILER_ENGINE_STOP();

        audio_manager::disable_vblank_handler();

        result.cpu_usage_ticks = data.cpu_usage_timer.elapsed_ticks();
        data.restart_cpu_usage_timer = true;

        hw::core::wait_for_vblank();

        BN_PROFILER_ENGINE_START("eng_hblank_fx_commit");
        hblank_effects_manager::commit();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_display_commit");
        display_manager::commit();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_sprites_commit");
        sprites_manager::commit();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_bgs_commit");
        bgs_manager::commit();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_palettes_commit");
        palettes_manager::commit();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_hdma_commit");
        hdma_manager::commit();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_spr_tiles_commit");
        sprite_tiles_manager::commit();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_big_maps_commit");
        bgs_manager::commit_big_maps();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_bg_blocks_commit");
        bg_blocks_manager::commit();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_cpu_usage");
        result.vblank_usage_ticks = data.cpu_usage_timer.elapsed_ticks();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_audio_commit");
        audio_manager::commit();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_gpio_commit");
        gpio_manager::commit();
        BN_PROFILER_ENGINE_STOP();

        BN_PROFILER_ENGINE_START("eng_keypad");
        keypad_manager::update();
        BN_PROFILER_ENGINE_STOP();

        return result;
    }
}

void init()
{
    init(string_view());
}

void init(const string_view& keypad_commands)
{
    // Init storage systems:
    hw::game_pak::init();
    hw::memory::init();
    hw::sram::init();

    // Init display:
    display_manager::init();

    // Init irq system:
    hw::irq::init();

    // Init H-Blank effects system:
    hblank_effects_manager::init();

    // Init audio system:
    audio_manager::init(hp_vblank_function, link_manager::commit);

    // Init link system:
    link_manager::init();

    // Init high level systems:
    memory_manager::init();
    cameras_manager::init();
    sprite_tiles_manager::init();
    sprites_manager::init();
    bg_blocks_manager::init();
    keypad_manager::init(keypad_commands);

    // WTF hack (if it isn't present and flto is enabled, sometimes everything crash):
    string<32> hack_string;
    ostringstream hack_string_stream(hack_string);
    hack_string_stream.append(2);

    // Init timer system:
    hw::timer::init();
    data.cpu_usage_timer.restart();

    // First update:
    update();

    // Keypad polling fix:
    keypad_manager::update();

    // Reset profiler:
    BN_PROFILER_RESET();
}

int skip_frames()
{
    return data.skip_frames;
}

void set_skip_frames(int skip_frames)
{
    BN_ASSERT(skip_frames >= 0, "Invalid skip frames: ", skip_frames);

    data.skip_frames = skip_frames;
}

void update()
{
    int update_frames = data.skip_frames + 1;
    data.last_update_frames = update_frames;

    if(update_frames == 1)
    {
        data.last_ticks = update_impl();
    }
    else
    {
        ticks total_ticks;

        for(int frame_index = 0; frame_index < update_frames; ++frame_index)
        {
            ticks frame_ticks = update_impl();
            frame_index += frame_ticks.cpu_usage_ticks / timers::ticks_per_frame();
            total_ticks.cpu_usage_ticks += frame_ticks.cpu_usage_ticks;
            total_ticks.vblank_usage_ticks += frame_ticks.vblank_usage_ticks;
        }

        data.last_ticks = total_ticks;
    }
}

void sleep(keypad::key_type wake_up_key)
{
    const keypad::key_type wake_up_keys[] = { wake_up_key };
    sleep(wake_up_keys);
}

void sleep(const span<const keypad::key_type>& wake_up_keys)
{
    BN_ASSERT(! wake_up_keys.empty(), "There's no wake up keys");

    // Wait until a wake up key is not pressed:
    while(true)
    {
        bool wait = true;

        for(keypad::key_type wake_up_key : wake_up_keys)
        {
            if(! keypad::held(wake_up_key))
            {
                wait = false;
                break;
            }
        }

        if(wait)
        {
            update();
        }
        else
        {
            break;
        }
    }

    // Sleep gpio:
    gpio_manager::sleep();

    // Sleep display:
    display_manager::sleep();

    // Disable irqs:
    disable(true);

    // Enable keypad interrupt with the specified wake up keys:
    keypad_manager::set_interrupt(wake_up_keys);
    hw::irq::replace_or_push_back_enabled(hw::irq::id::KEYPAD, nullptr);

    // Enable sleep mode:
    hw::core::sleep();

    // Remove keypad interrupt:
    hw::irq::remove(hw::irq::id::KEYPAD);

    // Enable irqs:
    enable();

    // Update keypad:
    keypad_manager::update();

    // Wait for vblank:
    hw::core::wait_for_vblank();

    // Restart CPU usage timer:
    BN_PROFILER_ENGINE_START("eng_cpu_usage");
    data.cpu_usage_timer.restart();
    BN_PROFILER_ENGINE_STOP();

    // Wake up display:
    display_manager::wake_up();

    // Wake up gpio:
    gpio_manager::wake_up();
}

void reset()
{
    stop(true);
    hw::core::reset();
}

fixed current_cpu_usage()
{
    int current_cpu_usage_ticks = data.cpu_usage_timer.elapsed_ticks();
    return fixed(current_cpu_usage_ticks) / timers::ticks_per_frame();
}

fixed last_cpu_usage()
{
    return fixed(data.last_ticks.cpu_usage_ticks) / (timers::ticks_per_frame() * data.last_update_frames);
}

fixed last_vblank_usage()
{
    return fixed(data.last_ticks.vblank_usage_ticks) / (timers::ticks_per_vblank() * data.last_update_frames);
}

}

#if BN_CFG_ASSERT_ENABLED
    namespace _bn::assert
    {
        void show(const char* condition, const char* file_name, const char* function, int line, const char* message)
        {
            bn::core::stop(false);
            bn::hw::show::error(condition, file_name, function, line, message);

            while(true)
            {
                bn::hw::core::wait_for_vblank();
            }
        }

        void show(const char* condition, const char* file_name, const char* function, int line,
                  const bn::istring_base& message)
        {
            bn::core::stop(false);
            bn::hw::show::error(condition, file_name, function, line, message);

            while(true)
            {
                bn::hw::core::wait_for_vblank();
            }
        }
    }
#endif

#if BN_CFG_PROFILER_ENABLED
    namespace bn::profiler
    {
        void show()
        {
            core::stop(false);
            hw::show::profiler_results();
        }
    }
#endif
