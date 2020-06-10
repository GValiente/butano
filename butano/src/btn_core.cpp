#include "btn_core.h"

#include "btn_span.h"
#include "btn_fixed.h"
#include "btn_timer.h"
#include "btn_string.h"
#include "btn_keypad.h"
#include "btn_optional.h"
#include "btn_profiler.h"
#include "btn_string_view.h"
#include "btn_bgs_manager.h"
#include "btn_audio_manager.h"
#include "btn_keypad_manager.h"
#include "btn_memory_manager.h"
#include "btn_display_manager.h"
#include "btn_sprites_manager.h"
#include "btn_palettes_manager.h"
#include "btn_bg_blocks_manager.h"
#include "btn_input_string_stream.h"
#include "btn_sprite_tiles_manager.h"
#include "btn_hblank_effects_manager.h"
#include "../hw/include/btn_hw_irq.h"
#include "../hw/include/btn_hw_core.h"
#include "../hw/include/btn_hw_sram.h"
#include "../hw/include/btn_hw_timer.h"
#include "../hw/include/btn_hw_game_pak.h"

#if BTN_CFG_ASSERT_ENABLED
    #include "btn_string_view.h"
#endif

#if BTN_CFG_ASSERT_ENABLED || BTN_CFG_PROFILER_ENABLED
    #include "../hw/include/btn_hw_show.h"
#endif

#if BTN_CFG_PROFILER_ENABLED && BTN_CFG_PROFILER_LOG_ENGINE
    #define BTN_PROFILER_ENGINE_START(id) \
        BTN_PROFILER_START(id)

    #define BTN_PROFILER_ENGINE_STOP() \
        BTN_PROFILER_STOP()
#else
    #define BTN_PROFILER_ENGINE_START(id) \
        do \
        { \
        } while(false)

    #define BTN_PROFILER_ENGINE_STOP() \
        do \
        { \
        } while(false)
#endif

namespace btn::core
{

namespace
{
    class static_data
    {

    public:
        optional<timer> cpu_usage_timer;
        fixed cpu_usage;
    };

    BTN_DATA_EWRAM static_data data;

    void enable()
    {
        hblank_effects_manager::enable();
        audio_manager::enable();
    }

    void disable()
    {
        audio_manager::disable();
        hblank_effects_manager::disable();
    }

