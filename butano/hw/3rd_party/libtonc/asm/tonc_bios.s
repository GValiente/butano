//
//	Main GBA BIOS functions.
//
//! \file tonc_bios.s
//! \author J Vijn
//! \date 20071130 - 20090801

#include "../include/tonc_asminc.h"

@ === SoftReset [00h] =================================================
@ DECL: void SoftReset();
@ DESC: 
BEGIN_FUNC_THUMB(SoftReset, CSEC_TEXT)
	swi		0x00
	bx		lr
END_FUNC(SoftReset)

@ === RegisterRamReset [01h] ==========================================
@ DECL: void RegisterRamReset(u32 flags);
@ DESC: 
BEGIN_FUNC_THUMB(RegisterRamReset, CSEC_TEXT)
	swi		0x01
	bx		lr
END_FUNC(RegisterRamReset)

@ === Halt [02h] ======================================================
@ DECL: void Halt();
@ DESC: 
BEGIN_FUNC_THUMB(Halt, CSEC_TEXT)
	swi		0x02
	bx		lr
END_FUNC(Halt)

@ === Stop [03h] ======================================================
@ DECL: void Stop();
@ DESC: 
BEGIN_FUNC_THUMB(Stop, CSEC_TEXT)
	swi		0x03
	bx		lr
END_FUNC(Stop)

@ === IntrWait [04h] ==================================================
@ DECL: void IntrWait(u32 flagClear, u32 irq);
@ DESC: 
BEGIN_FUNC_THUMB(IntrWait, CSEC_TEXT)
	swi		0x04
	bx		lr
END_FUNC(IntrWait)

@ === VBlankIntrWait [05h] ============================================
@ DECL: void VBlankIntrWait();
@ DESC: 
BEGIN_FUNC_THUMB(VBlankIntrWait, CSEC_TEXT)
	swi		0x05
	bx		lr
END_FUNC(VBlankIntrWait)

@ === Div [06h] =======================================================
@ DECL: s32 Div(s32 num, s32 den);
@ DESC: 
BEGIN_FUNC_THUMB(Div, CSEC_TEXT)
	swi		0x06
	bx		lr
END_FUNC(Div)

@ === DivArm [07h] ====================================================
@ DECL: s32 DivArm(s32 den, s32 num);
@ DESC: 
BEGIN_FUNC_THUMB(DivArm, CSEC_TEXT)
	swi		0x07
	bx		lr
END_FUNC(DivArm)

@ === Sqrt [08h] ======================================================
@ DECL: u32 Sqrt(u32 num);
@ DESC: 
BEGIN_FUNC_THUMB(Sqrt, CSEC_TEXT)
	swi		0x08
	bx		lr
END_FUNC(Sqrt)

@ === ArcTan [09h] ====================================================
@ DECL: s16 ArcTan(s16 dydx);
@ DESC: 
BEGIN_FUNC_THUMB(ArcTan, CSEC_TEXT)
	swi		0x09
	bx		lr
END_FUNC(ArcTan)

@ === ArcTan2 [0Ah] ===================================================
@ DECL: s16 ArcTan2(s16 x, s16 y);
@ DESC: 
BEGIN_FUNC_THUMB(ArcTan2, CSEC_TEXT)
	swi		0x0A
	bx		lr
END_FUNC(ArcTan2)

@ === CpuSet [0Bh] ====================================================
@ DECL: void CpuSet(const void *src, void *dst, u32 mode);
@ DESC: 
BEGIN_FUNC_THUMB(CpuSet, CSEC_TEXT)
	swi		0x0B
	bx		lr
END_FUNC(CpuSet)

@ === CpuFastSet [0Ch] ================================================
@ DECL: void CpuFastSet(const void *src, void *dst, u32 mode);
@ DESC: 
BEGIN_FUNC_THUMB(CpuFastSet, CSEC_TEXT)
	swi		0x0C
	bx		lr
END_FUNC(CpuFastSet)

@ === BiosCheckSum [0Dh] ================================================
@ DECL: u32 BiosCheckSum();
@ DESC: 
BEGIN_FUNC_THUMB(BiosCheckSum, CSEC_TEXT)
	swi		0x0D
	bx		lr
END_FUNC(BiosCheckSum)

@ === BgAffineSet [0Eh] ===============================================
@ DECL: void ObjAffineSet(const ObjAffineSource *src, void *dst, s32 num, s32 offset);
@ DESC: 
BEGIN_FUNC_THUMB(BgAffineSet, CSEC_TEXT)
	swi		0x0E
	bx		lr
END_FUNC(BgAffineSet)

@ === ObjAffineSet [0Fh] ==============================================
@ DECL: void BgAffineSet(const BGAffineSource *src, BGAffineDest *dst, s32 num);
@ DESC: 
BEGIN_FUNC_THUMB(ObjAffineSet, CSEC_TEXT)
	swi		0x0F
	bx		lr
END_FUNC(ObjAffineSet)

@ === BitUnPack [10h] =================================================
@ DECL: void BitUnPack(const void *src, void *dst, BUP *bup);
@ DESC: 
BEGIN_FUNC_THUMB(BitUnPack, CSEC_TEXT)
	swi		0x10
	bx		lr
END_FUNC(BitUnPack)

@ === LZ77UnCompWram [11h] ============================================
@ DECL: void LZ77UnCompWram(const void *src, void *dst);
@ DESC: 
BEGIN_FUNC_THUMB(LZ77UnCompWram, CSEC_TEXT)
	swi		0x11
	bx		lr
