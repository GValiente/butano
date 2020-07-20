//
//  GBA Memory map 
//
//! \file tonc_memmap.h
//! \author J Vijn
//! \date 20060508 - 20060508
//
// 
// === NOTES ===
//
// * The REG_BGxy registers for affine backgrounds
//   should be _signed_ (vs16 / vs32), not unsigned (vu16 / vu32)
// * I have removed several REG_x_L, REG_x_H pairs because all they 
//   do is clutter up the file
// * C++ doesn't seem to like struct copies if the type specifiers 
//   don't match (e.g., volatile, non-volatile). Most registers 
//   don't really need the volatile specifier anyway, so if this 
//   presents a problem consider removing it.
// * I'm using defines for the memory map here, but GCC cannot optimize 
//   these properly and they will often appear inside a loop, potentially
//   slowing it down to up 50% or so, depending on how much you do 
//   in the loop. Possible remedy: use a set of global pointers for the 
//   memory map instead of defines. It'll only be 4 or so pointers, so 
//   it should be ok. (PONDER: system with void pointers?)

#ifndef TONC_MEMMAP
#define TONC_MEMMAP

#ifndef __ASM__
#include "tonc_types.h"
#endif

/*!	\defgroup grpReg	IO Registers	*/
/*!	\defgroup grpRegAlt	IO Alternates	*/


// === MEMORY SECTIONS ================================================

/*! \addtogroup grpMemmap
	\brief Basic memory map
*/
/*!	\{	*/


//! \name Main sections
//\{
#define MEM_EWRAM	0x02000000	//!< External work RAM
#define MEM_IWRAM	0x03000000	//!< Internal work RAM
#define MEM_IO		0x04000000	//!< I/O registers
#define MEM_PAL		0x05000000	//!< Palette. Note: no 8bit write !!
#define MEM_VRAM	0x06000000	//!< Video RAM. Note: no 8bit write !!
#define MEM_OAM		0x07000000	//!< Object Attribute Memory (OAM) Note: no 8bit write !!
#define MEM_ROM		0x08000000	//!< ROM. No write at all (duh)
#define MEM_SRAM	0x0E000000	//!< Static RAM. 8bit write only
//\}


//! \name Main section sizes
//\{
#define EWRAM_SIZE	0x40000
#define IWRAM_SIZE	0x08000
#define PAL_SIZE	0x00400
#define VRAM_SIZE	0x18000
#define OAM_SIZE	0x00400
#define SRAM_SIZE	0x10000
//\}

//! \name Sub section sizes
//\{
#define PAL_BG_SIZE		0x00200		//!< BG palette size
#define PAL_OBJ_SIZE	0x00200		//!< Object palette size
#define CBB_SIZE		0x04000		//!< Charblock size
#define SBB_SIZE		0x00800		//!< Screenblock size
#define VRAM_BG_SIZE	0x10000		//!< BG VRAM size
#define VRAM_OBJ_SIZE	0x08000		//!< Object VRAM size
#define M3_SIZE			0x12C00		//!< Mode 3 buffer size
#define M4_SIZE			0x09600		//!< Mode 4 buffer size
#define M5_SIZE			0x0A000		//!< Mode 5 buffer size
#define VRAM_PAGE_SIZE	0x0A000		//!< Bitmap page size
//\}


//! \name Sub sections
//\{
#define REG_BASE		MEM_IO

#define MEM_PAL_BG		(MEM_PAL)					//!< Background palette address
#define MEM_PAL_OBJ		(MEM_PAL + PAL_BG_SIZE)		//!< Object palette address
#define MEM_VRAM_FRONT	(MEM_VRAM)					//!< Front page address
#define MEM_VRAM_BACK	(MEM_VRAM + VRAM_PAGE_SIZE)	//!< Back page address
#define MEM_VRAM_OBJ	(MEM_VRAM + VRAM_BG_SIZE)	//!< Object VRAM address
//\}

/*!	\}	*/


// --------------------------------------------------------------------
//  STRUCTURED MEMORY MAP 
// --------------------------------------------------------------------


/*! \defgroup grpMemArray Memory mapped arrays
	\ingroup grpMemmap
	\brief	These are some macros for easier access of various 
	  memory sections. They're all arrays or matrices, using the 
	  types that would be the most natural for that concept.
*/
/*	\{	*/


//! \name Palette
//\{

