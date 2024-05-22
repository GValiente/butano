/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include <coroutine>
#include "bn_core.h"
#include "bn_math.h"
#include "bn_random.h"
#include "bn_profiler.h"
#include "bn_unique_ptr.h"
#include "bn_seed_random.h"
#include "bn_best_fit_allocator.h"

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
constexpr bool check_bios = true;

void div_test(int& integer)
{
    constexpr int dividend = bn::numeric_limits<int>::max() / 2;

    int div_result = 0;
    BN_PROFILER_START("div_regular");

    for(int i = 0; i < its; ++i)
    {
        div_result += dividend / (i + 1);
    }

    BN_PROFILER_STOP();

    integer += div_result;

    if(check_bios)
    {
        int bios_div_result = 0;
        BN_PROFILER_START("div_bios");

        for(int i = 0; i < its; ++i)
        {
            bios_div_result += Div(dividend, i + 1);
        }

        BN_PROFILER_STOP();

        BN_ASSERT(div_result == bios_div_result, "Invalid division");
        integer += bios_div_result;
    }
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

    integer += sqrt_result;

    if(check_bios)
    {
        int bios_sqrt_result = 0;
        BN_PROFILER_START("sqrt_bios");

        for(int i = 0; i < its; ++i)
        {
            bios_sqrt_result += Sqrt(unsigned(bn::abs(bios_sqrt_result)));
        }

        BN_PROFILER_STOP();

        BN_ASSERT(sqrt_result == bios_sqrt_result, "Invalid sqrt");
        integer += bios_sqrt_result;
    }
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

    if(check_bios)
    {
        BN_PROFILER_START("atan2_bios");

        for(int y = -its_sqrt_half; y < its_sqrt_half; ++y)
        {
            for(int x = -its_sqrt_half; x < its_sqrt_half; ++x)
            {
                integer += int(ArcTan2(int16_t(x), int16_t(y)));
            }
        }

        BN_PROFILER_STOP();
    }

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

template<class allocator>
class std_coroutine_task
{

public:
    struct promise_type;
    using co_handle_t = std::coroutine_handle<promise_type>;

    struct promise_type
    {
        [[nodiscard]] std_coroutine_task get_return_object()
        {
            return std_coroutine_task(co_handle_t::from_promise(*this));
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

        void* operator new(unsigned bytes)
        {
            return allocator::alloc(bytes);
        }

        void operator delete(void* ptr)
        {
            allocator::free(ptr);
        }
    };

    std_coroutine_task(co_handle_t co_handle) :
        _co_handle(co_handle)
    {
    }

    ~std_coroutine_task()
    {
        if(_co_handle)
        {
            _co_handle.destroy();
        }
    }

    std_coroutine_task(const std_coroutine_task&) = delete;
    std_coroutine_task& operator=(const std_coroutine_task&) = delete;

    std_coroutine_task(std_coroutine_task&& other) noexcept :
        _co_handle(bn::move(other._co_handle))
    {
        other._co_handle = co_handle_t();
    }

    std_coroutine_task& operator=(std_coroutine_task&& other) noexcept
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

class ewram_allocator
{

public:
    [[nodiscard]] static void* alloc(unsigned bytes)
    {
        return ::malloc(bytes);
    }

    static void free(void* ptr)
    {
        return ::free(ptr);
    }
};

class iwram_allocator
{

public:
    static inline bn::best_fit_allocator* allocator = nullptr;

    [[nodiscard]] static void* alloc(unsigned bytes)
    {
        return allocator->alloc(int(bytes));
    }

    static void free(void* ptr)
    {
        allocator->free(ptr);
    }
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-default"

template<class allocator>
std_coroutine_task<allocator> std_coroutine_impl(int& integer)
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

int agbabi_coroutine_impl(__agbabi_coro_t* coro)
{
    bn::random random;
    unsigned last_result = 0;

    for(int i = 0; i < its; ++i)
    {
        unsigned new_result = random.get();
        unsigned result = last_result + new_result;
        last_result = new_result;
        __agbabi_coro_yield(coro, int(result));
    }

    return 0;
}

void coroutine_test(int& integer)
{
    constexpr int stack_length = 0x200;

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

    BN_PROFILER_START("coroutine_std_ewram");

    int std_ewram_result = 0;

    {
        std_coroutine_task<ewram_allocator> task(std_coroutine_impl<ewram_allocator>(std_ewram_result));

        while(! task.done())
        {
            task();
        }
    }

    BN_PROFILER_STOP();

    BN_ASSERT(disabled_result == std_ewram_result, "Invalid std ewram coroutine");

    BN_PROFILER_START("coroutine_std_iwram");

    int std_iwram_result = 0;

    {
        alignas(int) char buffer[stack_length];
        bn::best_fit_allocator allocator(buffer, stack_length);
        iwram_allocator::allocator = &allocator;

        std_coroutine_task<iwram_allocator> task(std_coroutine_impl<iwram_allocator>(std_iwram_result));

        while(! task.done())
        {
            task();
        }
    }

    BN_PROFILER_STOP();

    BN_ASSERT(disabled_result == std_iwram_result, "Invalid std iwram coroutine");

    BN_PROFILER_START("coroutine_agbabi_ewram");

    int agbabi_ewram_result = 0;

    {
        bn::unique_ptr<bn::array<int, stack_length>> stack_ptr(new bn::array<int, stack_length>());
        int* stack = stack_ptr->data();

        __agbabi_coro_t coro;
        __agbabi_coro_make(&coro, stack + stack_length, agbabi_coroutine_impl);

        while(! coro.joined)
        {
            agbabi_ewram_result += __agbabi_coro_resume(&coro);
        }
    }

    BN_PROFILER_STOP();

    BN_ASSERT(disabled_result == agbabi_ewram_result, "Invalid agbabi ewram coroutine");

    BN_PROFILER_START("coroutine_agbabi_iwram");

    int agbabi_iwram_result = 0;

    {
        int stack[stack_length];

        __agbabi_coro_t coro;
        __agbabi_coro_make(&coro, stack + stack_length, agbabi_coroutine_impl);

        while(! coro.joined)
        {
            agbabi_iwram_result += __agbabi_coro_resume(&coro);
        }
    }

    BN_PROFILER_STOP();

    BN_ASSERT(disabled_result == agbabi_iwram_result, "Invalid agbabi iwram coroutine");

    integer += disabled_result;
    integer += std_ewram_result;
    integer += std_iwram_result;
    integer += agbabi_ewram_result;
    integer += agbabi_iwram_result;
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

    if(check_bios)
    {
        BN_PROFILER_START("rl_wram_bios");

        RLUnCompWram(tiles, buffer);

        BN_PROFILER_STOP();
    }

    BN_PROFILER_START("rl_vram_regular");

    bn::hw::decompress::rl_vram(tiles, buffer);

    BN_PROFILER_STOP();

    if(check_bios)
    {
        BN_PROFILER_START("rl_vram_bios");

        RLUnCompVram(tiles, buffer);

        BN_PROFILER_STOP();
    }
}

void lz77_decomp_test()
{
    const bn::tile* tiles = bn::regular_bg_items::butano_huge_lz77.tiles_item().tiles_ref().data();
    bn::unique_ptr<bn::array<uint8_t, 64 * 1024>> buffer_ptr(new bn::array<uint8_t, 64 * 1024>());
    uint8_t* buffer = buffer_ptr->data();

    BN_PROFILER_START("lz77_regular");

    bn::hw::decompress::lz77(tiles, buffer);

    BN_PROFILER_STOP();

    if(check_bios)
    {
        BN_PROFILER_START("lz77_wram_bios");

        LZ77UnCompWram(tiles, buffer);

        BN_PROFILER_STOP();

        BN_PROFILER_START("lz77_vram_bios");

        LZ77UnCompVram(tiles, buffer);

        BN_PROFILER_STOP();
    }
}

void huff_decomp_test()
{
    const bn::tile* tiles = bn::regular_bg_items::butano_huge_huff.tiles_item().tiles_ref().data();
    bn::unique_ptr<bn::array<uint8_t, 64 * 1024>> buffer_ptr(new bn::array<uint8_t, 64 * 1024>());
    uint8_t* buffer = buffer_ptr->data();

    BN_PROFILER_START("huff_regular");

    bn::hw::decompress::huff(tiles, buffer);

    BN_PROFILER_STOP();

    if(check_bios)
    {
        BN_PROFILER_START("huff_bios");

        HuffUnComp(tiles, buffer);

        BN_PROFILER_STOP();
    }
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
