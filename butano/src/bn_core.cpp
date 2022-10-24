/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_core_lock.h"

#include "bn_timer.h"
#include "bn_keypad.h"
#include "bn_timers.h"
#include "bn_version.h"
#include "bn_profiler.h"
#include "bn_system_font.h"
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
#include "../hw/include/bn_hw_hblank_effects.h"

#if BN_CFG_ASSERT_ENABLED || BN_CFG_PROFILER_ENABLED
    #include "../hw/include/bn_hw_show.h"
#endif

#if BN_CFG_PROFILER_ENABLED && BN_CFG_PROFILER_LOG_ENGINE
    #if BN_CFG_PROFILER_LOG_ENGINE_DETAILED
        #define BN_PROFILER_ENGINE_GENERAL_START(id) \
            do \
            { \
            } while(false)

        #define BN_PROFILER_ENGINE_GENERAL_STOP() \
            do \
            { \
            } while(false)

        #define BN_PROFILER_ENGINE_DETAILED_START(id) \
            BN_PROFILER_START(id)

        #define BN_PROFILER_ENGINE_DETAILED_STOP() \
            BN_PROFILER_STOP()
    #else
        #define BN_PROFILER_ENGINE_GENERAL_START(id) \
            BN_PROFILER_START(id)

        #define BN_PROFILER_ENGINE_GENERAL_STOP() \
            BN_PROFILER_STOP()

        #define BN_PROFILER_ENGINE_DETAILED_START(id) \
            do \
            { \
            } while(false)

        #define BN_PROFILER_ENGINE_DETAILED_STOP() \
            do \
            { \
            } while(false)
    #endif
#else
    #define BN_PROFILER_ENGINE_GENERAL_START(id) \
        do \
        { \
        } while(false)

    #define BN_PROFILER_ENGINE_GENERAL_STOP() \
        do \
        { \
        } while(false)

    #define BN_PROFILER_ENGINE_DETAILED_START(id) \
        do \
        { \
        } while(false)

    #define BN_PROFILER_ENGINE_DETAILED_STOP() \
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
        vblank_callback_type vblank_callback = nullptr;
        timer cpu_usage_timer;
        ticks last_ticks;
        bn::system_font system_font;
        string_view assert_tag = BN_VERSION_STRING;
        int skip_frames = 0;
        int last_update_frames = 1;
        bool slow_game_pak = false;
        volatile bool waiting_for_vblank = false;
    };

    BN_DATA_EWRAM static_data data;

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

        if(disable_audio)
        {
            audio_manager::disable_vblank_handler();
        }

        hw::core::wait_for_vblank();
        audio_manager::update();

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

        BN_PROFILER_ENGINE_GENERAL_START("eng_update");

        BN_PROFILER_ENGINE_DETAILED_START("eng_cameras_update");
        cameras_manager::update();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_sprites_update");
        sprites_manager::update();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_spr_tiles_update");
        sprite_tiles_manager::update();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_bgs_update");
        bgs_manager::update();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_bg_blocks_update");
        bg_blocks_manager::update();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_palettes_update");
        palettes_manager::update();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_display_update");
        display_manager::update();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_hblank_fx_update");
        hblank_effects_manager::update();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        bool use_dma = ! link_manager::active();

        BN_PROFILER_ENGINE_GENERAL_STOP();

        result.cpu_usage_ticks = data.cpu_usage_timer.elapsed_ticks();
        data.waiting_for_vblank = true;

        hw::core::wait_for_vblank();

        data.cpu_usage_timer.restart();

        BN_PROFILER_ENGINE_GENERAL_START("eng_commit");

        BN_PROFILER_ENGINE_DETAILED_START("eng_audio_commands");
        audio_manager::execute_commands();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_display_commit");
        display_manager::commit();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_sprites_commit");
        sprites_manager::commit(use_dma);
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_bgs_commit");
        bgs_manager::commit(use_dma);
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_palettes_commit");
        palettes_manager::commit(use_dma);
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_spr_tiles_unc_commit");
        sprite_tiles_manager::commit_uncompressed(use_dma);
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_hdma_update");
        hdma_manager::update();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        hdma_manager::commit(use_dma);

        BN_PROFILER_ENGINE_DETAILED_START("eng_hblank_fx_commit");
        hblank_effects_manager::commit();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_spr_tiles_cmp_commit");
        sprite_tiles_manager::commit_compressed();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_big_maps_commit");
        bgs_manager::commit_big_maps();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_bg_blocks_commit");
        bg_blocks_manager::commit();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_vblank_callback");
        if(vblank_callback_type vblank_callback = data.vblank_callback)
        {
            vblank_callback();
        }
        BN_PROFILER_ENGINE_DETAILED_STOP();

        result.vblank_usage_ticks = data.cpu_usage_timer.elapsed_ticks();

        BN_PROFILER_ENGINE_DETAILED_START("eng_audio_commit");
        audio_manager::commit();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_gpio_commit");
        gpio_manager::commit();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_GENERAL_STOP();

        return result;
    }
}