//! Background palette.
/*! pal_bg_mem[i]	= color i					( COLOR )
*/
#define pal_bg_mem		((COLOR*)MEM_PAL)

//! Object palette. 
/*! pal_obj_mem[i]	= color i					( COLOR )
*/
#define pal_obj_mem		((COLOR*)MEM_PAL_OBJ)


//! Background palette matrix. 
/*! pal_bg_bank[y]		= bank y				( COLOR[ ] )<br>
	pal_bg_bank[y][x]	= color color y*16+x	( COLOR )
*/
#define pal_bg_bank		((PALBANK*)MEM_PAL)

//! Object palette matrix. 
/*!	pal_obj_bank[y]		= bank y				( COLOR[ ] )<br>
	pal_obj_bank[y][x]	= color y*16+x			( COLOR )
*/
#define pal_obj_bank	((PALBANK*)MEM_PAL_OBJ)

//\}	// End Palette


//! \name VRAM
//\{

//!	Charblocks, 4bpp tiles.
/*!	tile_mem[y]		= charblock y				( TILE[ ] )<br>
	tile_mem[y][x]	= block y, tile x			( TILE )
*/
#define tile_mem		( (CHARBLOCK*)MEM_VRAM)

//!	Charblocks, 8bpp tiles.
/*!	tile_mem[y]		= charblock y				( TILE[ ] )<br>
	tile_mem[y][x]	= block y, tile x			( TILE )
*/
#define tile8_mem		((CHARBLOCK8*)MEM_VRAM)

//!	Object charblocks, 4bpp tiles.
/*!	tile_mem[y]		= charblock y				( TILE[ ] )<br>
	tile_mem[y][x]	= block y, tile x			( TILE )
*/
#define tile_mem_obj	( (CHARBLOCK*)MEM_VRAM_OBJ)

//!	Object charblocks, 4bpp tiles.
/*!	tile_mem[y]		= charblock y				( TILE[ ] )<br>
	tile_mem[y][x]	= block y, tile x			( TILE )
*/
#define tile8_mem_obj	((CHARBLOCK8*)MEM_VRAM_OBJ)

//! Screenblocks as arrays
/*!	se_mem[y]		= screenblock y				( SCR_ENTRY[ ] )<br>
*	se_mem[y][x]	= screenblock y, entry x	( SCR_ENTRY )
*/
#define se_mem			((SCREENBLOCK*)MEM_VRAM)


//! Screenblock as matrices
/*!	se_mat[s]		= screenblock s					( SCR_ENTRY[ ][ ] )<br>
	se_mat[s][y][x]	= screenblock s, entry (x,y)	( SCR_ENTRY )
*/
#define se_mat			((SCREENMAT*)MEM_VRAM)

//! Main mode 3/5 frame as an array
/*!	vid_mem[i]		= pixel i						( COLOR )
*/
#define vid_mem			((COLOR*)MEM_VRAM)

//! Mode 3 frame as a matrix
/*!	m3_mem[y][x]	= pixel (x, y)					( COLOR )
*/
#define m3_mem			((M3LINE*)MEM_VRAM)


//! Mode 4 first page as a matrix
/*!	m4_mem[y][x]	= pixel (x, y)					( u8 )
*	\note	This is a byte-buffer. Not to be used for writing.
*/
#define m4_mem			((M4LINE*)MEM_VRAM)

//! Mode 5 first page as a matrix
/*!	m5_mem[y][x]	= pixel (x, y)					( COLOR )
*/
#define m5_mem			((M5LINE*)MEM_VRAM)

//! First page array
#define vid_mem_front	((COLOR*)MEM_VRAM)

//! Second page array
#define vid_mem_back	((COLOR*)MEM_VRAM_BACK)

//! Mode 4 second page as a matrix
/*!	m4_mem[y][x]	= pixel (x, y)					( u8 )
*	\note	This is a byte-buffer. Not to be used for writing.
*/
#define m4_mem_back		((M4LINE*)MEM_VRAM_BACK)

//! Mode 5 second page as a matrix
/*!	m5_mem[y][x]	= pixel (x, y)					( COLOR )
*/
#define m5_mem_back		((M5LINE*)MEM_VRAM_BACK)

//\}	// End VRAM


//! \name OAM
//\{

