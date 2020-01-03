#ifndef BTN_CORE_H
#define BTN_CORE_H

#include "btn_span_fwd.h"
#include "btn_fixed_fwd.h"

namespace btn
{
namespace keypad
{
    enum class button_type;
}

namespace core
{
    void init();

    void update();

    void update(int frames);

    void sleep(keypad::button_type wake_up_button);

    void sleep(const span<const keypad::button_type>& wake_up_buttons);

    [[noreturn]] void reset();

    [[nodiscard]] fixed cpu_usage();
}
}

#endif
