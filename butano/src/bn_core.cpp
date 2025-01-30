/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_core_lock.h"

#include "bn_color.h"
#include "bn_timer.h"
#include "bn_keypad.h"
#include "bn_memory.h"
#include "bn_timers.h"
#include "bn_profiler.h"
#include "bn_system_font.h"
#include "bn_bgs_manager.h"
#include "bn_hdma_manager.h"
#include "bn_link_manager.h"
#include "bn_gpio_manager.h"
#include "bn_audio_manager.h"
#include "bn_config_assert.h"
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
#include "../hw/include/bn_hw_gpio.h"
#include "../hw/include/bn_hw_sram.h"
#include "../hw/include/bn_hw_timer.h"
#include "../hw/include/bn_hw_memory.h"
#include "../hw/include/bn_hw_game_pak.h"
#include "../hw/include/bn_hw_hblank_effects.h"

#if BN_CFG_ASSERT_ENABLED
    #include "bn_assert_callback_type.h"
#endif

#if BN_CFG_ASSERT_ENABLED || BN_CFG_PROFILER_ENABLED
    #include "../hw/include/bn_hw_show.h"
#endif

#ifdef BN_STACKTRACE
    #if BN_CFG_LOG_ENABLED
        #include "../hw/include/bn_hw_stacktrace.h"
    #endif
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
        int missed_frames = 0;
    };

    class static_data
    {

    public:
        vblank_callback_type vblank_callback = nullptr;
        #if BN_CFG_ASSERT_ENABLED
            assert::callback_type assert_callback = nullptr;
        #endif
        timer cpu_usage_timer;
        ticks last_ticks;
        bn::system_font system_font;
        string_view assert_tag = BN_CFG_ASSERT_TAG;
        int skip_frames = 0;
        int last_update_frames = 1;
        int missed_frames = 0;
        bool dma_enabled = true;
        bool slow_game_pak = false;
        volatile bool waiting_for_vblank = false;
    };

    BN_DATA_EWRAM_BSS static_data data;

    void enable()
    {
        hblank_effects_manager::enable();
        link_manager::enable();
        audio_manager::enable();
        hdma_manager::enable();
    }

    void disable(bool disable_vblank_irq)
    {
        hdma_manager::disable();

        if(disable_vblank_irq)
        {
            audio_manager::disable();
        }

        link_manager::disable();
        hblank_effects_manager::disable();
    }

    void stop(bool disable_vblank_irq)
    {
        hw::core::wait_for_vblank();

        audio_manager::stop();
        hdma_manager::force_stop();
        hblank_effects_manager::stop();
        palettes_manager::stop();
        bgs_manager::stop();
        display_manager::stop();
        keypad_manager::stop();
        gpio_manager::stop();

        disable(disable_vblank_irq);
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

        bool use_dma = data.dma_enabled && ! link_manager::active();

        BN_PROFILER_ENGINE_GENERAL_STOP();

        BN_BARRIER;
        result.cpu_usage_ticks = data.cpu_usage_timer.elapsed_ticks();

        BN_BARRIER;
        data.waiting_for_vblank = true;

        hw::core::wait_for_vblank();

        BN_BARRIER;
        data.cpu_usage_timer.restart();

        BN_PROFILER_ENGINE_GENERAL_START("eng_commit");

        BN_BARRIER;
        result.missed_frames = data.missed_frames;
        data.missed_frames = 0;

        BN_PROFILER_ENGINE_DETAILED_START("eng_hblank_fx_commit");
        hblank_effects_manager::disable();
        BN_PROFILER_ENGINE_DETAILED_STOP();

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

        bool hdma_running = hdma_manager::commit(use_dma);

        BN_PROFILER_ENGINE_DETAILED_START("eng_hblank_fx_commit");
        bool hblank_effects_running = hblank_effects_manager::commit();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_big_maps_commit");
        bgs_manager::commit_big_maps();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        use_dma = use_dma && ! hdma_running && ! hblank_effects_running;

        BN_PROFILER_ENGINE_DETAILED_START("eng_bg_blocks_unc_commit");
        bg_blocks_manager::commit_uncompressed(use_dma);
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_spr_tiles_cmp_commit");
        sprite_tiles_manager::commit_compressed();
        BN_PROFILER_ENGINE_DETAILED_STOP();

        BN_PROFILER_ENGINE_DETAILED_START("eng_bg_blocks_cmp_commit");
        bg_blocks_manager::commit_compressed();
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

        BN_PROFILER_ENGINE_GENERAL_STOP();

        return result;
    }
}

void init()
{
    init(nullopt, string_view());
}

void init(const optional<color>& transparent_color)
{
    init(transparent_color, string_view());
}

void init(const string_view& keypad_commands)
{
    init(nullopt, keypad_commands);
}

