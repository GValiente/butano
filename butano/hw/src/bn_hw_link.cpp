/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
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
        LinkConnection connection;
        bn::deque<uint16_t, LINK_DEFAULT_BUFFER_SIZE> sendMessages;
        bn::deque<LinkResponse, LINK_DEFAULT_BUFFER_SIZE> firstReceivedMessages;
        bn::deque<LinkResponse, LINK_DEFAULT_BUFFER_SIZE> secondReceivedMessages;
        volatile bool blockSendMessages = false;
        volatile bool blockReceivedMessages = false;
        volatile bool clearSendMessages = false;
        volatile bool clearReceivedMessages = false;
        bool active = false;
    };

    BN_DATA_EWRAM static_data data;

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

    void _receiveResponseCallback(const LinkResponse& response)
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
            for(const LinkResponse& secondReceivedMessage : data.secondReceivedMessages)
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
    data.connection.init(_sendDataCallback, _receiveResponseCallback, _resetStateCallback);
    irq::replace_or_push_back_disabled(irq::id::SERIAL, _serial_intr);
    irq::replace_or_push_back_disabled(irq::id::TIMER1, _timer_intr);
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

    data.blockSendMessages = true;

    if(data.sendMessages.full())
    {
        data.sendMessages.pop_front();
    }

    data.sendMessages.push_back(data_to_send);

    data.blockSendMessages = false;

    if(data.clearSendMessages)
    {
        data.blockSendMessages = true;

        data.sendMessages.clear();

        data.blockSendMessages = false;
        data.clearSendMessages = false;
    }
}

bool receive(LinkResponse& response)
{
    _check_active();

    data.blockReceivedMessages = true;

    bool success = ! data.firstReceivedMessages.empty();

    if(success)
    {
        response = data.firstReceivedMessages.front();
        data.firstReceivedMessages.pop_front();
    }

    data.blockReceivedMessages = false;

    if(data.clearReceivedMessages)
    {
        data.blockReceivedMessages = true;

        data.firstReceivedMessages.clear();

        data.blockReceivedMessages = false;
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