//! Object attribute memory
/*!	oam_mem[i]		= object i						( OBJ_ATTR )
*/
#define oam_mem			((OBJ_ATTR*)MEM_OAM)
#define obj_mem			((OBJ_ATTR*)MEM_OAM)

//! Object affine memory
/*!	obj_aff_mem[i]		= object matrix i			( OBJ_AFFINE )	
*/
#define obj_aff_mem		((OBJ_AFFINE*)MEM_OAM)

//\}		// End OAM

//!	\name ROM
//\{

//! ROM pointer
#define rom_mem			((u16*)MEM_ROM)

//\}

//!	\name SRAM
//\{

//! SRAM pointer
#define sram_mem		((u8*)MEM_SRAM)

//\}

/*!	\}	*/


// --------------------------------------------------------------------
// REGISTER LIST
// --------------------------------------------------------------------


/*!	\addtogroup grpReg
	\ingroup grpMemmap
*/
/*!	\{	*/

//! \name IWRAM 'registers'
//\{

// 0300:7ff[y] is mirrored at 03ff:fff[y], which is why this works out:
#define REG_IFBIOS			*(vu16*)(REG_BASE-0x0008)	//!< IRQ ack for IntrWait functions
#define REG_RESET_DST		*(vu16*)(REG_BASE-0x0006)	//!< Destination for after SoftReset
#define REG_ISR_MAIN		*(fnptr*)(REG_BASE-0x0004)	//!< IRQ handler address
//\}

//! \name Display registers
//\{
#define REG_DISPCNT			*(vu32*)(REG_BASE+0x0000)	//!< Display control
#define REG_DISPSTAT		*(vu16*)(REG_BASE+0x0004)	//!< Display status
#define REG_VCOUNT			*(vu16*)(REG_BASE+0x0006)	//!< Scanline count
//\}

//! \name Background control registers
//\{
#define REG_BGCNT			((vu16*)(REG_BASE+0x0008))	//!< Bg control array

#define REG_BG0CNT			*(vu16*)(REG_BASE+0x0008)	//!< Bg0 control
#define REG_BG1CNT			*(vu16*)(REG_BASE+0x000A)	//!< Bg1 control
#define REG_BG2CNT			*(vu16*)(REG_BASE+0x000C)	//!< Bg2 control
#define REG_BG3CNT			*(vu16*)(REG_BASE+0x000E)	//!< Bg3 control
//\}

//! \name Regular background scroll registers. (write only!)
//\{
#define REG_BG_OFS			((BG_POINT*)(REG_BASE+0x0010))	//!< Bg scroll array

#define REG_BG0HOFS			*(vu16*)(REG_BASE+0x0010)		//!< Bg0 horizontal scroll
#define REG_BG0VOFS			*(vu16*)(REG_BASE+0x0012)		//!< Bg0 vertical scroll
#define REG_BG1HOFS			*(vu16*)(REG_BASE+0x0014)		//!< Bg1 horizontal scroll
#define REG_BG1VOFS			*(vu16*)(REG_BASE+0x0016)		//!< Bg1 vertical scroll
#define REG_BG2HOFS			*(vu16*)(REG_BASE+0x0018)		//!< Bg2 horizontal scroll
#define REG_BG2VOFS			*(vu16*)(REG_BASE+0x001A)		//!< Bg2 vertical scroll
#define REG_BG3HOFS			*(vu16*)(REG_BASE+0x001C)		//!< Bg3 horizontal scroll
#define REG_BG3VOFS			*(vu16*)(REG_BASE+0x001E)		//!< Bg3 vertical scroll
//\}

//! \name Affine background parameters. (write only!)
//\{
#define REG_BG_AFFINE		((BG_AFFINE*)(REG_BASE+0x0000))	//!< Bg affine array

