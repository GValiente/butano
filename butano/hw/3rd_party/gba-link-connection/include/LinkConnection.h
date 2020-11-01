#ifndef LINK_CONNECTION_H
#define LINK_CONNECTION_H

#include <tonc_core.h>
#include <tonc_memmap.h>

#define LINK_MAX_PLAYERS 4
#define LINK_NO_DATA 0xffff
#define LINK_HEART_BIT 0xf
#define LINK_HEART_BIT_UNKNOWN 2
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
// - Bit 0xF will be ignored: it'll be used as a heartbeat

inline bool isBitHigh(u32 bit) {
    return (REG_SIOCNT & (1 << bit)) != 0;
}

inline void setBitHigh(u32 bit) {
    REG_SIOCNT |= 1 << bit;
}

inline u16 withHeartBit(u16 data, bool heartBit) {
    return (data & ~(1 << LINK_HEART_BIT)) | (heartBit << LINK_HEART_BIT);
}

struct LinkState {
    u16 playerCount = 0;
    u16 currentPlayerId = 0;
    u16 data[4];
    u8 _heartBits[4];
    u32 _tick = 0;
    u32 _lastIRQTick = 0;

    bool isConnected() {
        return playerCount > 1;
    }

    bool hasData(u32 playerId) {
        return data[playerId] != LINK_NO_DATA;
    }

    bool _isOutOfSync() {
        return isConnected() && (_lastIRQTick != _tick || currentPlayerId >= playerCount);
    }

    void _sync() {
        _lastIRQTick = _tick;
    }

    void _reset() {
        playerCount = 0;
        currentPlayerId = 0;
        for (u32 i = 0; i < LINK_MAX_PLAYERS; i++) {
            data[i] = LINK_NO_DATA;
            _heartBits[i] = LINK_HEART_BIT_UNKNOWN;
        }
        _tick = 0;
        _lastIRQTick = 0;
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

    static void init(BaudRate baudRate) {
        REG_RCNT = 0;
        REG_SIOCNT = (u8)baudRate;
        setBitHigh(LINK_BIT_MULTIPLAYER);
        setBitHigh(LINK_BIT_IRQ);
    }

    LinkState tick(u16 data) {
        bool shouldForceReset = !isBitHigh(LINK_BIT_READY) ||
                isBitHigh(LINK_BIT_ERROR) ||
                _linkState._isOutOfSync();

        if (shouldForceReset) {
            resetCommunicationCircuit();
            _linkState._reset();
            return _linkState;
        }

        REG_SIOMLT_SEND = withHeartBit(data, heartBit);
        _linkState._tick++;
        heartBit = !heartBit;

        if (!isBitHigh(LINK_BIT_SLAVE))
            setBitHigh(LINK_BIT_START);

        return _linkState;
    }

private:
    bool heartBit = 0;

    static void resetCommunicationCircuit() {
        REG_RCNT = 0xf;
        REG_RCNT = 0;
    }
};

extern LinkConnection* linkConnection;

inline void ISR_serial() {
    LinkState& oldLinkState = linkConnection->_linkState;
    LinkState newLinkState;

    if(! isBitHigh(LINK_BIT_READY) || isBitHigh(LINK_BIT_ERROR)) {
        for (u32 i = 0; i < LINK_MAX_PLAYERS; i++) {
            newLinkState.data[i] = LINK_NO_DATA;
            newLinkState._heartBits[i] = LINK_HEART_BIT_UNKNOWN;
        }
    }
    else {
        newLinkState.currentPlayerId =
                (REG_SIOCNT & (0b11 << LINK_BITS_PLAYER_ID)) >> LINK_BITS_PLAYER_ID;

        for (u32 i = 0; i < LINK_MAX_PLAYERS; i++) {
            u32 data = REG_SIOMULTI[i];
            u32 oldHeartBit = oldLinkState._heartBits[i];
            u32 newHeartBit = (data & (1 << LINK_HEART_BIT)) != 0;
            bool isConnectionAlive =
                    data != LINK_NO_DATA &&
                    (oldHeartBit == LINK_HEART_BIT_UNKNOWN || oldHeartBit != newHeartBit);

            if(isConnectionAlive) {
                newLinkState.data[i] = withHeartBit(data, 0);
                newLinkState._heartBits[i] = newHeartBit;
                newLinkState.playerCount++;
            }
            else {
                newLinkState.data[i] = LINK_NO_DATA;
                newLinkState._heartBits[i] = LINK_HEART_BIT_UNKNOWN;
            }
        }
    }

    u32 tick = oldLinkState._tick;
    newLinkState._tick = tick;
    newLinkState._lastIRQTick = tick;
    oldLinkState = newLinkState;
}

#endif  // LINK_CONNECTION_H
