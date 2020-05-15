#ifndef BTN_CORE_H
#define BTN_CORE_H

#include "btn_span_fwd.h"
#include "btn_fixed_fwd.h"

namespace btn
{

namespace keypad
{
    enum class key_type;
}

namespace core
{
    void init();

    void update();

    void update(int frames);

    void sleep(keypad::key_type wake_up_key);

    void sleep(const span<const keypad::key_type>& wake_up_keys);

    [[noreturn]] void reset();

    [[nodiscard]] fixed cpu_usage();
}
}

#endif