#define REG_BG2PA			*(vs16*)(REG_BASE+0x0020)	//!< Bg2 matrix.pa
#define REG_BG2PB			*(vs16*)(REG_BASE+0x0022)	//!< Bg2 matrix.pb
#define REG_BG2PC			*(vs16*)(REG_BASE+0x0024)	//!< Bg2 matrix.pc
#define REG_BG2PD			*(vs16*)(REG_BASE+0x0026)	//!< Bg2 matrix.pd
#define REG_BG2X			*(vs32*)(REG_BASE+0x0028)	//!< Bg2 x scroll
#define REG_BG2Y			*(vs32*)(REG_BASE+0x002C)	//!< Bg2 y scroll
#define REG_BG3PA			*(vs16*)(REG_BASE+0x0030)	//!< Bg3 matrix.pa.
#define REG_BG3PB			*(vs16*)(REG_BASE+0x0032)	//!< Bg3 matrix.pb
#define REG_BG3PC			*(vs16*)(REG_BASE+0x0034)	//!< Bg3 matrix.pc
#define REG_BG3PD			*(vs16*)(REG_BASE+0x0036)	//!< Bg3 matrix.pd
#define REG_BG3X			*(vs32*)(REG_BASE+0x0038)	//!< Bg3 x scroll
#define REG_BG3Y			*(vs32*)(REG_BASE+0x003C)	//!< Bg3 y scroll
//\}

//! \name Windowing registers
//\{
#define REG_WIN0H			*(vu16*)(REG_BASE+0x0040)	//!< win0 right, left (0xLLRR)
#define REG_WIN1H			*(vu16*)(REG_BASE+0x0042)	//!< win1 right, left (0xLLRR)
#define REG_WIN0V			*(vu16*)(REG_BASE+0x0044)	//!< win0 bottom, top (0xTTBB)
#define REG_WIN1V			*(vu16*)(REG_BASE+0x0046)	//!< win1 bottom, top (0xTTBB)
#define REG_WININ			*(vu16*)(REG_BASE+0x0048)	//!< win0, win1 control
#define REG_WINOUT			*(vu16*)(REG_BASE+0x004A)	//!< winOut, winObj control
//\}

//! \name Alternate Windowing registers
//\{
#define REG_WIN0R			*(vu8*)(REG_BASE+0x0040)	//!< Win 0 right
#define REG_WIN0L			*(vu8*)(REG_BASE+0x0041)	//!< Win 0 left
#define REG_WIN1R			*(vu8*)(REG_BASE+0x0042)	//!< Win 1 right
#define REG_WIN1L			*(vu8*)(REG_BASE+0x0043)	//!< Win 1 left

#define REG_WIN0B			*(vu8*)(REG_BASE+0x0044)	//!< Win 0 bottom
#define REG_WIN0T			*(vu8*)(REG_BASE+0x0045)	//!< Win 0 top
#define REG_WIN1B			*(vu8*)(REG_BASE+0x0046)	//!< Win 1 bottom
#define REG_WIN1T			*(vu8*)(REG_BASE+0x0047)	//!< Win 1 top

#define REG_WIN0CNT			*(vu8*)(REG_BASE+0x0048)	//!< window 0 control
#define REG_WIN1CNT			*(vu8*)(REG_BASE+0x0049)	//!< window 1 control
#define REG_WINOUTCNT		*(vu8*)(REG_BASE+0x004A)	//!< Out window control
#define REG_WINOBJCNT		*(vu8*)(REG_BASE+0x004B)	//!< Obj window control
//\}


//! \name Graphic effects
//\{
#define REG_MOSAIC			*(vu32*)(REG_BASE+0x004C)	//!< Mosaic control
#define REG_BLDCNT			*(vu16*)(REG_BASE+0x0050)	//!< Alpha control
#define REG_BLDALPHA		*(vu16*)(REG_BASE+0x0052)	//!< Fade level
#define REG_BLDY			*(vu16*)(REG_BASE+0x0054)	//!< Blend levels
//\}


// === SOUND REGISTERS ===
// sound regs, partially following pin8gba's nomenclature

//! \name Channel 1: Square wave with sweep
//\{
#define REG_SND1SWEEP		*(vu16*)(REG_BASE+0x0060)	//!< Channel 1 Sweep
#define REG_SND1CNT			*(vu16*)(REG_BASE+0x0062)	//!< Channel 1 Control
#define REG_SND1FREQ		*(vu16*)(REG_BASE+0x0064)	//!< Channel 1 frequency
//\}

//! \name Channel 2: Simple square wave
//\{
#define REG_SND2CNT			*(vu16*)(REG_BASE+0x0068)	//!< Channel 2 control
#define REG_SND2FREQ		*(vu16*)(REG_BASE+0x006C)	//!< Channel 2 frequency
//\}

