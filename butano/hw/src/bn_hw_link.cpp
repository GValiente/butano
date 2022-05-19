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
        bn::deque<LinkConnection::Response, LINK_DEFAULT_BUFFER_SIZE> firstReceivedMessages;
        bn::deque<LinkConnection::Response, LINK_DEFAULT_BUFFER_SIZE> secondReceivedMessages;
        volatile bool blockSendMessages = false;
        volatile bool blockReceivedMessages = false;
        volatile bool clearSendMessages = false;
        volatile bool clearReceivedMessages = false;
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

    void _receiveResponseCallback(const LinkConnection::Response& response)
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
            for(const LinkConnection::Response& secondReceivedMessage : data.secondReceivedMessages)
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

void init(LinkConnection& connection_ref)
{
    linkConnection = &connection_ref;
    connection_ref.init(_sendDataCallback, _receiveResponseCallback, _resetStateCallback);
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
        data.blockSendMessages = true;

        data.sendMessages.clear();

        data.blockSendMessages = false;
        data.clearSendMessages = false;
    }
}

bool receive(LinkConnection::Response& response)
{
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
    linkConnection->_onSerial();
}

void _timer_intr()
{
    linkConnection->_onTimer();
}

}