void init(const optional<color>& transparent_color, const string_view& keypad_commands)
{
    ::new(static_cast<void*>(&data)) static_data();

    // Initial wait:
    hw::core::init();

    // Init H-Blank effects system:
    hblank_effects_manager::init();

    // Init irq system:
    hw::irq::init();
    hw::irq::set_isr(hw::irq::id::HBLANK, hw::hblank_effects::_intr);

    // Init hdma system:
    hdma_manager::init();

    // Init link system:
    link_manager::init();

    // Init audio system:
    audio_manager::init();

    // Init storage systems:
    data.slow_game_pak = hw::game_pak::init();
    hw::memory::init();

    [[maybe_unused]] const char* sram_string = hw::sram::init();

    // Init gpio:
    [[maybe_unused]] const char* rtc_string = hw::gpio::init();

    // Init display:
    display_manager::init();

    // Init high level systems:
    memory_manager::init();
    cameras_manager::init();
    palettes_manager::init(transparent_color);
    sprite_tiles_manager::init();
    sprites_manager::init();
    bg_blocks_manager::init();
    bgs_manager::init();
    keypad_manager::init(keypad_commands);

    // First update:
    update();

    // Keypad polling fix:
    keypad_manager::update();

    // Init timer system:
    hw::timer::init();
    data.cpu_usage_timer.restart();
    data.last_ticks = ticks();

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
        int frame_index = 0;

        while(frame_index < update_frames)
        {
            ticks frame_ticks = update_impl();
            total_ticks.cpu_usage_ticks += frame_ticks.cpu_usage_ticks;
            total_ticks.vblank_usage_ticks = bn::max(total_ticks.vblank_usage_ticks, frame_ticks.vblank_usage_ticks);
            frame_index += frame_ticks.missed_frames + 1;
        }

        total_ticks.missed_frames = frame_index - update_frames;
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

        ++data.missed_frames;
    }
}

void sleep(keypad::key_type wake_up_key)
{
    const keypad::key_type wake_up_keys[] = { wake_up_key };
    sleep(wake_up_keys);
}

void sleep(const span<const keypad::key_type>& wake_up_keys)
{
    BN_BASIC_ASSERT(! wake_up_keys.empty(), "There are no keys");

    // Force at least one update:
    update();

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

void hard_reset()
{
    stop(true);
    hw::core::hard_reset();
}

fixed current_cpu_usage()
{
    int current_cpu_usage_ticks = data.cpu_usage_timer.elapsed_ticks();
    int current_update_frames = data.skip_frames + 1;
    return fixed(current_cpu_usage_ticks) / (timers::ticks_per_frame() * current_update_frames);
}

int current_cpu_ticks()
{
    return data.cpu_usage_timer.elapsed_ticks();
}

fixed last_cpu_usage()
{
    return fixed(data.last_ticks.cpu_usage_ticks) / (timers::ticks_per_frame() * data.last_update_frames);
}

int last_cpu_ticks()
{
    return data.last_ticks.cpu_usage_ticks;
}

fixed last_vblank_usage()
{
    return fixed(data.last_ticks.vblank_usage_ticks) / timers::ticks_per_vblank();
}

int last_vblank_ticks()
{
    return data.last_ticks.vblank_usage_ticks;
}

int last_missed_frames()
{
    return data.last_ticks.missed_frames;
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

void log_stacktrace()
{
    #if BN_CFG_LOG_ENABLED
        #ifdef BN_STACKTRACE
            bn::hw::stacktrace::log(3);
        #else
            BN_ERROR("Stack trace logging is disabled");
        #endif
    #endif
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


namespace bn::memory
{

bool dma_enabled()
{
    return core::data.dma_enabled;
}

void set_dma_enabled(bool dma_enabled)
{
    core::data.dma_enabled = dma_enabled;
}

}

#if BN_CFG_ASSERT_ENABLED
    namespace bn::assert
    {
        callback_type callback()
        {
            return core::data.assert_callback;
        }

        void set_callback(callback_type callback)
        {
            core::data.assert_callback = callback;
        }
    }

    namespace _bn::assert
    {
        namespace
        {
            [[noreturn]] void _show_impl(
                    const char* condition, const char* file_name, const char* function, int line,
                    const bn::string_view& message)
            {
                if(bn::assert::callback_type assert_callback = bn::core::data.assert_callback)
                {
                    assert_callback();
                }

                bn::core::stop(false);
                bn::hw::show::error(bn::core::system_font(), condition, file_name, function, line, message,
                                    bn::core::assert_tag());

                #ifdef BN_STACKTRACE
                    #if BN_CFG_LOG_ENABLED
                        bn::hw::core::wait_for_vblank();
                        bn::hw::stacktrace::log(5);
                    #endif
                #endif

                while(true)
                {
                    bn::hw::core::wait_for_vblank();
                }
            }
        }

        void show(const char* file_name, int line)
        {
            _show_impl("", file_name, "", line, "");
        }

        void show(const char* condition, const char* file_name, const char* function, int line)
        {
            _show_impl(condition, file_name, function, line, "");
        }

        void show(const char* condition, const char* file_name, const char* function, int line, const char* message)
        {
            _show_impl(condition, file_name, function, line, message);
        }

        void show(const char* condition, const char* file_name, const char* function, int line,
                  const bn::istring_base& message)
        {
            _show_impl(condition, file_name, function, line, message);
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