END_FUNC(LZ77UnCompWram)

@ === LZ77UnCompVram [12h] ============================================
@ DECL: void LZ77UnCompVram(const void *src, void *dst);
@ DESC: 
BEGIN_FUNC_THUMB(LZ77UnCompVram, CSEC_TEXT)
	swi		0x12
	bx		lr
END_FUNC(LZ77UnCompVram)

@ === HuffUnComp [13h] ================================================
@ DECL: void HuffUnComp(const void *src, void *dst);
@ DESC: 
BEGIN_FUNC_THUMB(HuffUnComp, CSEC_TEXT)
	swi		0x13
	bx		lr
END_FUNC(HuffUnComp)

@ === RLUnCompWram [14h] ==============================================
@ DECL: void RLUnCompWram(const void *src, void *dst);
@ DESC: 
BEGIN_FUNC_THUMB(RLUnCompWram, CSEC_TEXT)
	swi		0x14
	bx		lr
END_FUNC(RLUnCompWram)

@ === RLUnCompVram [15h] ==============================================
@ DECL: void RLUnCompVram(const void *src, void *dst);
@ DESC: 
BEGIN_FUNC_THUMB(RLUnCompVram, CSEC_TEXT)
	swi		0x15
	bx		lr
END_FUNC(RLUnCompVram)

@ === Diff8bitUnFilterWram [16h] ======================================
@ DECL: void Diff8bitUnFilterWram(const void *src, void *dst);
@ DESC: 
BEGIN_FUNC_THUMB(Diff8bitUnFilterWram, CSEC_TEXT)
	swi		0x16
	bx		lr
END_FUNC(Diff8bitUnFilterWram)

@ === Diff8bitUnFilterVram [17h] ======================================
@ DECL: void Diff8bitUnFilterVram(const void *src, void *dst);
@ DESC: 
BEGIN_FUNC_THUMB(Diff8bitUnFilterVram, CSEC_TEXT)
	swi		0x17
	bx		lr
END_FUNC(Diff8bitUnFilterVram)

@ === Diff16bitUnFilter [18h] =========================================
@ DECL: void Diff16bitUnFilter(const void *src, void *dst);
@ DESC: 
BEGIN_FUNC_THUMB(Diff16bitUnFilter, CSEC_TEXT)
	swi		0x18
	bx		lr
END_FUNC(Diff16bitUnFilter)

@ === SoundBias [19h] =================================================
@ DECL: void SoundBias(u32 bias);
@ DESC: 
BEGIN_FUNC_THUMB(SoundBias, CSEC_TEXT)
	swi		0x19
	bx		lr
END_FUNC(SoundBias)

@ === SoundDriverInit [1Ah] ===========================================
@ DECL: void SoundDriverInit(void *src);
@ DESC: 
BEGIN_FUNC_THUMB(SoundDriverInit, CSEC_TEXT)
	swi		0x1A
	bx		lr
END_FUNC(SoundDriverInit)

@ === SoundDriverMode [1Bh] ===========================================
@ DECL: void SoundDriverMode(u32 mode);
@ DESC: 
BEGIN_FUNC_THUMB(SoundDriverMode, CSEC_TEXT)
	swi		0x1B
	bx		lr
END_FUNC(SoundDriverMode)

@ === SoundDriverMain [1Ch] ===========================================
@ DECL: void SoundDriverMain();
@ DESC: 
BEGIN_FUNC_THUMB(SoundDriverMain, CSEC_TEXT)
	swi		0x1C
	bx		lr
END_FUNC(SoundDriverMain)

@ === SoundDriverVSync [1Dh] ==========================================
@ DECL: void SoundDriverVSync();
@ DESC: 
BEGIN_FUNC_THUMB(SoundDriverVSync, CSEC_TEXT)
	swi		0x1D
	bx		lr
END_FUNC(SoundDriverVSync)

@ === SoundChannelClear [1Eh] =========================================
@ DECL: void SoundChannelClear();
@ DESC: 
BEGIN_FUNC_THUMB(SoundChannelClear, CSEC_TEXT)
	swi		0x1E
	bx		lr
END_FUNC(SoundChannelClear)

@ === MidiKey2Freq [1Fh] ==============================================
@ DECL: u32 MidiKey2Freq(void *wa, u8 mk, u8 fp);
@ DESC: 
BEGIN_FUNC_THUMB(MidiKey2Freq, CSEC_TEXT)
	swi		0x1F
	bx		lr
END_FUNC(MidiKey2Freq)

@ === MultiBoot [25h] =================================================
@ DECL: int MultiBoot(MultiBootParam* mb, u32 mode);
@ DESC: 
BEGIN_FUNC_THUMB(MultiBoot, CSEC_TEXT)
	swi		0x25
	bx		lr
END_FUNC(MultiBoot)

@ === SoundDriverVSyncOff [28h] =======================================
@ DECL: void SoundDriverVSyncOff();
@ DESC: 
BEGIN_FUNC_THUMB(SoundDriverVSyncOff, CSEC_TEXT)
	swi		0x28
	bx		lr
END_FUNC(SoundDriverVSyncOff)

@ === SoundDriverVSyncOn [29h] ========================================
@ DECL: void SoundDriverVSyncOn();
@ DESC: 
BEGIN_FUNC_THUMB(SoundDriverVSyncOn, CSEC_TEXT)
	swi		0x29
	bx		lr
END_FUNC(SoundDriverVSyncOn)

