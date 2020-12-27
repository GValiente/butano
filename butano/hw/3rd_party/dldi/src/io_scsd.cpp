/*
 Hardware Routines for reading a Secure Digital card
 using the Supercard SD
 Copyright (c) 2006 Michael "Chishm" Chisholm

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:
  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.
  3. The name of the author may not be used to endorse or promote products derived
     from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "../include/io_scsd.h"

#include <cstdint>

namespace
{
    #define SEND_STATUS         13
    #define BUSY_WAIT_TIMEOUT   128
    #define REG_SCSD_CMD        (*(volatile uint16_t*)(0x09800000))

    uint8_t _SD_CRC7(uint8_t* data, int cnt) {
        int i, a;
        uint8_t crc, temp;

        crc = 0;
        for (a = 0; a < cnt; a++)
        {
            temp = data[a];
            for (i = 0; i < 8; i++)
            {
                crc <<= 1;
                if ((temp & 0x80) ^ (crc & 0x80)) crc ^= 0x09;
                temp <<= 1;
            }
        }
        crc = (crc << 1) | 1;
        return(crc);
    }

    bool _SCSD_sendCommand (uint8_t command, uint32_t argument) {
        uint8_t databuff[6];
        uint8_t *tempDataPtr = databuff;
        int length = 6;
        uint16_t dataByte;
        int curBit;
        int i;

        *tempDataPtr++ = command | 0x40;
        *tempDataPtr++ = argument>>24;
        *tempDataPtr++ = argument>>16;
        *tempDataPtr++ = argument>>8;
        *tempDataPtr++ = argument;
        *tempDataPtr = _SD_CRC7 (databuff, 5);

        i = BUSY_WAIT_TIMEOUT;
        while (((REG_SCSD_CMD & 0x01) == 0) && (--i));
        if (i == 0) {
            return false;
        }

        dataByte = REG_SCSD_CMD;

        tempDataPtr = databuff;

        while (length--) {
            dataByte = *tempDataPtr++;
            for (curBit = 7; curBit >=0; curBit--){
                REG_SCSD_CMD = dataByte;
                dataByte = dataByte << 1;
            }
        }

        return true;
    }
}

bool _SCSD_isInserted (void) {
    return _SCSD_sendCommand (SEND_STATUS, 0);
}
