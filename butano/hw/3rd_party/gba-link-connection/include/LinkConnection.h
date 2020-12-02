#ifndef LINK_CONNECTION_H
#define LINK_CONNECTION_H

#include <tonc_core.h>
#include <tonc_memdef.h>
#include <tonc_memmap.h>

#include "bn_deque.h"
#include "bn_config_link.h"

static_assert(BN_CFG_LINK_BAUD_RATE == BN_LINK_BAUD_RATE_9600_BPS ||
              BN_CFG_LINK_BAUD_RATE == BN_LINK_BAUD_RATE_38400_BPS ||
              BN_CFG_LINK_BAUD_RATE == BN_LINK_BAUD_RATE_57600_BPS ||
              BN_CFG_LINK_BAUD_RATE == BN_LINK_BAUD_RATE_115200_BPS);

static_assert(BN_CFG_LINK_SEND_WAIT > 0);

static_assert(bn::power_of_two(BN_CFG_LINK_MAX_MESSAGES));

static_assert(BN_CFG_LINK_MAX_MISSING_MESSAGES >= 0);


#define LINK_MAX_PLAYERS 4
#define LINK_DISCONNECTED 0xFFFF
#define LINK_NO_DATA 0x0

#define LINK_DEFAULT_TIMEOUT (BN_CFG_LINK_MAX_MISSING_MESSAGES + 1)
#define LINK_DEFAULT_REMOTE_TIMEOUT LINK_DEFAULT_TIMEOUT
#define LINK_DEFAULT_BUFFER_SIZE BN_CFG_LINK_MAX_MESSAGES
#define LINK_DEFAULT_INTERVAL BN_CFG_LINK_SEND_WAIT
#define LINK_DEFAULT_SEND_TIMER_ID 1
#define LINK_BASE_FREQUENCY TM_FREQ_1024
#define LINK_REMOTE_TIMEOUT_OFFLINE -1

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
// - 2) Add the required interrupt service routines:
//       irq_init(NULL);
//       irq_add(II_VBLANK, LINK_ISR_VBLANK);
//       irq_add(II_SERIAL, LINK_ISR_SERIAL);
//       irq_add(II_TIMER3, LINK_ISR_TIMER);
//       irq_add(II_TIMER2, NULL);
// - 3) Initialize the library with:
//       linkConnection->activate();
// - 4) Send/read messages by using:
//       linkConnection->send(...);
//       linkConnection->linkState

// `data` restrictions:
// 0xFFFF and 0x0 are reserved values, so don't use them
// (they mean 'disconnected' and 'no data' respectively)

void LINK_ISR_VBLANK();
void LINK_ISR_TIMER();
void LINK_ISR_SERIAL();
u16 LINK_QUEUE_POP(bn::ideque<u16>& q);
void LINK_QUEUE_CLEAR(bn::ideque<u16>& q);

struct LinkState {
    bn::deque<u16, LINK_DEFAULT_BUFFER_SIZE> _incomingMessages[LINK_MAX_PLAYERS];
    bn::deque<u16, LINK_DEFAULT_BUFFER_SIZE> _outgoingMessages;
    int _timeouts[LINK_MAX_PLAYERS];
    u32 _IRQTimeout;
    u8 playerCount;
    u8 currentPlayerId;
    bool _IRQFlag;
    
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
    LinkState linkState;
    
    void init() {
        stop();
    }
    
    bool isActive() {
        return isEnabled;
    }
    
    void activate() {
        isEnabled = true;
        reset();
    }
    
    void deactivate() {
        isEnabled = false;
        resetState();
        stop();
    }

    void block() {
        isBlocked = true;
    }

    void unblock() {
        isBlocked = false;
    }
    
    void send(u16 data) {
        if (data == LINK_DISCONNECTED || data == LINK_NO_DATA)
            return;
        
        push(linkState._outgoingMessages, data);
    }
    
    void _onVBlank() {
        if (!isEnabled || isBlocked)
            return;
        
        if (!linkState._IRQFlag)
            linkState._IRQTimeout++;
        
        linkState._IRQFlag = false;
    }
    
    void _onTimer() {
        if (!isEnabled || isBlocked)
            return;
        
        if (didTimeout()) {
            reset();
            return;
        }
        
        if (isMaster() && isReady() && !isSending())
            sendPendingData();
    }
    
