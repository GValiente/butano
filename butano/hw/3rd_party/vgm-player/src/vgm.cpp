#include "../include/vgm.h"

#include <cstring>
#include "../../gbt-player/include/gbt_hardware.h"

//---------------------------------------------------------------------------
typedef unsigned char u8;
typedef unsigned int u32;
#define REG_BASE 0x04000000

//---------------------------------------------------------------------------
#define VGM_CMD_WAIT 0x61	// wait vblank
#define VGM_CMD_EOM  0x66	// end of mark
#define VGM_CMD_WREG 0xb3	// write register

//---------------------------------------------------------------------------
typedef struct {

    const u8* pCur;
    const u8* pFile;
    uint16_t oldRegSoundcntL;
    u8 id;
    bool isLoop;

} ST_VGM;

enum {
    VGM_ID_STOP,
    VGM_ID_PLAY,
};

//---------------------------------------------------------------------------
__attribute__((section(".ewram"))) ST_VGM Vgm;


//---------------------------------------------------------------------------
void VgmInit(void)
{
    memset(&Vgm, 0x00, sizeof(ST_VGM));
}
//---------------------------------------------------------------------------
void VgmPlay(const u8* pFile, bool isLoop)
{
	VgmInit();
	VgmStop();

	Vgm.id     = VGM_ID_PLAY;
	Vgm.pCur   = pFile;
	Vgm.pFile  = pFile;
	Vgm.isLoop = isLoop;
}
//---------------------------------------------------------------------------
void VgmStop(void)
{
	Vgm.id = VGM_ID_STOP;

	// REG_SOUNDCNT
	*(u8*)(REG_BASE + 0x84) = 0x00;

	// ch1
	*(u8*)(REG_BASE + 0x60) = 0x00;
	*(u8*)(REG_BASE + 0x62) = 0x00;
	*(u8*)(REG_BASE + 0x63) = 0x00;
	*(u8*)(REG_BASE + 0x64) = 0x00;
	*(u8*)(REG_BASE + 0x65) = 0x00;

	// ch2
	*(u8*)(REG_BASE + 0x68) = 0x00;
	*(u8*)(REG_BASE + 0x69) = 0x00;
	*(u8*)(REG_BASE + 0x6c) = 0x00;
	*(u8*)(REG_BASE + 0x6d) = 0x00;

	// ch3
	for(u32 i=0; i<0x10; i++)
	{
		*(u8*)(REG_BASE + 0x90 + i) = 0;
	}
	*(u8*)(REG_BASE + 0x70) = 0x00;
	*(u8*)(REG_BASE + 0x72) = 0x00;
	*(u8*)(REG_BASE + 0x74) = 0x80;

	// ch4
	*(u8*)(REG_BASE + 0x78) = 0x00;
	*(u8*)(REG_BASE + 0x79) = 0x00;
	*(u8*)(REG_BASE + 0x7c) = 0x00;
	*(u8*)(REG_BASE + 0x7d) = 0x00;

	// REG_SOUNDCNT
	*(u8*)(REG_BASE + 0x80) = 0x77;
	*(u8*)(REG_BASE + 0x81) = 0xff;
	*(u8*)(REG_BASE + 0x84) = 0x80;
}
//---------------------------------------------------------------------------
bool VgmIntrVblank(void)
{
	if(Vgm.id == VGM_ID_STOP)
	{
        return true;
	}

	for(;;)
	{
		u8 cmd = *Vgm.pCur++;

		if(cmd == VGM_CMD_WAIT)
		{
            return true;
		}

		if(cmd == VGM_CMD_WREG)
		{
			u8 adr = *Vgm.pCur++;
			u8 dat = *Vgm.pCur++;
			*(u8*)(REG_BASE + adr) = dat;

			continue;
		}

		if(cmd == VGM_CMD_EOM)
		{
			if(Vgm.isLoop == false)
			{
				VgmStop();

                return true;
			}

			u32 loop;

			loop  = *Vgm.pCur++;
			loop |= *Vgm.pCur++ << 8;
			loop |= *Vgm.pCur++ << 16;
			loop |= *Vgm.pCur++ << 24;

			Vgm.pCur = Vgm.pFile + loop;
			continue;
		}

        return false;
	}
}
//---------------------------------------------------------------------------
bool VgmActive(void)
{
	return Vgm.id == VGM_ID_PLAY;
}

u32 VgmGetOffsetPlay(void)
{
	return (u32)(Vgm.pCur - Vgm.pFile);
}

void VgmSetOffsetPlay(unsigned offset)
{
    Vgm.pCur = Vgm.pFile + offset;
}

//---------------------------------------------------------------------------
void VgmPause(void)
{
    Vgm.oldRegSoundcntL = REG_SOUNDCNT_L;
    Vgm.id = VGM_ID_STOP;

    uint16_t mask =
        SOUNDCNT_L_PSG_1_ENABLE_RIGHT | SOUNDCNT_L_PSG_1_ENABLE_LEFT |
        SOUNDCNT_L_PSG_2_ENABLE_RIGHT | SOUNDCNT_L_PSG_2_ENABLE_LEFT |
        SOUNDCNT_L_PSG_3_ENABLE_RIGHT | SOUNDCNT_L_PSG_3_ENABLE_LEFT |
        SOUNDCNT_L_PSG_4_ENABLE_RIGHT | SOUNDCNT_L_PSG_4_ENABLE_LEFT;

    REG_SOUNDCNT_L &= ~mask;
}

void VgmResume(void)
{
    Vgm.id = VGM_ID_PLAY;

    REG_SOUNDCNT_L = Vgm.oldRegSoundcntL;
}
