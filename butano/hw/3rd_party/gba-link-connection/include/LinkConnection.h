#ifndef LINK_CONNECTION_H
#define LINK_CONNECTION_H

#include <tonc_core.h>
#include <tonc_memdef.h>
#include <tonc_memmap.h>

#include "bn_deque.h"

#define LINK_MAX_PLAYERS 4
#define LINK_DISCONNECTED 0xFFFF
#define LINK_NO_DATA 0x0
#define LINK_TRANSFER_VCOUNT_WAIT 2
#define LINK_DEFAULT_TIMEOUT 3
#define LINK_DEFAULT_BUFFER_SIZE 64

#define LINK_BIT_SLAVE 2
#define LINK_BIT_READY 3
#define LINK_BITS_PLAYER_ID 4
#define LINK_BIT_ERROR 6
#define LINK_BIT_START 7
#define LINK_BIT_MULTIPLAYER 13
#define LINK_BIT_IRQ 14
#define LINK_BIT_GENERAL_PURPOSE_LOW 14
#define LINK_BIT_GENERAL_PURPOSE_HIGH 15
#define LINK_SET_HIGH(REG, BIT) REG |= 1 << BIT
#define LINK_SET_LOW(REG, BIT) REG &= ~(1 << BIT)

// A Link Cable connection for Multi-player mode.

// Usage:
// - 1) Include this header in your main.cpp file and add:
//       LinkConnection* linkConnection = new LinkConnection();
// - 2) Add the interrupt service routines:
//       irq_add(II_VBLANK, LINK_ISR_VBLANK);
//       irq_add(II_SERIAL, LINK_ISR_SERIAL);
// - 3) Send/read messages by using:
//       linkConnection->send(...);
//       linkConnection->linkState

// `data` restrictions:
// 0xFFFF and 0x0 are reserved values, so don't use them
// (they mean 'disconnected' and 'no data' respectively)

void LINK_ISR_VBLANK();
void LINK_ISR_SERIAL();
u16 LINK_QUEUE_POP(bn::ideque<u16>& q);
void LINK_QUEUE_CLEAR(bn::ideque<u16>& q);

struct LinkState {
    bn::deque<u16, LINK_DEFAULT_BUFFER_SIZE> _incomingMessages[LINK_MAX_PLAYERS];
    bn::deque<u16, LINK_DEFAULT_BUFFER_SIZE> _outgoingMessages;
    u32 _IRQTimeout;
    bool _IRQFlag;
    u8 playerCount;
    u8 currentPlayerId;

    bool isConnected() {
        return playerCount > 1 && currentPlayerId < playerCount;
    }

    bool hasMessage(u8 playerId) {
        if (playerId >= playerCount)
            return false;

        return !_incomingMessages[playerId].empty();
    }

    u16 readMessage(u8 playerId) {
        return LINK_QUEUE_POP(_incomingMessages[playerId]);
    }
};

class LinkConnection {
public:
    enum BaudRate {
        BAUD_RATE_0,  // 9600 bps
        BAUD_RATE_1,  // 38400 bps
        BAUD_RATE_2,  // 57600 bps
        BAUD_RATE_3   // 115200 bps
    };

    LinkState linkState;

    void init(bool startNow = true,
              BaudRate _baudRate = BAUD_RATE_3,
              u32 _timeout = LINK_DEFAULT_TIMEOUT) {
        this->baudRate = _baudRate;
        this->timeout = _timeout;

        if (startNow)
            activate();
        else
            stop();
    }

    bool isActive() { return isEnabled; }

    void activate() {
        isEnabled = true;
        reset();
    }

    void deactivate() {
        isEnabled = false;
        resetState();
        stop();
    }

    void send(u16 data) {
        if (data == LINK_DISCONNECTED || data == LINK_NO_DATA)
            return;

        push(linkState._outgoingMessages, data);
    }

    bool isReady() {
        return isBitHigh(LINK_BIT_READY) && !isBitHigh(LINK_BIT_ERROR);
    }

    void _onVBlank() {
        if (!isEnabled || resetIfNeeded())
            return;

        if (!linkState._IRQFlag) {
            linkState._IRQTimeout++;

            if (linkState._IRQTimeout >= timeout)
                reset();
            else if (isMaster())
                transfer(LINK_NO_DATA, true);
        }

        linkState._IRQFlag = false;
    }