//! \name Channel 3: Wave player
//\{
#define REG_SND3SEL			*(vu16*)(REG_BASE+0x0070)	//!< Channel 3 wave select
#define REG_SND3CNT			*(vu16*)(REG_BASE+0x0072)	//!< Channel 3 control
#define REG_SND3FREQ		*(vu16*)(REG_BASE+0x0074)	//!< Channel 3 frequency
//\}

//! \name Channel 4: Noise generator
//\{
#define REG_SND4CNT			*(vu16*)(REG_BASE+0x0078)	//!< Channel 4 control
#define REG_SND4FREQ		*(vu16*)(REG_BASE+0x007C)	//!< Channel 4 frequency
//\}

//! \name Sound control
//\{
#define REG_SNDCNT			*(vu32*)(REG_BASE+0x0080)	//!< Main sound control
#define REG_SNDDMGCNT		*(vu16*)(REG_BASE+0x0080)	//!< DMG channel control
#define REG_SNDDSCNT		*(vu16*)(REG_BASE+0x0082)	//!< Direct Sound control
#define REG_SNDSTAT			*(vu16*)(REG_BASE+0x0084)	//!< Sound status
#define REG_SNDBIAS			*(vu16*)(REG_BASE+0x0088)	//!< Sound bias
//\}

//! \name Sound buffers
//\{
#define REG_WAVE_RAM		 (vu32*)(REG_BASE+0x0090)	//!< Channel 3 wave buffer

#define REG_WAVE_RAM0		*(vu32*)(REG_BASE+0x0090)
#define REG_WAVE_RAM1		*(vu32*)(REG_BASE+0x0094)
#define REG_WAVE_RAM2		*(vu32*)(REG_BASE+0x0098)
#define REG_WAVE_RAM3		*(vu32*)(REG_BASE+0x009C)

#define REG_FIFO_A			*(vu32*)(REG_BASE+0x00A0)	//!< DSound A FIFO
#define REG_FIFO_B			*(vu32*)(REG_BASE+0x00A4)	//!< DSound B FIFO
//\}

//! \name DMA registers
//\{
#define REG_DMA			((volatile DMA_REC*)(REG_BASE+0x00B0))	//!< DMA as DMA_REC array

#define REG_DMA0SAD			*(vu32*)(REG_BASE+0x00B0)	//!< DMA 0 Source address
#define REG_DMA0DAD			*(vu32*)(REG_BASE+0x00B4)	//!< DMA 0 Destination address
#define REG_DMA0CNT			*(vu32*)(REG_BASE+0x00B8)	//!< DMA 0 Control

#define REG_DMA1SAD			*(vu32*)(REG_BASE+0x00BC)	//!< DMA 1 Source address
#define REG_DMA1DAD			*(vu32*)(REG_BASE+0x00C0)	//!< DMA 1 Destination address
#define REG_DMA1CNT			*(vu32*)(REG_BASE+0x00C4)	//!< DMA 1 Control

#define REG_DMA2SAD			*(vu32*)(REG_BASE+0x00C8)	//!< DMA 2 Source address
#define REG_DMA2DAD			*(vu32*)(REG_BASE+0x00CC)	//!< DMA 2 Destination address
#define REG_DMA2CNT			*(vu32*)(REG_BASE+0x00D0)	//!< DMA 2 Control

#define REG_DMA3SAD			*(vu32*)(REG_BASE+0x00D4)	//!< DMA 3 Source address
#define REG_DMA3DAD			*(vu32*)(REG_BASE+0x00D8)	//!< DMA 3 Destination address
#define REG_DMA3CNT			*(vu32*)(REG_BASE+0x00DC)	//!< DMA 3 Control
//\}

//! \name Timer registers
//\{
#define REG_TM			((volatile TMR_REC*)(REG_BASE+0x0100))	//!< Timers as TMR_REC array

#define REG_TM0D			*(vu16*)(REG_BASE+0x0100)	//!< Timer 0 data
#define REG_TM0CNT			*(vu16*)(REG_BASE+0x0102)	//!< Timer 0 control
#define REG_TM1D			*(vu16*)(REG_BASE+0x0104)	//!< Timer 1 data
#define REG_TM1CNT			*(vu16*)(REG_BASE+0x0106)	//!< Timer 1 control
#define REG_TM2D			*(vu16*)(REG_BASE+0x0108)	//!< Timer 2 data
#define REG_TM2CNT			*(vu16*)(REG_BASE+0x010A)	//!< Timer 2 control
#define REG_TM3D			*(vu16*)(REG_BASE+0x010C)	//!< Timer 3 data
#define REG_TM3CNT			*(vu16*)(REG_BASE+0x010E)	//!< Timer 3 control
//\}