void init()
{
    init(string_view());
}

void init(const string_view& keypad_commands)
{
    // Init irq system:
    hw::irq::init();
    hw::irq::set_isr(hw::irq::id::HBLANK, hw::hblank_effects::_intr);

    // Init audio system:
    audio_manager::init();

    // Init storage systems:
    data.slow_game_pak = hw::game_pak::init();
    hw::memory::init();

    [[maybe_unused]] const char* sram_type = hw::sram::init();

    // Init display:
    display_manager::init();

    // Init H-Blank effects system:
    hblank_effects_manager::init();

    // Init link system:
    link_manager::init();

    // Init high level systems:
    memory_manager::init();
    cameras_manager::init();
    sprite_tiles_manager::init();
    sprites_manager::init();
    bg_blocks_manager::init();
    keypad_manager::init(keypad_commands);

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

    BN_PROFILER_ENGINE_DETAILED_START("eng_keypad");
    keypad_manager::update();
    BN_PROFILER_ENGINE_DETAILED_STOP();
}

void on_vblank()
{
    if(data.waiting_for_vblank)
    {
        BN_PROFILER_ENGINE_DETAILED_START("eng_audio_update");
        audio_manager::update();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        data.waiting_for_vblank = false;
    }
    else
    {
        hdma_manager::commit(false);
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
    bool wait = true;

    while(wait)
    {
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
    }

    // Lock core updates:
    core_lock lock;

    // Sleep display:
    display_manager::sleep();

    // Enable keypad interrupt with the specified wake up keys:
    keypad_manager::set_interrupt(wake_up_keys);
    hw::irq::enable(hw::irq::id::KEYPAD);

    // Enable sleep mode:
    hw::core::sleep();

    // Remove keypad interrupt:
    hw::irq::disable(hw::irq::id::KEYPAD);
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

vblank_callback_type vblank_callback()
{
    return data.vblank_callback;
}

void set_vblank_callback(vblank_callback_type vblank_callback)
{
    data.vblank_callback = vblank_callback;
}

bool slow_game_pak()
{
    return data.slow_game_pak;
}

const bn::system_font& system_font()
{
    return data.system_font;
}

void set_system_font(const bn::system_font& font)
{
    data.system_font = font;
}

const string_view& assert_tag()
{
    return data.assert_tag;
}

void set_assert_tag(const string_view& assert_tag)
{
    data.assert_tag = assert_tag;
}

}

namespace bn
{

core_lock::core_lock()
{
    // Update core before disabling irqs:
    core::update();

    // Sleep gpio:
    gpio_manager::sleep();

    // Disable irqs:
    core::disable(true);
}

core_lock::~core_lock()
{
    // Enable irqs:
    core::enable();

    // Update keypad:
    keypad_manager::update();

    // Wait for vblank:
    hw::core::wait_for_vblank();

    // Restart CPU usage timer:
    core::data.cpu_usage_timer.restart();

    // Wake up display (maybe display_manager::sleep() has not been called):
    display_manager::wake_up();

    // Wake up gpio:
    gpio_manager::wake_up();
}

}

#if BN_CFG_ASSERT_ENABLED
    namespace _bn::assert
    {
        void show(const char* file_name, const char* function, int line)
        {
            show("", file_name, function, line, "");
        }

        void show(const char* condition, const char* file_name, const char* function, int line)
        {
            show(condition, file_name, function, line, "");
        }

        void show(const char* condition, const char* file_name, const char* function, int line, const char* message)
        {
            bn::core::stop(true);
            bn::hw::show::error(bn::core::system_font(), condition, file_name, function, line, message,
                                bn::core::assert_tag());

            while(true)
            {
                bn::hw::core::wait_for_vblank();
            }
        }

        void show(const char* condition, const char* file_name, const char* function, int line,
                  const bn::istring_base& message)
        {
            bn::core::stop(true);
            bn::hw::show::error(bn::core::system_font(), condition, file_name, function, line, message,
                                bn::core::assert_tag());

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
            hw::show::profiler_results(core::system_font());
        }
    }
#endif