    void _onSerial() {
        if (!isEnabled || resetIfNeeded())
            return;

        linkState._IRQFlag = true;
        linkState._IRQTimeout = 0;

        linkState.playerCount = 0;
        linkState.currentPlayerId =
                (REG_SIOCNT & (0b11 << LINK_BITS_PLAYER_ID)) >> LINK_BITS_PLAYER_ID;

        for (u32 i = 0; i < LINK_MAX_PLAYERS; i++) {
            u16 data = REG_SIOMULTI[i];

            if (data != LINK_DISCONNECTED) {
                if (data != LINK_NO_DATA)
                    push(linkState._incomingMessages[i], data);
                linkState.playerCount++;
            } else
                LINK_QUEUE_CLEAR(linkState._incomingMessages[i]);
        }

        if (linkState.isConnected())
            sendPendingData();
    }

private:
    BaudRate baudRate;
    u32 timeout;
    bool isEnabled = false;

    void sendPendingData() {
        transfer(LINK_QUEUE_POP(linkState._outgoingMessages));
    }

    void transfer(u16 data, bool force = false) {
        bool shouldNotify = isMaster() && (data != LINK_NO_DATA || force);

        if (shouldNotify)
            setBitLow(LINK_BIT_START);

        wait(LINK_TRANSFER_VCOUNT_WAIT);
        REG_SIOMLT_SEND = data;

        if (shouldNotify)
            setBitHigh(LINK_BIT_START);
    }

    bool resetIfNeeded() {
        if (!isReady()) {
            reset();
            return true;
        }

        return false;
    }

    void reset() {
        resetState();
        stop();
        start();
    }

    void resetState() {
        linkState.playerCount = 0;
        linkState.currentPlayerId = 0;
        for (u32 i = 0; i < LINK_MAX_PLAYERS; i++)
            LINK_QUEUE_CLEAR(linkState._incomingMessages[i]);
        LINK_QUEUE_CLEAR(linkState._outgoingMessages);
        linkState._IRQFlag = false;
        linkState._IRQTimeout = 0;
    }

    void stop() {
        LINK_SET_LOW(REG_RCNT, LINK_BIT_GENERAL_PURPOSE_LOW);
        LINK_SET_HIGH(REG_RCNT, LINK_BIT_GENERAL_PURPOSE_HIGH);
    }

    void start() {
        LINK_SET_LOW(REG_RCNT, LINK_BIT_GENERAL_PURPOSE_HIGH);
        REG_SIOCNT = baudRate;
        REG_SIOMLT_SEND = 0;
        setBitHigh(LINK_BIT_MULTIPLAYER);
        setBitHigh(LINK_BIT_IRQ);
    }

    void push(bn::ideque<u16>& q, u16 value) {
        if (q.full())
            q.pop_front();

        q.push_back(value);
    }

    void wait(u32 verticalLines) {
        u32 lines = 0;
        u32 vCount = REG_VCOUNT;

        while (lines < verticalLines) {
            if (REG_VCOUNT != vCount) {
                lines++;
                vCount = REG_VCOUNT;
            }
        };
    }

    bool isMaster() { return !isBitHigh(LINK_BIT_SLAVE); }
    bool isBitHigh(u8 bit) { return (REG_SIOCNT >> bit) & 1; }
    void setBitHigh(u8 bit) { LINK_SET_HIGH(REG_SIOCNT, bit); }
    void setBitLow(u8 bit) { LINK_SET_LOW(REG_SIOCNT, bit); }
};

extern LinkConnection* linkConnection;

inline void LINK_ISR_VBLANK() {
    linkConnection->_onVBlank();
}

inline void LINK_ISR_SERIAL() {
    linkConnection->_onSerial();
}

inline u16 LINK_QUEUE_POP(bn::ideque<u16>& q) {
    if (q.empty())
        return LINK_NO_DATA;

    u16 value = q.front();
    q.pop_front();
    return value;
}

inline void LINK_QUEUE_CLEAR(bn::ideque<u16>& q) {
    q.clear();
}

#endif  // LINK_CONNECTION_H
