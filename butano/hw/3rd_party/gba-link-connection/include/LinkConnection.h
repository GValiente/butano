#ifndef LINK_CONNECTION_H
#define LINK_CONNECTION_H

#include <tonc_core.h>
#include <tonc_memmap.h>

#define LINK_MAX_PLAYERS 4
#define LINK_NO_DATA 0xffff

#define LINK_BIT_SLAVE 2
#define LINK_BIT_READY 3
#define LINK_BITS_PLAYER_ID 4
#define LINK_BIT_ERROR 6
#define LINK_BIT_START 7
#define LINK_BIT_MULTIPLAYER 13
#define LINK_BIT_IRQ 14

struct LinkState {
    u16 data[4];
    unsigned currentPlayerId = 0;

    int playerCount() {
        return hasData(0) + hasData(1) + hasData(2) + hasData(3);
    }

    bool isConnected() {
        return playerCount() > 1;
    }

    bool hasData(u8 playerId) { return data[playerId] != LINK_NO_DATA; }

    void clear() {
        currentPlayerId = 0;
        for (u32 i = 0; i < LINK_MAX_PLAYERS; i++)
           data[i] = LINK_NO_DATA;
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
    struct LinkState _linkState;

    void init(BaudRate baudRate) {
        REG_RCNT = 0;
        REG_SIOCNT = (u8)baudRate;
        this->setBitHigh(LINK_BIT_MULTIPLAYER);
        this->setBitHigh(LINK_BIT_IRQ);
    }

    LinkState tick(u16 data) {
        REG_SIOMLT_SEND = data;

        if (!isBitHigh(LINK_BIT_READY) || isBitHigh(LINK_BIT_ERROR)) {
            _linkState.clear();
            return _linkState;
        }

        if (!isBitHigh(LINK_BIT_SLAVE)) {
            if (isWaiting) {
                if (!isBitHigh(LINK_BIT_START))
                    isWaiting = false;
                return _linkState;
            }

            setBitHigh(LINK_BIT_START);
            isWaiting = true;
        }

        return _linkState;
    }

private:
    bool isWaiting = false;

    static bool isBitHigh(u8 bit) { return (REG_SIOCNT & (1 << bit)) != 0; }
    static void setBitHigh(u8 bit) { REG_SIOCNT |= 1 << bit; }
};

extern LinkConnection* linkConnection;

inline void ISR_serial() {
    LinkState newLinkState;
    bool ready = REG_SIOCNT & (1 << LINK_BIT_READY);
    bool error = REG_SIOCNT & (1 << LINK_BIT_ERROR);

    if(ready && ! error)
    {
        newLinkState.currentPlayerId = (REG_SIOCNT & (0b11 << LINK_BITS_PLAYER_ID)) >> LINK_BITS_PLAYER_ID;
        newLinkState.data[0] = REG_SIOMULTI0;
        newLinkState.data[1] = REG_SIOMULTI1;
        newLinkState.data[2] = REG_SIOMULTI2;
        newLinkState.data[3] = REG_SIOMULTI3;
    }
    else
    {
        newLinkState.data[0] = LINK_NO_DATA;
        newLinkState.data[1] = LINK_NO_DATA;
        newLinkState.data[2] = LINK_NO_DATA;
        newLinkState.data[3] = LINK_NO_DATA;
    }

    linkConnection->_linkState = newLinkState;
}

#endif  // LINK_CONNECTION_H