//! \name Serial communication
//{
#define REG_SIOCNT			*(vu16*)(REG_BASE+0x0128)	//!< Serial IO control (Normal/MP/UART)

#define REG_SIODATA			((vu32*)(REG_BASE+0x0120))
#define REG_SIODATA32		*(vu32*)(REG_BASE+0x0120)	//!< Normal/UART 32bit data
#define REG_SIODATA8		*(vu16*)(REG_BASE+0x012A)	//!< Normal/UART 8bit data

#define REG_SIOMULTI		((vu16*)(REG_BASE+0x0120))	//!< Multiplayer data array
#define REG_SIOMULTI0		*(vu16*)(REG_BASE+0x0120)	//!< MP master data
#define REG_SIOMULTI1		*(vu16*)(REG_BASE+0x0122)	//!< MP Slave 1 data
#define REG_SIOMULTI2		*(vu16*)(REG_BASE+0x0124)	//!< MP Slave 2 data 
#define REG_SIOMULTI3		*(vu16*)(REG_BASE+0x0126)	//!< MP Slave 3 data

#define REG_SIOMLT_RECV		*(vu16*)(REG_BASE+0x0120)	//!< MP data receiver
#define REG_SIOMLT_SEND		*(vu16*)(REG_BASE+0x012A)	//!< MP data sender
//\}

//! \name Keypad registers
//\{
#define REG_KEYINPUT		*(vu16*)(REG_BASE+0x0130)	//!< Key status (read only??)
#define REG_KEYCNT			*(vu16*)(REG_BASE+0x0132)	//!< Key IRQ control
//\}

//! \name Joybus communication
//\{
#define REG_RCNT			*(vu16*)(REG_BASE+0x0134)	//!< SIO Mode Select/General Purpose Data
#define REG_JOYCNT			*(vu16*)(REG_BASE+0x0140)	//!< JOY bus control
#define REG_JOY_RECV		*(vu32*)(REG_BASE+0x0150)	//!< JOY bus receiever
#define REG_JOY_TRANS		*(vu32*)(REG_BASE+0x0154)	//!< JOY bus transmitter
#define REG_JOYSTAT			*(vu16*)(REG_BASE+0x0158)	//!< JOY bus status
//\}

//! \name Interrupt / System registers
//\{
#define REG_IE				*(vu16*)(REG_BASE+0x0200)	//!< IRQ enable
#define REG_IF				*(vu16*)(REG_BASE+0x0202)	//!< IRQ status/acknowledge
#define REG_WAITCNT			*(vu16*)(REG_BASE+0x0204)	//!< Waitstate control
#define REG_IME				*(vu16*)(REG_BASE+0x0208)	//!< IRQ master enable
#define REG_PAUSE			*(vu16*)(REG_BASE+0x0300)	//!< Pause system (?)
//\}

/*!	\}	*/


// --------------------------------------------------------------------
// ALT REGISTERS
// --------------------------------------------------------------------


/*!	\addtogroup grpRegAlt
	\ingroup grpMemmap
	\brief	Alternate names for some of the registers
*/
/*!	\{	*/

#define REG_BLDMOD			*(vu16*)(REG_BASE+0x0050)	// alpha control
#define REG_COLEV			*(vu16*)(REG_BASE+0x0052)	// fade level
#define REG_COLEY			*(vu16*)(REG_BASE+0x0054)	// blend levels

