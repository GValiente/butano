/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_hdma_manager.h"

#include "bn_memory.h"
#include "bn_display.h"
#include "../hw/include/bn_hw_hdma.h"

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
            _channel(channel)
        {
        }

        [[nodiscard]] bool running() const
        {
            return _next_state().elements;
        }

        void disable()
        {
            hw::hdma::stop(_channel);
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

        void commit()
        {
            const state& current_state = _current_state();

            if(int elements = current_state.elements)
            {
                const uint16_t* source_ptr = current_state.source_ptr;
                uint16_t* destination_ptr = current_state.destination_ptr;
                memory::copy(source_ptr[(display::height() - 1) * elements], elements, *destination_ptr);
                hw::hdma::start(_channel, source_ptr, elements, destination_ptr);
            }
            else
            {
                hw::hdma::stop(_channel);
            }
        }

    private:
        state _states[2];
        int8_t _channel = 0;
        int8_t _current_state_index = 0;
        bool _updated = false;

        [[nodiscard]] const state& _current_state() const
        {
            return _states[_current_state_index];
        }

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
        entry low_priority_entry = entry(hw::hdma::low_priority_channel());
        entry high_priority_entry = entry(hw::hdma::high_priority_channel());
    };

    BN_DATA_EWRAM static_data data;
}

void enable()
{
    commit();
}

void disable()
{
    data.low_priority_entry.disable();
    data.high_priority_entry.disable();
}

bool low_priority_running()
{
    return data.low_priority_entry.running();
}

void low_priority_start(const uint16_t& source_ref, int elements, uint16_t& destination_ref)
{
    data.low_priority_entry.start(source_ref, elements, destination_ref);
}

void low_priority_stop()
{
    data.low_priority_entry.stop();
}

bool high_priority_running()
{
    return data.high_priority_entry.running();
}

void high_priority_start(const uint16_t& source_ref, int elements, uint16_t& destination_ref)
{
    data.high_priority_entry.start(source_ref, elements, destination_ref);
}

void high_priority_stop()
{
    data.high_priority_entry.stop();
}

void update()
{
    data.low_priority_entry.update();
    data.high_priority_entry.update();
}

void commit()
{
    data.low_priority_entry.commit();
    data.high_priority_entry.commit();
}

}
