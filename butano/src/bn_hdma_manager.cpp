/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_hdma_manager.h"

#include <new>
#include "bn_display.h"
#include "../hw/include/bn_hw_dma.h"
#include "../hw/include/bn_hw_irq.h"
#include "../hw/include/bn_hw_audio.h"
#include "../hw/include/bn_hw_memory.h"

#include "bn_hdma.cpp.h"

namespace bn::hdma_manager
{

namespace
{
    class state
    {

    public:
        const uint16_t* source_ptr = nullptr;
        uint16_t* destination_ptr = nullptr;
        int elements = 0;
    };

    class entry
    {

    public:
        explicit entry(int channel) :
            _channel(int8_t(channel))
        {
        }

        [[nodiscard]] bool running() const
        {
            return _next_state().elements;
        }

        void disable()
        {
            hw::dma::stop_hdma(_channel);
        }

        void start(const uint16_t& source_ref, int elements, uint16_t& destination_ref)
        {
            state& next_state = _next_state();
            next_state.source_ptr = &source_ref;
            next_state.destination_ptr = &destination_ref;
            next_state.elements = elements;
            _updated = true;
        }

        void stop()
        {
            state& next_state = _next_state();
            next_state.elements = 0;
            _updated = true;
        }

        void force_stop()
        {
            _states[0].elements = 0;
            _states[1].elements = 0;
            _updated = false;
        }

        void update()
        {
            if(_updated)
            {
                _updated = false;

                if(_current_state_index)
                {
                    _current_state_index = 0;
                    _states[1] = _states[0];
                }
                else
                {
                    _current_state_index = 1;
                    _states[0] = _states[1];
                }
            }
        }

        bool commit(bool use_dma, bool raise_irq)
        {
            const state& current_state = _current_state();

            if(int elements = current_state.elements)
            {
                const uint16_t* source_ptr = current_state.source_ptr;
                const uint16_t* initial_copy_source_ptr = source_ptr + ((display::height() - 1) * elements);
                uint16_t* destination_ptr = current_state.destination_ptr;

                if(use_dma)
                {
                    hw::dma::copy_half_words(initial_copy_source_ptr, elements, destination_ptr);
                }
                else
                {
                    hw::memory::copy_half_words(initial_copy_source_ptr, elements, destination_ptr);
                }

                if(raise_irq)
                {
                    hw::dma::start_hdma_irq(_channel, source_ptr, elements, destination_ptr);
                }
                else
                {
                    hw::dma::start_hdma(_channel, source_ptr, elements, destination_ptr);
                }

                return true;
            }

            hw::dma::stop_hdma(_channel);
            return false;
        }

    private:
        state _states[2];
        int8_t _channel = 0;
        int8_t _current_state_index = 0;
        bool _updated = false;

        [[nodiscard]] state& _current_state()
        {
            return _states[_current_state_index];
        }

        [[nodiscard]] const state& _next_state() const
        {
            return _states[(_current_state_index + 1) % 2];
        }

        [[nodiscard]] state& _next_state()
        {
            return _states[(_current_state_index + 1) % 2];
        }
    };

    class static_data
    {

    public:
        entry low_priority_entry = entry(hw::dma::low_priority_channel());
        entry high_priority_entry = entry(hw::dma::high_priority_channel());
        hdma::interrupt_handler_type current_high_priority_interrupt_handler = nullptr;
        hdma::interrupt_handler_type new_high_priority_interrupt_handler = nullptr;
        bool high_priority_irq_enabled = false;
    };

    alignas(static_data) BN_DATA_EWRAM_BSS char data_buffer[sizeof(static_data)];

    [[nodiscard]] static_data& data_ref()
    {
        return *reinterpret_cast<static_data*>(data_buffer);
    }
}

void init()
{
    ::new(static_cast<void*>(data_buffer)) static_data();
}

void enable()
{
    commit_interrupt_handler();
    commit_entries(false);
}

void force_stop()
{
    static_data& data = data_ref();
    data.low_priority_entry.force_stop();
    data.high_priority_entry.force_stop();

    disable();

    data.current_high_priority_interrupt_handler = nullptr;
    data.new_high_priority_interrupt_handler = nullptr;
}

void disable()
{
    static_data& data = data_ref();
    data.low_priority_entry.disable();
    data.high_priority_entry.disable();

    if(data.high_priority_irq_enabled)
    {
        data.high_priority_irq_enabled = false;
        hw::irq::disable(hw::irq::id::DMA0);
    }
}

bool low_priority_running()
{
    return data_ref().low_priority_entry.running();
}

void low_priority_start(const uint16_t& source_ref, int elements, uint16_t& destination_ref)
{
    BN_BASIC_ASSERT(hw::audio::dma_channel_free(3), "Not supported by the audio backend");

    data_ref().low_priority_entry.start(source_ref, elements, destination_ref);
}

void low_priority_stop()
{
    data_ref().low_priority_entry.stop();
}

bool high_priority_running()
{
    return data_ref().high_priority_entry.running();
}

void high_priority_start(const uint16_t& source_ref, int elements, uint16_t& destination_ref)
{
    BN_BASIC_ASSERT(hw::audio::dma_channel_free(0), "Not supported by the audio backend");

    data_ref().high_priority_entry.start(source_ref, elements, destination_ref);
}

void high_priority_stop()
{
    data_ref().high_priority_entry.stop();
}

hdma::interrupt_handler_type high_priority_interrupt_handler()
{
    return data_ref().new_high_priority_interrupt_handler;
}

void set_high_priority_interrupt_handler(hdma::interrupt_handler_type interrupt_handler)
{
    data_ref().new_high_priority_interrupt_handler = interrupt_handler;
}

void update()
{
    static_data& data = data_ref();
    data.high_priority_entry.update();
    data.low_priority_entry.update();
}

void commit_interrupt_handler()
{
    static_data& data = data_ref();
    hdma::interrupt_handler_type new_high_priority_interrupt_handler = data.new_high_priority_interrupt_handler;

    if(data.current_high_priority_interrupt_handler != new_high_priority_interrupt_handler)
    {
        data.current_high_priority_interrupt_handler = new_high_priority_interrupt_handler;
        bn::hw::irq::set_isr(bn::hw::irq::id::DMA0, new_high_priority_interrupt_handler);
    }
}

bool commit_entries(bool use_dma)
{
    static_data& data = data_ref();
    hdma::interrupt_handler_type high_priority_interrupt_handler = data.current_high_priority_interrupt_handler;
    bool running = data.high_priority_entry.commit(use_dma, high_priority_interrupt_handler);

    if(running && high_priority_interrupt_handler)
    {
        if(! data.high_priority_irq_enabled)
        {
            data.high_priority_irq_enabled = true;
            hw::irq::enable(hw::irq::id::DMA0);
        }
    }
    else
    {
        if(data.high_priority_irq_enabled)
        {
            data.high_priority_irq_enabled = false;
            hw::irq::disable(hw::irq::id::DMA0);
        }
    }

    running |= data.low_priority_entry.commit(use_dma, false);
    return running;
}

}