    void stop()
    {
        audio_manager::stop();
        audio_manager::disable_vblank_handler();
        hw::core::wait_for_vblank();
        audio_manager::commit();
        audio_manager::enable_vblank_handler();

        palettes_manager::stop();
        display_manager::stop();
        keypad_manager::stop();
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
    sprite_tiles_manager::init();
    sprites_manager::init();
    bg_blocks_manager::init();
    keypad_manager::init(keypad_commands);

    // WTF hack (if it isn't present and flto is enabled, sometimes everything crash):
    string<32> hack_string;
    input_string_stream hack_input_string_stream(hack_string);
    hack_input_string_stream.append(2);

    // Init timer system:
    hw::timer::init();
    data.cpu_usage_timer = timer();

    // First update:
    update();

    // Keypad polling fix:
    keypad_manager::update();

    // Reset profiler:
    BTN_PROFILER_RESET();
}

void update()
{
    BTN_PROFILER_ENGINE_START("eng_sprites_update");
    sprites_manager::update();
    BTN_PROFILER_ENGINE_STOP();

    BTN_PROFILER_ENGINE_START("eng_sprite_tiles_update");
    sprite_tiles_manager::update();
    BTN_PROFILER_ENGINE_STOP();

    BTN_PROFILER_ENGINE_START("eng_bgs_update");
    bgs_manager::update();
    BTN_PROFILER_ENGINE_STOP();

    BTN_PROFILER_ENGINE_START("eng_bg_blocks_update");
    bg_blocks_manager::update();
    BTN_PROFILER_ENGINE_STOP();

    BTN_PROFILER_ENGINE_START("eng_palettes_update");
    palettes_manager::update();
    BTN_PROFILER_ENGINE_STOP();

    BTN_PROFILER_ENGINE_START("eng_display_update");
    display_manager::update();
    BTN_PROFILER_ENGINE_STOP();

    BTN_PROFILER_ENGINE_START("eng_hblank_effects_update");
    hblank_effects_manager::update();
    BTN_PROFILER_ENGINE_STOP();


    BTN_PROFILER_ENGINE_START("eng_cpu_usage");
    data.cpu_usage = data.cpu_usage_timer->elapsed_frames();
    BTN_PROFILER_ENGINE_STOP();

    audio_manager::disable_vblank_handler();
    hw::core::wait_for_vblank();

    BTN_PROFILER_ENGINE_START("eng_cpu_usage");
    data.cpu_usage_timer->restart();
    BTN_PROFILER_ENGINE_STOP();

    BTN_PROFILER_ENGINE_START("eng_hblank_effects_commit");
    hblank_effects_manager::commit();
    BTN_PROFILER_ENGINE_STOP();

    BTN_PROFILER_ENGINE_START("eng_display_commit");
    display_manager::commit();
    BTN_PROFILER_ENGINE_STOP();

    BTN_PROFILER_ENGINE_START("eng_sprites_commit");
    sprites_manager::commit();
    BTN_PROFILER_ENGINE_STOP();

    BTN_PROFILER_ENGINE_START("eng_bgs_commit");
    bgs_manager::commit();
    BTN_PROFILER_ENGINE_STOP();

    BTN_PROFILER_ENGINE_START("eng_palettes_commit");
    palettes_manager::commit();
    BTN_PROFILER_ENGINE_STOP();

    BTN_PROFILER_ENGINE_START("eng_sprite_tiles_commit");
    sprite_tiles_manager::commit();
    BTN_PROFILER_ENGINE_STOP();

    BTN_PROFILER_ENGINE_START("eng_bg_blocks_commit");
    bg_blocks_manager::commit();
    BTN_PROFILER_ENGINE_STOP();

    BTN_PROFILER_ENGINE_START("eng_audio_commit");
    audio_manager::commit();
    BTN_PROFILER_ENGINE_STOP();

    audio_manager::enable_vblank_handler();

    BTN_PROFILER_ENGINE_START("eng_keypad");
    keypad_manager::update();
    BTN_PROFILER_ENGINE_STOP();
}

void update(int frames)
{
    BTN_ASSERT(frames > 0, "Invalid frames: ", frames);

    while(frames--)
    {
        update();
    }
}

void sleep(keypad::key_type wake_up_key)
{
    const keypad::key_type wake_up_keys[] = { wake_up_key };
    sleep(wake_up_keys);
}

void sleep(const span<const keypad::key_type>& wake_up_keys)
{
    BTN_ASSERT(! wake_up_keys.empty(), "There's no wake up keys");

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
    disable();

    // Enable keypad interrupt with the specified wake up keys:
    keypad_manager::set_interrupt(wake_up_keys);
    hw::irq::add(hw::irq::id::KEYPAD, nullptr);

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
    BTN_PROFILER_ENGINE_START("eng_cpu_usage");
    data.cpu_usage_timer->restart();
    BTN_PROFILER_ENGINE_STOP();

    // Wake up display:
    display_manager::wake_up();
}

void reset()
{
    stop();
    disable();
    hw::core::reset();
}

fixed cpu_usage()
{
    return data.cpu_usage;
}

}

#if BTN_CFG_ASSERT_ENABLED
    namespace _btn::assert
    {
        void show(const char* condition, const char* file_name, const char* function, int line, const char* message)
        {
            btn::core::stop();
            btn::hw::show::error(condition, file_name, function, line, message);

            while(true)
            {
                btn::hw::core::wait_for_vblank();
            }
        }

        void show(const char* condition, const char* file_name, const char* function, int line,
                  const btn::istring_base& message)
        {
            btn::core::stop();
            btn::hw::show::error(condition, file_name, function, line, message);

            while(true)
            {
                btn::hw::core::wait_for_vblank();
            }
        }
    }
#endif

#if BTN_CFG_PROFILER_ENABLED
    namespace btn::profiler
    {
        void show()
        {
            core::stop();
            hw::show::profiler_results();
        }
    }
#endif
