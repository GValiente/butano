#include "btn_core.h"

#include "btn_config_assert.h"

#if BTN_CFG_ASSERT_ENABLED
    #include "../3rd_party/etl/include/etl/error_handler.h"
#endif

#include "btn_span.h"
#include "btn_fixed.h"
#include "btn_timer.h"
#include "btn_keypad.h"
#include "btn_optional.h"
#include "btn_profiler.h"
#include "btn_bgs_manager.h"
#include "btn_audio_manager.h"
#include "btn_display_manager.h"
#include "btn_actions_manager.h"
#include "btn_sprites_manager.h"
#include "btn_palettes_manager.h"
#include "btn_bg_blocks_manager.h"
#include "btn_input_string_stream.h"
#include "btn_sprite_tiles_manager.h"
#include "../hw/include/btn_hw_irq.h"
#include "../hw/include/btn_hw_core.h"
#include "../hw/include/btn_hw_sram.h"
#include "../hw/include/btn_hw_timer.h"
#include "../hw/include/btn_hw_keypad.h"
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
    static_assert(BTN_CFG_ASSERT_BUFFER_SIZE >= 64);

    class static_data
    {

    public:
        optional<etl::error_handler::free_function> etl_error_callback;
        optional<timer> cpu_usage_timer;
        fixed cpu_usage;
    };

    BTN_DATA_EWRAM static_data data;

    void add_irqs()
    {
        audio_manager::add_irq();
    }

    void remove_irqs()
    {
        audio_manager::remove_irq();
    }

    void stop()
    {
        actions_manager::stop();
        hw::core::wait_for_vblank();
        audio_manager::stop();
    }

    #if BTN_CFG_ASSERT_ENABLED
        [[noreturn]] void etl_error_callback_function(const etl::exception& exception)
        {
            string<BTN_CFG_ASSERT_BUFFER_SIZE> message;
            message.append(exception.what());
            _btn::assert::show("ETL error", exception.file_name(), "", exception.line_number(), message);
        }
    #endif

    void _update_without_actions()
    {
        BTN_PROFILER_ENGINE_START("eng_palettes_update");
        palettes_manager::update();
        BTN_PROFILER_ENGINE_STOP();

        BTN_PROFILER_ENGINE_START("eng_sprite_tiles_update");
        sprite_tiles_manager::update();
        BTN_PROFILER_ENGINE_STOP();

        BTN_PROFILER_ENGINE_START("eng_sprites_update");
        sprites_manager::update();
        BTN_PROFILER_ENGINE_STOP();

        BTN_PROFILER_ENGINE_START("eng_bg_blocks_update");
        bg_blocks_manager::update();
        BTN_PROFILER_ENGINE_STOP();

        BTN_PROFILER_ENGINE_START("eng_audio_update");
        audio_manager::update();
        BTN_PROFILER_ENGINE_STOP();

        BTN_PROFILER_ENGINE_START("eng_cpu_usage");
        data.cpu_usage = data.cpu_usage_timer->elapsed_frames();
        BTN_PROFILER_ENGINE_STOP();

        hw::core::wait_for_vblank();

        BTN_PROFILER_ENGINE_START("eng_cpu_usage");
        data.cpu_usage_timer->restart();
        BTN_PROFILER_ENGINE_STOP();

        BTN_PROFILER_ENGINE_START("eng_display_commit");
        display_manager::commit();
        BTN_PROFILER_ENGINE_STOP();

        BTN_PROFILER_ENGINE_START("eng_palettes_commit");
        palettes_manager::commit();
        BTN_PROFILER_ENGINE_STOP();

        BTN_PROFILER_ENGINE_START("eng_sprite_tiles_commit");
        sprite_tiles_manager::commit();
        BTN_PROFILER_ENGINE_STOP();

        BTN_PROFILER_ENGINE_START("eng_sprites_commit");
        sprites_manager::commit();
        BTN_PROFILER_ENGINE_STOP();

        BTN_PROFILER_ENGINE_START("eng_bg_blocks_commit");
        bg_blocks_manager::commit();
        BTN_PROFILER_ENGINE_STOP();

        BTN_PROFILER_ENGINE_START("eng_bgs_commit");
        bgs_manager::commit();
        BTN_PROFILER_ENGINE_STOP();

        BTN_PROFILER_ENGINE_START("eng_audio_commit");
        audio_manager::commit();
        BTN_PROFILER_ENGINE_STOP();

        BTN_PROFILER_ENGINE_START("eng_keypad");
        hw::keypad::update();
        BTN_PROFILER_ENGINE_STOP();
    }
}

void init()
{
    #if BTN_CFG_ASSERT_ENABLED
        // Init ETL's error handler:
        data.etl_error_callback = etl::error_handler::free_function(etl_error_callback_function);
        etl::error_handler::set_callback(*data.etl_error_callback);
    #endif

    // Init storage systems:
    hw::game_pak::init();
    hw::sram::init();

    // Init display:
    display_manager::init();

    // Init irq system:
    hw::irq::init();
    add_irqs();

    // Init audio system:
    audio_manager::init();

    // Init high level systems:
    sprite_tiles_manager::init();
    sprites_manager::init();
    bg_blocks_manager::init();

    // Init timer system:
    hw::timer::init();
    data.cpu_usage_timer = timer();

    // First update:
    update();

    // Reset profiler:
    BTN_PROFILER_RESET();
}

void update()
{
    BTN_PROFILER_ENGINE_START("eng_actions_update");
    actions_manager::update();
    BTN_PROFILER_ENGINE_STOP();

    _update_without_actions();
}

void update(int frames)
{
    BTN_ASSERT(frames > 0, "Invalid frames: ", frames);

    while(frames--)
    {
        update();
    }
}

void sleep(keypad::button_type wake_up_button)
{
    const keypad::button_type wake_up_buttons[] = { wake_up_button };
    sleep(wake_up_buttons);
}

void sleep(const span<const keypad::button_type>& wake_up_buttons)
{
    BTN_ASSERT(! wake_up_buttons.empty(), "There's no wake up buttons");

    // Wait until a wake up button is not pressed:
    while(true)
    {
        bool wait = true;

        for(keypad::button_type wake_up_button : wake_up_buttons)
        {
            if(keypad::up(wake_up_button))
            {
                wait = false;
                break;
            }
        }

        if(wait)
        {
            _update_without_actions();
        }
        else
        {
            break;
        }
    }

    // Sleep audio:
    audio_manager::sleep();

    // Sleep display:
    display_manager::sleep();

    // Remove core irqs:
    remove_irqs();

    // Enable keypad interrupt with the specified wake up buttons:
    hw::keypad::set_interrupt(wake_up_buttons);
    hw::irq::add(hw::irq::id::KEYPAD, nullptr);

    // Enable sleep mode:
    hw::core::sleep();

    // Remove keypad interrupt:
    hw::irq::remove(hw::irq::id::KEYPAD);

    // Restore core irqs:
    add_irqs();

    // Update keypad:
    hw::keypad::update();

    // Wait for vblank:
    hw::core::wait_for_vblank();

    // Restart CPU usage timer:
    BTN_PROFILER_ENGINE_START("eng_cpu_usage");
    data.cpu_usage_timer->restart();
    BTN_PROFILER_ENGINE_STOP();

    // Wake up audio:
    audio_manager::wake_up();

    // Wake up display:
    display_manager::wake_up();
}

void reset()
{
    stop();
    remove_irqs();
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
        void show(const char* condition, const char* file_name, const char* function, int line,
                  const btn::istring& message)
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