// sound regs as in belogic and GBATek (mostly for compatability)
#define REG_SOUND1CNT		*(vu32*)(REG_BASE+0x0060)
#define REG_SOUND1CNT_L		*(vu16*)(REG_BASE+0x0060)
#define REG_SOUND1CNT_H		*(vu16*)(REG_BASE+0x0062)
#define REG_SOUND1CNT_X		*(vu16*)(REG_BASE+0x0064)
#define REG_SOUND2CNT_L		*(vu16*)(REG_BASE+0x0068)
#define REG_SOUND2CNT_H		*(vu16*)(REG_BASE+0x006C)
#define REG_SOUND3CNT		*(vu32*)(REG_BASE+0x0070)
#define REG_SOUND3CNT_L		*(vu16*)(REG_BASE+0x0070)
#define REG_SOUND3CNT_H		*(vu16*)(REG_BASE+0x0072)
#define REG_SOUND3CNT_X		*(vu16*)(REG_BASE+0x0074)
#define REG_SOUND4CNT_L		*(vu16*)(REG_BASE+0x0078)
#define REG_SOUND4CNT_H		*(vu16*)(REG_BASE+0x007C)
#define REG_SOUNDCNT		*(vu32*)(REG_BASE+0x0080)
#define REG_SOUNDCNT_L		*(vu16*)(REG_BASE+0x0080)
#define REG_SOUNDCNT_H		*(vu16*)(REG_BASE+0x0082)
#define REG_SOUNDCNT_X		*(vu16*)(REG_BASE+0x0084)
#define REG_SOUNDBIAS		*(vu16*)(REG_BASE+0x0088)

#define REG_WAVE			 (vu32*)(REG_BASE+0x0090)
#define REG_FIFOA			*(vu32*)(REG_BASE+0x00A0)
#define REG_FIFOB			*(vu32*)(REG_BASE+0x00A4)


#define REG_DMA0CNT_L		*(vu16*)(REG_BASE+0x00B8) // count
#define REG_DMA0CNT_H		*(vu16*)(REG_BASE+0x00BA) // flags
#define REG_DMA1CNT_L		*(vu16*)(REG_BASE+0x00C4)
#define REG_DMA1CNT_H		*(vu16*)(REG_BASE+0x00C6)
#define REG_DMA2CNT_L		*(vu16*)(REG_BASE+0x00D0)
#define REG_DMA2CNT_H		*(vu16*)(REG_BASE+0x00D2)
#define REG_DMA3CNT_L		*(vu16*)(REG_BASE+0x00DC)
#define REG_DMA3CNT_H		*(vu16*)(REG_BASE+0x00DE)

#define REG_TM0CNT_L		*(vu16*)(REG_BASE+0x0100)
#define REG_TM0CNT_H		*(vu16*)(REG_BASE+0x0102)

#define REG_TM1CNT_L		*(vu16*)(REG_BASE+0x0104)
#define REG_TM1CNT_H		*(vu16*)(REG_BASE+0x0106)

#define REG_TM2CNT_L		*(vu16*)(REG_BASE+0x0108)
#define REG_TM2CNT_H		*(vu16*)(REG_BASE+0x010a)

#define REG_TM3CNT_L		*(vu16*)(REG_BASE+0x010c)
#define REG_TM3CNT_H		*(vu16*)(REG_BASE+0x010e)


#define REG_KEYS			*(vu16*)(REG_BASE+0x0130)	// Key status
#define REG_P1				*(vu16*)(REG_BASE+0x0130)	// for backward combatibility
#define REG_P1CNT			*(vu16*)(REG_BASE+0x0132)	// ditto

#define REG_SCD0			*(vu16*)(REG_BASE+0x0120)
#define REG_SCD1			*(vu16*)(REG_BASE+0x0122)
#define REG_SCD2			*(vu16*)(REG_BASE+0x0124)
#define REG_SCD3			*(vu16*)(REG_BASE+0x0126)
#define REG_SCCNT			*(vu32*)(REG_BASE+0x0128)
#define REG_SCCNT_L			*(vu16*)(REG_BASE+0x0128)
#define REG_SCCNT_H			*(vu16*)(REG_BASE+0x012A)

#define REG_R				*(vu16*)(REG_BASE+0x0134)
#define REG_HS_CTRL			*(vu16*)(REG_BASE+0x0140)
#define REG_JOYRE			*(vu32*)(REG_BASE+0x0150)
#define REG_JOYRE_L			*(vu16*)(REG_BASE+0x0150)
#define REG_JOYRE_H			*(vu16*)(REG_BASE+0x0152)
#define REG_JOYTR			*(vu32*)(REG_BASE+0x0154)
#define REG_JOYTR_L			*(vu16*)(REG_BASE+0x0154)
#define REG_JOYTR_H			*(vu16*)(REG_BASE+0x0156)
#define REG_JSTAT			*(vu16*)(REG_BASE+0x0158)

#define REG_WSCNT			*(vu16*)(REG_BASE+0x0204)

/*!	\}	*/

#endif // TONC_MEMMAP

// EOF
