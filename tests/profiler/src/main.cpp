/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include <coroutine>
#include "bn_core.h"
#include "bn_math.h"
#include "bn_random.h"
#include "bn_profiler.h"
#include "bn_unique_ptr.h"
#include "bn_seed_random.h"

#include "../../butano/hw/include/bn_hw_dma.h"
#include "../../butano/hw/include/bn_hw_memory.h"
#include "../../butano/hw/include/bn_hw_decompress.h"

#include "bn_regular_bg_items_butano_huge_rl.h"
#include "bn_regular_bg_items_butano_huge_huff.h"
#include "bn_regular_bg_items_butano_huge_lz77.h"

namespace
{

constexpr int its_sqrt = 100;
constexpr int its = its_sqrt * its_sqrt;

void div_test(int& integer)
{
    int div_result = 0;
    BN_PROFILER_START("div_regular");

    for(int i = 0; i < its; ++i)
    {
        div_result += integer / (i + 1);
    }

    BN_PROFILER_STOP();

    int bios_div_result = 0;
    BN_PROFILER_START("div_bios");

    for(int i = 0; i < its; ++i)
    {
        bios_div_result += Div(integer, i + 1);
    }

    BN_PROFILER_STOP();

    BN_ASSERT(div_result == bios_div_result, "Invalid division");
    integer += div_result;
    integer += bios_div_result;
}

void sqrt_test(int& integer)
{
    int sqrt_result = 0;
    BN_PROFILER_START("sqrt_regular");

    for(int i = 0; i < its; ++i)
    {
        sqrt_result += bn::sqrt(bn::abs(sqrt_result));
    }

    BN_PROFILER_STOP();

    int bios_sqrt_result = 0;
    BN_PROFILER_START("sqrt_bios");

    for(int i = 0; i < its; ++i)
    {
        bios_sqrt_result += Sqrt(unsigned(bn::abs(bios_sqrt_result)));
    }

    BN_PROFILER_STOP();

    BN_ASSERT(sqrt_result == bios_sqrt_result, "Invalid sqrt");
    integer += sqrt_result;
    integer += bios_sqrt_result;
}

void random_test(int& integer)
{
    bn::random random;
    bn::seed_random seed_random;

    BN_PROFILER_START("random_regular");

    for(int i = 0; i < its; ++i)
    {
        integer += random.get();
    }

    BN_PROFILER_STOP();

    BN_PROFILER_START("random_seed");

    for(int i = 0; i < its; ++i)
    {
        integer += seed_random.get();
    }

    BN_PROFILER_STOP();
}

void lut_sin_test(int& integer)
{
    BN_PROFILER_START("lut_sin_regular");

    for(int i = 0; i < its; ++i)
    {
        integer += bn::lut_sin(i % 2048).data();
    }

    BN_PROFILER_STOP();

    BN_PROFILER_START("lut_sin_calculate");

    for(int i = 0; i < its; ++i)
    {
        integer += bn::calculate_sin_lut_value(i % 65536);
    }

    BN_PROFILER_STOP();
}

void atan2_test(int& integer)
{
    int its_sqrt_half = its_sqrt / 2;

    BN_PROFILER_START("atan2_regular");

    for(int y = -its_sqrt_half; y < its_sqrt_half; ++y)
    {
        for(int x = -its_sqrt_half; x < its_sqrt_half; ++x)
        {
            integer += bn::atan2(y, x).data();
        }
    }

    BN_PROFILER_STOP();

    BN_PROFILER_START("atan2_bios");

    for(int y = -its_sqrt_half; y < its_sqrt_half; ++y)
    {
        for(int x = -its_sqrt_half; x < its_sqrt_half; ++x)
        {
            integer += int(ArcTan2(int16_t(x), int16_t(y)));
        }
    }

    BN_PROFILER_STOP();

    BN_PROFILER_START("atan2_diamond_angle");

    for(int y = -its_sqrt_half; y < its_sqrt_half; ++y)
    {
        for(int x = -its_sqrt_half; x < its_sqrt_half; ++x)
        {
            integer += bn::diamond_angle(y, x).data();
        }
    }

    BN_PROFILER_STOP();
}


class coroutine_task
{

public:
    struct promise_type;
    using co_handle_t = std::coroutine_handle<promise_type>;

    struct promise_type
    {
        [[nodiscard]] coroutine_task get_return_object()
        {
            return coroutine_task(co_handle_t::from_promise(*this));
        }

        [[nodiscard]] auto initial_suspend()
        {
            return std::suspend_never();
        }

        [[nodiscard]] auto final_suspend() noexcept
        {
            return std::suspend_always();
        }

        void unhandled_exception()
        {
        }

        void return_void()
        {
        }
    };

    coroutine_task(co_handle_t co_handle) :
        _co_handle(co_handle)
    {
    }

    ~coroutine_task()
    {
        if(_co_handle)
        {
            _co_handle.destroy();
        }
    }

    coroutine_task(const coroutine_task&) = delete;
    coroutine_task& operator=(const coroutine_task&) = delete;

    coroutine_task(coroutine_task&& other) noexcept :
        _co_handle(bn::move(other._co_handle))
    {
        other._co_handle = co_handle_t();
    }

    coroutine_task& operator=(coroutine_task&& other) noexcept
    {
        if(this != &other)
        {
            _co_handle = std::move(other._co_handle);
            other._co_handle = co_handle_t();
        }

        return *this;
    }

