/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
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
    class static_data
    {

    public:
        timer cpu_usage_timer;
        int cpu_usage_ticks = 0;
        int vblank_usage_ticks = 0;
    };

    BN_DATA_EWRAM static_data data;

    void enable()
    {
        hblank_effects_manager::enable();
        audio_manager::enable();
    }

    void disable(bool disable_audio)
    {
        if(disable_audio)
        {
            audio_manager::disable();
        }

        hblank_effects_manager::disable();
    }

    void stop(bool disable_audio)
    {
        audio_manager::stop();
        audio_manager::disable_vblank_handler();
        hw::core::wait_for_vblank();
        audio_manager::commit();
        audio_manager::enable_vblank_handler();

        palettes_manager::stop();
        display_manager::stop();
        keypad_manager::stop();

        disable(disable_audio);
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
    hw::sram::init();

    // Init display:
    display_manager::init();

    // Init irq system:
    hw::irq::init();

    // Init hblank effects system:
    hblank_effects_manager::init();

    // Init audio system:
    audio_manager::init();

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

void update()
{
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

    BN_PROFILER_ENGINE_START("eng_cpu_usage");
    data.cpu_usage_ticks = data.cpu_usage_timer.elapsed_ticks();
    BN_PROFILER_ENGINE_STOP();

    audio_manager::disable_vblank_handler();
    hw::core::wait_for_vblank();

    BN_PROFILER_ENGINE_START("eng_cpu_usage");
    data.cpu_usage_timer.restart();
    BN_PROFILER_ENGINE_STOP();

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

    BN_PROFILER_ENGINE_START("eng_spr_tiles_commit");
    sprite_tiles_manager::commit();
    BN_PROFILER_ENGINE_STOP();

    BN_PROFILER_ENGINE_START("eng_bg_blocks_commit");
    bg_blocks_manager::commit();
    BN_PROFILER_ENGINE_STOP();

    BN_PROFILER_ENGINE_START("eng_cpu_usage");
    data.vblank_usage_ticks = data.cpu_usage_timer.elapsed_ticks();
    BN_PROFILER_ENGINE_STOP();

    BN_PROFILER_ENGINE_START("eng_audio_commit");
    audio_manager::commit();
    BN_PROFILER_ENGINE_STOP();

    audio_manager::enable_vblank_handler();

    BN_PROFILER_ENGINE_START("eng_keypad");
    keypad_manager::update();
    BN_PROFILER_ENGINE_STOP();
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

    // Sleep display:
    display_manager::sleep();

    // Disable irqs:
    disable(true);

    // Enable keypad interrupt with the specified wake up keys:
    keypad_manager::set_interrupt(wake_up_keys);
    hw::irq::replace_or_push_back(hw::irq::id::KEYPAD, nullptr);

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
}

void reset()
{
    stop(true);
    hw::core::reset();
}

fixed cpu_usage()
{
    return fixed(data.cpu_usage_ticks) / timers::ticks_per_frame();
}

fixed vblank_usage()
{
    return fixed(data.vblank_usage_ticks) / timers::ticks_per_vblank();
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
