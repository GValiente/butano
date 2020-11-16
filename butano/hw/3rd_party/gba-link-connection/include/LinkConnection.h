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

// A Link Cable connection for Multi-player mode.

// Usage:
// - 1) Include this header in your main.cpp file and add:
//       LinkConnection* linkConnection = new LinkConnection();
// - 2) Add the interrupt service routine:
//       irq_add(II_SERIAL, ISR_serial);
// - 3) Add to your update loop:
//       auto linkState = linkConnection->tick(data);
// - 4) Use `linkState` to process updates

// `data` restrictions:
// - 0xFFFF means 'disconnected', so don't use it

struct LinkState {
    u16 data[4];
    unsigned currentPlayerId = 0;

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
        setBitHigh(LINK_BIT_MULTIPLAYER);
        setBitHigh(LINK_BIT_IRQ);
    }

    LinkState tick(u16 data) {
        LinkState result;
        REG_SIOMLT_SEND = data;

        if (!isBitHigh(LINK_BIT_READY) || isBitHigh(LINK_BIT_ERROR)) {
            result.clear();
        }
        else {
            if (!isBitHigh(LINK_BIT_SLAVE))
                setBitHigh(LINK_BIT_START);

            result = _linkState;
        }

        _linkState.clear();
        return result;
    }

private:
    static bool isBitHigh(unsigned bit) {
        return (REG_SIOCNT & (1 << bit)) != 0;
    }

    static void setBitHigh(unsigned bit) {
        REG_SIOCNT |= 1 << bit;
    }
};

extern LinkConnection* linkConnection;

inline void ISR_serial() {
    LinkState newLinkState;
    newLinkState.currentPlayerId = (REG_SIOCNT & (0b11 << LINK_BITS_PLAYER_ID)) >> LINK_BITS_PLAYER_ID;
    newLinkState.data[0] = REG_SIOMULTI[0];
    newLinkState.data[1] = REG_SIOMULTI[1];
    newLinkState.data[2] = REG_SIOMULTI[2];
    newLinkState.data[3] = REG_SIOMULTI[3];
    linkConnection->_linkState = newLinkState;
}

#endif  // LINK_CONNECTION_H