    void _onSerial() {
        if (!isEnabled || isBlocked)
            return;
        
        if (resetIfNeeded())
            return;
        
        linkState._IRQFlag = true;
        linkState._IRQTimeout = 0;
        
        unsigned newPlayerCount = 0;
        for (u32 i = 0; i < LINK_MAX_PLAYERS; i++) {
            u16 data = REG_SIOMULTI[i];
            
            if (data != LINK_DISCONNECTED) {
                if (data != LINK_NO_DATA && i != linkState.currentPlayerId)
                    push(linkState._incomingMessages[i], data);
                newPlayerCount++;
                linkState._timeouts[i] = 0;
            }
            else if (linkState._timeouts[i] > LINK_REMOTE_TIMEOUT_OFFLINE) {
                if (linkState._timeouts[i] >= LINK_DEFAULT_REMOTE_TIMEOUT) {
                    LINK_QUEUE_CLEAR(linkState._incomingMessages[i]);
                    linkState._timeouts[i] = LINK_REMOTE_TIMEOUT_OFFLINE;
                }
                else {
                    linkState._timeouts[i]++;
                    newPlayerCount++;
                }
            }
        }
        
        linkState.playerCount = newPlayerCount;
        linkState.currentPlayerId =
                (REG_SIOCNT & (0b11 << LINK_BITS_PLAYER_ID)) >> LINK_BITS_PLAYER_ID;
        
        if (!isMaster())
            sendPendingData();
    }
    
private:
    bool isEnabled = false;
    bool isBlocked = false;
    
    bool isReady() { return isBitHigh(LINK_BIT_READY); }
    bool hasError() { return isBitHigh(LINK_BIT_ERROR); }
    bool isMaster() { return !isBitHigh(LINK_BIT_SLAVE); }
    bool isSending() { return isBitHigh(LINK_BIT_START); }
    bool didTimeout() { return linkState._IRQTimeout >= LINK_DEFAULT_TIMEOUT; }
    
    void sendPendingData() {
        transfer(LINK_QUEUE_POP(linkState._outgoingMessages));
    }
    
    void transfer(u16 data) {
        REG_SIOMLT_SEND = data;
        
        if (isMaster()) {
            setBitHigh(LINK_BIT_START);
        }
    }
    
    bool resetIfNeeded() {
        if (!isReady() || hasError()) {
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
        for (u32 i = 0; i < LINK_MAX_PLAYERS; i++) {
            LINK_QUEUE_CLEAR(linkState._incomingMessages[i]);
            linkState._timeouts[i] = LINK_REMOTE_TIMEOUT_OFFLINE;
        }
        LINK_QUEUE_CLEAR(linkState._outgoingMessages);
        linkState._IRQFlag = false;
        linkState._IRQTimeout = 0;
    }
    
    void stop() {
        stopTimer();
        
        LINK_SET_LOW(REG_RCNT, LINK_BIT_GENERAL_PURPOSE_LOW);
        LINK_SET_HIGH(REG_RCNT, LINK_BIT_GENERAL_PURPOSE_HIGH);
    }
    
    void start() {
        startTimer();

        LINK_SET_LOW(REG_RCNT, LINK_BIT_GENERAL_PURPOSE_HIGH);
        REG_SIOCNT = BN_CFG_LINK_BAUD_RATE;
        REG_SIOMLT_SEND = 0;
        setBitHigh(LINK_BIT_MULTIPLAYER);
        setBitHigh(LINK_BIT_IRQ);
    }
    
    void stopTimer() {
        REG_TM[LINK_DEFAULT_SEND_TIMER_ID].cnt = REG_TM[LINK_DEFAULT_SEND_TIMER_ID].cnt & (~TM_ENABLE);
    }
    
    void startTimer() {
        REG_TM[LINK_DEFAULT_SEND_TIMER_ID].start = -LINK_DEFAULT_INTERVAL;
        REG_TM[LINK_DEFAULT_SEND_TIMER_ID].cnt = TM_ENABLE | TM_IRQ | LINK_BASE_FREQUENCY;
    }
    
    void push(bn::ideque<u16>& q, u16 value) {
        if (q.full())
            q.pop_front();
        
        q.push_back(value);
    }
    
    bool isBitHigh(unsigned bit) { return (REG_SIOCNT >> bit) & 1; }
    void setBitHigh(unsigned bit) { LINK_SET_HIGH(REG_SIOCNT, bit); }
    void setBitLow(unsigned bit) { LINK_SET_LOW(REG_SIOCNT, bit); }
};

extern LinkConnection* linkConnection;

inline void LINK_ISR_VBLANK() {
    linkConnection->_onVBlank();
}

inline void LINK_ISR_TIMER() {
    linkConnection->_onTimer();
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