    [[nodiscard]] bool done() const
    {
        return _co_handle.done();
    }

    void operator()()
    {
        if(_co_handle && ! _co_handle.done())
        {
            _co_handle.resume();
        }
    }

private:
    co_handle_t _co_handle;
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-default"

coroutine_task coroutine_impl(int& integer)
{
    bn::random random;
    unsigned last_result = 0;

    for(int i = 0; i < its; ++i)
    {
        unsigned new_result = random.get();
        unsigned result = last_result + new_result;
        last_result = new_result;
        integer += int(result);
        co_await std::suspend_always{};
    }
}

#pragma GCC diagnostic pop

void coroutine_test(int& integer)
{
    BN_PROFILER_START("coroutine_disabled");

    int disabled_result = 0;

    {
        bn::random random;
        unsigned last_result = 0;

        for(int i = 0; i < its; ++i)
        {
            unsigned new_result = random.get();
            unsigned result = last_result + new_result;
            last_result = new_result;
            disabled_result += int(result);
        }
    }

    BN_PROFILER_STOP();

    BN_PROFILER_START("coroutine_enabled");

    int enabled_result = 0;
    coroutine_task task(coroutine_impl(enabled_result));

    while(! task.done())
    {
        task();
    }

    BN_PROFILER_STOP();

    BN_ASSERT(disabled_result == enabled_result, "Invalid coroutine");

    integer += disabled_result;
    integer += enabled_result;
}


constexpr int copy_words = bn::regular_bg_items::butano_huge_huff.tiles_item().tiles_ref().size_bytes() / 4;
constexpr int copy_words_data[copy_words] = {};

void copy_words_test()
{
    bn::unique_ptr<bn::array<int, copy_words>> buffer_ptr(new bn::array<int, copy_words>());
    int* buffer = buffer_ptr->data();

    BN_PROFILER_START("copy_words_regular");

    bn::hw::memory::copy_words(copy_words_data, copy_words, buffer);

    BN_PROFILER_STOP();

    BN_PROFILER_START("copy_words_dma");

    bn::hw::dma::copy_words(copy_words_data, copy_words, buffer);

    BN_PROFILER_STOP();

    BN_PROFILER_START("copy_words_fiq");

    bn::hw::memory::copy_words_fiq(copy_words_data, copy_words, buffer);

    BN_PROFILER_STOP();
}

void rl_decomp_test()
{
    const bn::tile* tiles = bn::regular_bg_items::butano_huge_rl.tiles_item().tiles_ref().data();
    bn::unique_ptr<bn::array<uint8_t, 64 * 1024>> buffer_ptr(new bn::array<uint8_t, 64 * 1024>());
    uint8_t* buffer = buffer_ptr->data();

    BN_PROFILER_START("rl_wram_regular");

    bn::hw::decompress::rl_wram(tiles, buffer);

    BN_PROFILER_STOP();

    BN_PROFILER_START("rl_wram_bios");

    RLUnCompWram(tiles, buffer);

    BN_PROFILER_STOP();

    BN_PROFILER_START("rl_vram_regular");

    bn::hw::decompress::rl_vram(tiles, buffer);

    BN_PROFILER_STOP();

    BN_PROFILER_START("rl_vram_bios");

    RLUnCompVram(tiles, buffer);

    BN_PROFILER_STOP();
}

void lz77_decomp_test()
{
    const bn::tile* tiles = bn::regular_bg_items::butano_huge_lz77.tiles_item().tiles_ref().data();
    bn::unique_ptr<bn::array<uint8_t, 64 * 1024>> buffer_ptr(new bn::array<uint8_t, 64 * 1024>());
    uint8_t* buffer = buffer_ptr->data();

    BN_PROFILER_START("lz77_regular");

    bn::hw::decompress::lz77(tiles, buffer);

    BN_PROFILER_STOP();

    BN_PROFILER_START("lz77_wram_bios");

    LZ77UnCompWram(tiles, buffer);

    BN_PROFILER_STOP();

    BN_PROFILER_START("lz77_vram_bios");

    LZ77UnCompVram(tiles, buffer);

    BN_PROFILER_STOP();
}

void huff_decomp_test()
{
    const bn::tile* tiles = bn::regular_bg_items::butano_huge_huff.tiles_item().tiles_ref().data();
    bn::unique_ptr<bn::array<uint8_t, 64 * 1024>> buffer_ptr(new bn::array<uint8_t, 64 * 1024>());
    uint8_t* buffer = buffer_ptr->data();

    BN_PROFILER_START("huff_regular");

    bn::hw::decompress::huff(tiles, buffer);

    BN_PROFILER_STOP();

    BN_PROFILER_START("huff_bios");

    HuffUnComp(tiles, buffer);

    BN_PROFILER_STOP();
}

}

int main()
{
    bn::core::init();

    int integer = 123456789;
    div_test(integer);
    sqrt_test(integer);
    random_test(integer);
    lut_sin_test(integer);
    atan2_test(integer);
    coroutine_test(integer);
    copy_words_test();
    rl_decomp_test();
    lz77_decomp_test();
    huff_decomp_test();

    if(integer)
    {
        bn::profiler::show();
    }
    else
    {
        while(true)
        {
        }
    }
}
