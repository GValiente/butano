/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_link.h"

#include "../include/bn_hw_irq.h"

namespace bn::hw::link
{

namespace
{
    class static_data
    {

    public:
        lc::LinkConnection connection;
        deque<uint16_t, LINK_DEFAULT_BUFFER_SIZE> sendMessages;
        deque<lc::LinkResponse, LINK_DEFAULT_BUFFER_SIZE> firstReceivedMessages;
        deque<lc::LinkResponse, LINK_DEFAULT_BUFFER_SIZE> secondReceivedMessages;
        volatile bool blockSendMessages = false;
        volatile bool blockReceivedMessages = false;
        volatile bool clearSendMessages = false;
        volatile bool clearReceivedMessages = false;
        bool active = false;
    };

    BN_DATA_EWRAM_BSS static_data data;

    void _check_active()
    {
        if(! data.active)
        {
            enable();
            data.active = true;
        }
    }

    void _sendDataCallback()
    {
        if(! data.blockSendMessages)
        {
            for(uint16_t message : data.sendMessages)
            {
                data.connection.send(message);
            }

            data.sendMessages.clear();
        }
    }

    void _receiveResponseCallback(const lc::LinkResponse& response)
    {
        if(data.blockReceivedMessages)
        {
            if(data.secondReceivedMessages.full())
            {
                data.secondReceivedMessages.pop_front();
            }

            data.secondReceivedMessages.push_back(response);
        }
        else
        {
            for(const lc::LinkResponse& secondReceivedMessage : data.secondReceivedMessages)
            {
                if(data.firstReceivedMessages.full())
                {
                    data.firstReceivedMessages.pop_front();
                }

                data.firstReceivedMessages.push_back(secondReceivedMessage);
            }

            data.secondReceivedMessages.clear();

            if(data.firstReceivedMessages.full())
            {
                data.firstReceivedMessages.pop_front();
            }

            data.firstReceivedMessages.push_back(response);
        }
    }

    void _resetStateCallback()
    {
        if(data.blockSendMessages)
        {
            data.clearSendMessages = true;
        }
        else
        {
            data.sendMessages.clear();
        }

        if(data.blockReceivedMessages)
        {
            data.clearReceivedMessages = true;
        }
        else
        {
            data.firstReceivedMessages.clear();
        }

        data.secondReceivedMessages.clear();
    }
}

void init()
{
    new(&data) static_data();

    data.connection.init(_sendDataCallback, _receiveResponseCallback, _resetStateCallback);
    irq::set_isr(irq::id::SERIAL, _serial_intr);
    irq::set_isr(irq::id::TIMER1, _timer_intr);
    data.connection.deactivate();
}

bool active()
{
    return data.active;
}

void enable()
{
    data.connection.activate();
    irq::enable(irq::id::SERIAL);
    irq::enable(irq::id::TIMER1);
}

void disable()
{
    irq::disable(irq::id::TIMER1);
    irq::disable(irq::id::SERIAL);
    data.connection.deactivate();
}

void deactivate()
{
    if(data.active)
    {
        data.active = false;
        disable();
    }
}

void send(int data_to_send)
{
    _check_active();

    BN_BARRIER;
    data.blockSendMessages = true;
    BN_BARRIER;

    if(data.sendMessages.full())
    {
        data.sendMessages.pop_front();
    }

    data.sendMessages.push_back(uint16_t(data_to_send));

    BN_BARRIER;
    data.blockSendMessages = false;
    BN_BARRIER;

    if(data.clearSendMessages)
    {
        BN_BARRIER;
        data.blockSendMessages = true;
        BN_BARRIER;

        data.sendMessages.clear();

        BN_BARRIER;
        data.blockSendMessages = false;
        BN_BARRIER;

        data.clearSendMessages = false;
    }
}

bool receive(lc::LinkResponse& response)
{
    _check_active();

    BN_BARRIER;
    data.blockReceivedMessages = true;
    BN_BARRIER;

    bool success = ! data.firstReceivedMessages.empty();

    if(success)
    {
        response = data.firstReceivedMessages.front();
        data.firstReceivedMessages.pop_front();
    }

    BN_BARRIER;
    data.blockReceivedMessages = false;
    BN_BARRIER;

    if(data.clearReceivedMessages)
    {
        BN_BARRIER;
        data.blockReceivedMessages = true;
        BN_BARRIER;

        data.firstReceivedMessages.clear();

        BN_BARRIER;
        data.blockReceivedMessages = false;
        BN_BARRIER;

        data.clearReceivedMessages = false;

        success = false;
    }

    return success;
}

void _serial_intr()
{
    data.connection._onSerial();
}

void _timer_intr()
{
    data.connection._onTimer();
}

void commit()
{
    if(data.active)
    {
        data.connection._onVBlank();
    }
}

}
