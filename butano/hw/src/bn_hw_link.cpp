/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_link.h"

LinkConnection* linkConnection = nullptr;

namespace bn::hw::link
{

namespace
{
    class static_data
    {

    public:
        bn::deque<uint16_t, LINK_DEFAULT_BUFFER_SIZE> sendMessages;
        volatile bool blockSendMessages = false;
        volatile bool clearSendMessages = false;
    };

    BN_DATA_EWRAM static_data data;

    void _sendDataCallback()
    {
        if(! data.blockSendMessages)
        {
            LinkConnection* lc = linkConnection;

            for(uint16_t message : data.sendMessages)
            {
                lc->send(message);
            }

            data.sendMessages.clear();
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
    }
}

void init(connection& connection_ref)
{
    linkConnection = &connection_ref;
    connection_ref.init(_sendDataCallback, _resetStateCallback);
    irq::replace_or_push_back_disabled(irq::id::SERIAL, _serial_intr);
    irq::replace_or_push_back_disabled(irq::id::TIMER1, _timer_intr);
    linkConnection->deactivate();
}

void send(int data_to_send)
{
    data.blockSendMessages = true;

    if(data.sendMessages.full())
    {
        data.sendMessages.pop_front();
    }

    data.sendMessages.push_back(data_to_send);

    data.blockSendMessages = false;

    if(data.clearSendMessages)
    {
        data.sendMessages.clear();
        data.clearSendMessages = false;
    }
}

void _serial_intr()
{
    linkConnection->_onSerial();
}

void _timer_intr()
{
    linkConnection->_onTimer();
}

}
