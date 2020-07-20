//
//  Memory map defines. All of them
//
//! \file tonc_memdef.h
//! \author J Vijn
//! \date 20060508 - 20080521
//
/* === NOTES ===
  * 20080521 : comms items taken from libgba
*/

#ifndef TONC_MEMDEF
#define TONC_MEMDEF

/*! \defgroup grpMemBits	Memory map bit(fields)
	\ingroup grpMemmap
	\brief List of all bit(field) definitions of memory mapped items.
*/

// --- Prefixes ---
// REG_DISPCNT		: DCNT
// REG_DISPSTAT		: DSTAT
// REG_BGxCNT		: BG
// REG_WIN_x		: WIN
// REG_MOSAIC		: MOS
// REG_BLDCNT		: BLD
// REG_SND1SWEEP	: SSW
// REG_SNDxCNT,		: SSQR
// REG_SNDxFREQ,	: SFREQ
// REG_SNDDMGCNT	: SDMG
// REG_SNDDSCNT		: SDS
// REG_SNDSTAT		: SSTAT
// REG_DMAxCNT		: DMA
// REG_TMxCNT		: TM
// REG_SIOCNT		: SIO(N/M/U)
// REG_RCNT			: R / GPIO
// REG_KEYINPUT		: KEY
// REG_KEYCNT		: KCNT
// REG_IE, REG_IF	: IRQ
// REG_WSCNT		: WS
// Regular SE		: SE
// OAM attr 0		: ATTR0
// OAM attr 1		: ATTR1
// OAM attr 2		: ATTR2


// --- REG_DISPCNT -----------------------------------------------------

/*!	\defgroup grpVideoDCNT	Display Control Flags
	\ingroup grpMemBits
	\brief	Bits for REG_DISPCNT
*/
/*!	\{	*/

#define DCNT_MODE0				 0	//!< Mode 0; bg 0-4: reg
#define DCNT_MODE1			0x0001	//!< Mode 1; bg 0-1: reg; bg 2: affine
#define DCNT_MODE2			0x0002	//!< Mode 2; bg 2-3: affine
#define DCNT_MODE3			0x0003	//!< Mode 3; bg2: 240x160\@16 bitmap
#define DCNT_MODE4			0x0004	//!< Mode 4; bg2: 240x160\@8 bitmap
#define DCNT_MODE5			0x0005	//!< Mode 5; bg2: 160x128\@16 bitmap
#define DCNT_GB				0x0008	//!< (R) GBC indicator
#define DCNT_PAGE			0x0010	//!< Page indicator
#define DCNT_OAM_HBL		0x0020	//!< Allow OAM updates in HBlank
#define DCNT_OBJ_2D				 0	//!< OBJ-VRAM as matrix
#define DCNT_OBJ_1D			0x0040	//!< OBJ-VRAM as array
#define DCNT_BLANK			0x0080	//!< Force screen blank
#define DCNT_BG0			0x0100	//!< Enable bg 0
#define DCNT_BG1			0x0200	//!< Enable bg 1
#define DCNT_BG2			0x0400	//!< Enable bg 2
#define DCNT_BG3			0x0800	//!< Enable bg 3
#define DCNT_OBJ			0x1000	//!< Enable objects
#define DCNT_WIN0			0x2000	//!< Enable window 0
#define DCNT_WIN1			0x4000	//!< Enable window 1
#define DCNT_WINOBJ			0x8000	//!< Enable object window

#define DCNT_MODE_MASK		0x0007
#define DCNT_MODE_SHIFT			 0
#define DCNT_MODE(n)		((n)<<DCNT_MODE_SHIFT)

#define DCNT_LAYER_MASK		0x1F00
#define DCNT_LAYER_SHIFT		 8
#define DCNT_LAYER(n)		((n)<<DCNT_LAYER_SHIFT)

#define DCNT_WIN_MASK		0xE000
#define DCNT_WIN_SHIFT			13
#define DCNT_WIN(n)			((n)<<DCNT_WIN_SHIFT)

#define DCNT_BUILD(mode, layer, win, obj1d, objhbl)				\
(																\
		(((win)&7)<<13) | (((layer)&31)<<8) | (((obj1d)&1)<<6)	\
	| (((objhbl)&1)<<5) | ((mode)&7)							\
)


/*!	\}	/defgroup	*/


// --- REG_DISPSTAT ----------------------------------------------------

/*!	\defgroup grpVideoDSTAT	Display Status Flags
	\ingroup grpMemBits
	\brief	Bits for REG_DISPSTAT
*/
/*!	\{	*/

#define DSTAT_IN_VBL	0x0001	//!< Now in VBlank
#define DSTAT_IN_HBL	0x0002	//!< Now in HBlank
#define DSTAT_IN_VCT	0x0004	//!< Now in set VCount
#define DSTAT_VBL_IRQ	0x0008	//!< Enable VBlank irq
#define DSTAT_HBL_IRQ	0x0010	//!< Enable HBlank irq
#define DSTAT_VCT_IRQ	0x0020	//!< Enable VCount irq

#define DSTAT_VCT_MASK	0xFF00
#define DSTAT_VCT_SHIFT		 8
#define DSTAT_VCT(n)	((n)<<DSTAT_VCT_SHIFT)

/*!	\}	/defgroup	*/

// --- REG_BGxCNT ------------------------------------------------------

/*!	\defgroup grpVideoBGCNT	Background Control Flags
	\ingroup grpMemBits
	\brief	Bits for REG_BGxCNT
*/
/*!	\{	*/

#define BG_MOSAIC		0x0040	//!< Enable Mosaic
#define BG_4BPP				 0	//!< 4bpp (16 color) bg (no effect on affine bg)
#define BG_8BPP			0x0080	//!< 8bpp (256 color) bg (no effect on affine bg)
#define BG_WRAP			0x2000	//!< Wrap around edges of affine bgs
#define BG_SIZE0			 0
#define BG_SIZE1		0x4000
#define BG_SIZE2		0x8000
#define BG_SIZE3		0xC000
#define BG_REG_32x32		 0	//!< reg bg, 32x32 (256x256 px)
#define BG_REG_64x32	0x4000	//!< reg bg, 64x32 (512x256 px)
#define BG_REG_32x64	0x8000	//!< reg bg, 32x64 (256x512 px)
#define BG_REG_64x64	0xC000	//!< reg bg, 64x64 (512x512 px)
#define BG_AFF_16x16		 0	//!< affine bg, 16x16 (128x128 px)
#define BG_AFF_32x32	0x4000	//!< affine bg, 32x32 (256x256 px)
#define BG_AFF_64x64	0x8000	//!< affine bg, 64x64 (512x512 px)
#define BG_AFF_128x128	0xC000	//!< affine bg, 128x128 (1024x1024 px)

#define BG_PRIO_MASK	0x0003
#define BG_PRIO_SHIFT		 0
#define BG_PRIO(n)		((n)<<BG_PRIO_SHIFT)

#define BG_CBB_MASK		0x000C
#define BG_CBB_SHIFT		 2
#define BG_CBB(n)		((n)<<BG_CBB_SHIFT)

#define BG_SBB_MASK		0x1F00
#define BG_SBB_SHIFT		 8
#define BG_SBB(n)		((n)<<BG_SBB_SHIFT)

#define BG_SIZE_MASK	0xC000
#define BG_SIZE_SHIFT		14
#define BG_SIZE(n)		((n)<<BG_SIZE_SHIFT)


#define BG_BUILD(cbb, sbb, size, bpp, prio, mos, wrap)		\
(															\
	   ((size)<<14)  | (((wrap)&1)<<13) | (((sbb)&31)<<8)	\
	| (((bpp)&8)<<4) | (((mos)&1)<<6)   | (((cbb)&3)<<2)	\
	| ((prio)&3)											\
)

/*!	\}	*/


/*! \defgroup grpVideoGfx Graphic effects
	\ingroup grpMemBits
*/
/*!	\{	*/

// --- REG_WIN_x ------------------------------------------------------

//! \name Window macros
//\{

#define WIN_BG0			0x0001	//!< Windowed bg 0
#define WIN_BG1			0x0002	//!< Windowed bg 1
#define WIN_BG2			0x0004	//!< Windowed bg 2
#define WIN_BG3			0x0008	//!< Windowed bg 3
#define WIN_OBJ			0x0010	//!< Windowed objects
#define WIN_ALL			0x001F	//!< All layers in window.
#define WIN_BLD			0x0020	//!< Windowed blending

#define WIN_LAYER_MASK	0x003F
#define WIN_LAYER_SHIFT		 0
#define WIN_LAYER(n)	((n)<<WIN_LAYER_SHIFT)


#define WIN_BUILD(low, high)	\
	( ((high)<<8) | (low) )

#define WININ_BUILD(win0, win1)		WIN_BUILD(win0, win1)

#define WINOUT_BUILD(out, obj)		WIN_BUILD(out, obj)

//\}

// --- REG_MOSAIC ------------------------------------------------------

//! \name Mosaic macros
//\{

#define MOS_BH_MASK		0x000F
#define MOS_BH_SHIFT		 0
#define MOS_BH(n)		((n)<<MOS_BH_SHIFT)

#define MOS_BV_MASK		0x00F0
#define MOS_BV_SHIFT		 4
#define MOS_BV(n)		((n)<<MOS_BV_SHIFT)

#define MOS_OH_MASK		0x0F00
#define MOS_OH_SHIFT		 8
#define MOS_OH(n)		((n)<<MOS_OH_SHIFT)

#define MOS_OV_MASK		0xF000
#define MOS_OV_SHIFT		12
#define MOS_OV(n)		((n)<<MOS_OV_SHIFT)

#define MOS_BUILD(bh, bv, oh, ov)		\
	( (((ov)&15)<<12) | (((oh)&15)<<8) | (((bv)&15)<<4)| ((bh)&15) )

//\}

/*	\}	*/


// --- REG_BLDCNT ------------------------------------------------------

/*!	\defgroup grpVideoBLD	Blend Flags
	\ingroup grpMemBits
	\brief	Macros for REG_BLDCNT, REG_BLDY and REG_BLDALPHA
*/
/*!	\{	*/

//!\ name Blend control
//\{

#define BLD_BG0			0x0001	//!< Blend bg 0
#define BLD_BG1			0x0002	//!< Blend bg 1
#define BLD_BG2			0x0004	//!< Blend bg 2
#define BLD_BG3			0x0008	//!< Blend bg 3
#define BLD_OBJ			0x0010	//!< Blend objects
#define BLD_ALL			0x001F	//!< All layers (except backdrop)
#define BLD_BACKDROP	0x0020	//!< Blend backdrop
#define BLD_OFF				 0	//!< Blend mode is off
#define BLD_STD			0x0040	//!< Normal alpha blend (with REG_EV)
#define BLD_WHITE		0x0080	//!< Fade to white (with REG_Y)
#define BLD_BLACK		0x00C0	//!< Fade to black (with REG_Y)

#define BLD_TOP_MASK	0x003F
#define BLD_TOP_SHIFT		 0
#define BLD_TOP(n)		((n)<<BLD_TOP_SHIFT)

#define BLD_MODE_MASK	0x00C0
#define BLD_MODE_SHIFT		 6
#define BLD_MODE(n)		((n)<<BLD_MODE_SHIFT)

#define BLD_BOT_MASK	0x3F00
#define BLD_BOT_SHIFT		 8
#define BLD_BOT(n)		((n)<<BLD_BOT_SHIFT)

#define BLD_BUILD(top, bot, mode)		\
	( (((bot)&63)<<8) | (((mode)&3)<<6) | ((top)&63) )

//\}

// --- REG_BLDALPHA ---

//! \name Blend weights

#define BLD_EVA_MASK	0x001F
#define BLD_EVA_SHIFT		 0
#define BLD_EVA(n)		((n)<<BLD_EVA_SHIFT)

#define BLD_EVB_MASK	0x1F00
#define BLD_EVB_SHIFT		 8
#define BLD_EVB(n)		((n)<<BLD_EVB_SHIFT)

#define BLDA_BUILD(eva, evb)		\
	( ((eva)&31) | (((evb)&31)<<8) )

//\}


// --- REG_BLDY ---

//! \name Fade levels

#define BLDY_MASK		0x001F
#define BLDY_SHIFT		 0
#define BLDY(n)		((n)<<BLD_EY_SHIFT)

#define BLDY_BUILD(ey)				\
	( (ey)&31 )

//\}

/*!	\}	*/


// --- REG_SND1SWEEP ---------------------------------------------------

/*!	\defgroup grpAudioSSW	Tone Generator, Sweep Flags
	\ingroup grpMemBits
	\brief	Bits for REG_SND1SWEEP (aka REG_SOUND1CNT_L)
*/
/*!	\{	*/

#define SSW_INC			 0		//!< Increasing sweep rate
#define SSW_DEC			0x0008	//!< Decreasing sweep rate
#define SSW_OFF         0x0008	//!< Disable sweep altogether

#define SSW_SHIFT_MASK	0x0007
#define SSW_SHIFT_SHIFT		 0
#define SSW_SHIFT(n)	((n)<<SSW_SHIFT_SHIFT)

#define SSW_TIME_MASK	0x0070
#define SSW_TIME_SHIFT		 4
#define SSW_TIME(n)		((n)<<SSW_TIME_SHIFT)


#define SSW_BUILD(shift, dir, time) \
	( (((time)&7)<<4) | ((dir)<<3) | ((shift)&7) )

/*!	\}	/defgroup	*/

// --- REG_SND1CNT, REG_SND2CNT, REG_SND4CNT ---------------------------

/*!	\defgroup grpAudioSSQR	Tone Generator, Square Flags
	\ingroup grpMemBits
	\brief	Bits for REG_SND{1,2,4}CNT
	(aka REG_SOUND1CNT_H, REG_SOUND2CNT_L, REG_SOUND4CNT_L, respectively)
*/
/*!	\{	*/

#define SSQR_DUTY1_8		 0	//!< 12.5% duty cycle (#-------)
#define SSQR_DUTY1_4	0x0040	//!< 25% duty cycle (##------)
#define SSQR_DUTY1_2	0x0080	//!< 50% duty cycle (####----)
#define SSQR_DUTY3_4	0x00C0	//!< 75% duty cycle (######--) Equivalent to 25%
#define SSQR_INC			 0	//!< Increasing volume
#define SSQR_DEC		0x0800	//!< Decreasing volume

#define SSQR_LEN_MASK	0x003F
#define SSQR_LEN_SHIFT		 0
#define SSQR_LEN(n)		((n)<<SSQR_LEN_SHIFT)

#define SSQR_DUTY_MASK	0x00C0
#define SSQR_DUTY_SHIFT		 6
#define SSQR_DUTY(n)	((n)<<SSQR_DUTY_SHIFT)

#define SSQR_TIME_MASK	0x0700
#define SSQR_TIME_SHIFT		 8
#define SSQR_TIME(n)	((n)<<SSQR_TIME_SHIFT)

#define SSQR_IVOL_MASK	0xF000
#define SSQR_IVOL_SHIFT		12
#define SSQR_IVOL(n)	((n)<<SSQR_IVOL_SHIFT)


#define SSQR_ENV_BUILD(ivol, dir, time)				\
	(  ((ivol)<<12) | ((dir)<<11) | (((time)&7)<<8) )

#define SSQR_BUILD(_ivol, dir, step, duty, len)		\
	( SSQR_ENV_BUILD(ivol,dir,step) | (((duty)&3)<<6) | ((len)&63) )


/*!	\}	/defgroup	*/

// --- REG_SND1FREQ, REG_SND2FREQ, REG_SND3FREQ ------------------------

/*!	\defgroup grpAudioSFREQ	Tone Generator, Frequency Flags
	\ingroup grpMemBits
	\brief	Bits for REG_SND{1-3}FREQ 
	(aka REG_SOUND1CNT_X, REG_SOUND2CNT_H, REG_SOUND3CNT_X)
*/
/*!	\{	*/

#define SFREQ_HOLD				 0	//!< Continuous play
#define SFREQ_TIMED			0x4000	//!< Timed play
#define SFREQ_RESET			0x8000	//!< Reset sound

#define SFREQ_RATE_MASK		0x07FF
#define SFREQ_RATE_SHIFT		 0
#define SFREQ_RATE(n)		((n)<<SFREQ_RATE_SHIFT)

#define SFREQ_BUILD(rate, timed, reset)				\
	( ((rate)&0x7FF) | ((timed)<<14) | ((reset)<<15) )

/*!	\}	/defgroup	*/

// --- REG_SNDDMGCNT ---------------------------------------------------

/*!	\defgroup grpAudioSDMG	Tone Generator, Control Flags
	\ingroup grpMemBits
	\brief	Bits for REG_SNDDMGCNT (aka REG_SOUNDCNT_L)
*/
/*!	\{	*/


#define SDMG_LSQR1		0x0100	//!< Enable channel 1 on left 
#define SDMG_LSQR2		0x0200	//!< Enable channel 2 on left
#define SDMG_LWAVE		0x0400	//!< Enable channel 3 on left
#define SDMG_LNOISE		0x0800	//!< Enable channel 4 on left	
#define SDMG_RSQR1		0x1000	//!< Enable channel 1 on right
#define SDMG_RSQR2		0x2000	//!< Enable channel 2 on right
#define SDMG_RWAVE		0x4000	//!< Enable channel 3 on right
#define SDMG_RNOISE		0x8000	//!< Enable channel 4 on right

#define SDMG_LVOL_MASK	0x0007
#define SDMG_LVOL_SHIFT		 0
#define SDMG_LVOL(n)	((n)<<SDMG_LVOL_SHIFT)

#define SDMG_RVOL_MASK	0x0070
#define SDMG_RVOL_SHIFT		 4
#define SDMG_RVOL(n)	((n)<<SDMG_RVOL_SHIFT)


// Unshifted values
#define SDMG_SQR1		0x01
#define SDMG_SQR2		0x02
#define SDMG_WAVE		0x04
#define SDMG_NOISE		0x08


#define SDMG_BUILD(_lmode, _rmode, _lvol, _rvol)	\
	( ((_rmode)<<12) | ((_lmode)<<8) | (((_rvol)&7)<<4) | ((_lvol)&7) )

#define SDMG_BUILD_LR(_mode, _vol) SDMG_BUILD(_mode, _mode, _vol, _vol)

/*!	\}	/defgroup	*/

// --- REG_SNDDSCNT ----------------------------------------------------

/*!	\defgroup grpAudioSDS	Direct Sound Flags
	\ingroup grpMemBits
	\brief	Bits for REG_SNDDSCNT (aka REG_SOUNDCNT_H)
*/
/*!	\{	*/

#define SDS_DMG25			 0	//!< Tone generators at 25% volume
#define SDS_DMG50		0x0001	//!< Tone generators at 50% volume
#define SDS_DMG100		0x0002	//!< Tone generators at 100% volume
#define SDS_A50			 0	//!< Direct Sound A at 50% volume
#define SDS_A100		0x0004	//!< Direct Sound A at 100% volume
#define SDS_B50			 0	//!< Direct Sound B at 50% volume
#define SDS_B100		0x0008	//!< Direct Sound B at 100% volume
#define SDS_AR			0x0100	//!< Enable Direct Sound A on right
#define SDS_AL			0x0200	//!< Enable Direct Sound A on left
#define SDS_ATMR0			 0	//!< Direct Sound A to use timer 0
#define SDS_ATMR1		0x0400	//!< Direct Sound A to use timer 1
#define SDS_ARESET		0x0800	//!< Reset FIFO of Direct Sound A
#define SDS_BR			0x1000	//!< Enable Direct Sound B on right
#define SDS_BL			0x2000	//!< Enable Direct Sound B on left
#define SDS_BTMR0			 0	//!< Direct Sound B to use timer 0
#define SDS_BTMR1		0x4000	//!< Direct Sound B to use timer 1
#define SDS_BRESET		0x8000	//!< Reset FIFO of Direct Sound B

/*!	\}	/defgroup	*/

// --- REG_SNDSTAT -----------------------------------------------------

/*!	\defgroup grpAudioSSTAT	Sound Status Flags
	\ingroup grpMemBits
	\brief	Bits for REG_SNDSTAT (and REG_SOUNDCNT_X)
*/
/*!	\{	*/

#define SSTAT_SQR1		0x0001	//!< (R) Channel 1 status
#define SSTAT_SQR2		0x0002	//!< (R) Channel 2 status
#define SSTAT_WAVE		0x0004	//!< (R) Channel 3 status
#define SSTAT_NOISE		0x0008	//!< (R) Channel 4 status
#define SSTAT_DISABLE		 0	//!< Disable sound
#define SSTAT_ENABLE	0x0080	//!< Enable sound. NOTE: enable before using any other sound regs

/*!	\}	/defgroup	*/

// --- REG_DMAxCNT -----------------------------------------------------

/*!	\defgroup grpAudioDMA	DMA Control Flags
	\ingroup grpMemBits
	\brief	Bits for REG_DMAxCNT
*/
/*!	\{	*/

#define DMA_DST_INC				 0	//!< Incrementing destination address
#define DMA_DST_DEC		0x00200000	//!< Decrementing destination
#define DMA_DST_FIXED	0x00400000	//!< Fixed destination 
#define DMA_DST_RELOAD	0x00600000	//!< Increment destination, reset after full run
#define DMA_SRC_INC				 0	//!< Incrementing source address
#define DMA_SRC_DEC		0x00800000	//!< Decrementing source address
#define DMA_SRC_FIXED	0x01000000	//!< Fixed source address
#define DMA_REPEAT		0x02000000	//!< Repeat transfer at next start condition 
#define DMA_16					 0	//!< Transfer by halfword
#define DMA_32			0x04000000	//!< Transfer by word
#define DMA_AT_NOW				 0	//!< Start transfer now
#define DMA_GAMEPAK		0x08000000	//!< Gamepak DRQ
#define DMA_AT_VBLANK	0x10000000	//!< Start transfer at VBlank
#define DMA_AT_HBLANK	0x20000000	//!< Start transfer at HBlank
#define DMA_AT_SPECIAL	0x30000000	//!< Start copy at 'special' condition. Channel dependent
#define DMA_AT_FIFO		0x30000000	//!< Start at FIFO empty (DMA0/DMA1)
#define DMA_AT_REFRESH	0x30000000	//!< VRAM special; start at VCount=2 (DMA3)
#define DMA_IRQ			0x40000000	//!< Enable DMA irq
#define DMA_ENABLE		0x80000000	//!< Enable DMA

#define DMA_COUNT_MASK	0x0000FFFF
#define DMA_COUNT_SHIFT			 0
#define DMA_COUNT(n)	((n)<<DMA_COUNT_SHIFT)


// \name Extra 
//\{

#define DMA_NOW		(DMA_ENABLE  | DMA_AT_NOW)
#define DMA_16NOW	(DMA_NOW | DMA_16)
#define DMA_32NOW	(DMA_NOW | DMA_32)

// copies
#define DMA_CPY16	(DMA_NOW | DMA_16)
#define DMA_CPY32	(DMA_NOW | DMA_32)

// fills
#define DMA_FILL16	(DMA_NOW | DMA_SRC_FIXED | DMA_16)
#define DMA_FILL32	(DMA_NOW | DMA_SRC_FIXED | DMA_32)

#define DMA_HDMA	(DMA_ENABLE | DMA_REPEAT | DMA_AT_HBLANK | DMA_DST_RELOAD)

//\}

/*!	\}	/defgroup	*/


// --- REG_TMxCNT ------------------------------------------------------

/*!	\defgroup grpTimerTM	Timer Control Flags
	\ingroup grpMemBits
	\brief	Bits for REG_TMxCNT
*/
/*!	\{	*/

#define TM_FREQ_SYS			 0	//!< System clock timer (16.7 Mhz)
#define TM_FREQ_1			 0	//!< 1 cycle/tick (16.7 Mhz)
#define TM_FREQ_64		0x0001	//!< 64 cycles/tick (262 kHz)
#define TM_FREQ_256		0x0002	//!< 256 cycles/tick (66 kHz)
#define TM_FREQ_1024	0x0003	//!< 1024 cycles/tick (16 kHz)
#define TM_CASCADE		0x0004	//!< Increment when preceding timer overflows
#define TM_IRQ			0x0040	//!< Enable timer irq
#define TM_ENABLE		0x0080	//!< Enable timer

#define TM_FREQ_MASK	0x0003
#define TM_FREQ_SHIFT		 0
#define TM_FREQ(n)		((n)<<TM_FREQ_SHIFT)

/*!	\}	/defgroup	*/


// --- REG_SIOCNT ----------------------------------------------------------

/*!	\defgroup grpSioCnt	Serial I/O Control
	\ingroup grpMemBits
	\brief	Bits for REG_TMxCNT
*/
/*!	\{	*/

//!	\name General SIO bits.
//\{
#define SIO_MODE_8BIT	0x0000	//!< Normal comm mode, 8-bit.
#define SIO_MODE_32BIT	0x1000	//!< Normal comm mode, 32-bit.
#define SIO_MODE_MULTI	0x2000	//!< Multi-play comm mode.
#define SIO_MODE_UART	0x3000	//!< UART comm mode.

#define SIO_SI_HIGH		0x0004
#define SIO_IRQ			0x4000	//!< Enable serial irq.

#define SIO_MODE_MASK	0x3000
#define SIO_MODE_SHIFT		12
#define SIO_MODE(n)		((n)<<SIO_MODE_SHIFT)
//\}

//!	\name Normal mode bits. UNTESTED.
//\{
#define SION_CLK_EXT	0x0000	//!< Slave unit; use external clock (default).
#define SION_CLK_INT	0x0001	//!< Master unit; use internal clock.

#define SION_256KHZ		0x0000	//!< 256 kHz clockspeed (default).
#define SION_2MHZ		0x0002	//!< 2 MHz clockspeed.

#define SION_RECV_HIGH	0x0004	//!< SI high; opponent ready to receive (R).
#define SION_SEND_HIGH	0x0008	//!< SO high; ready to transfer.

#define SION_ENABLE		0x0080	//!< Start transfer/transfer enabled.
//\}

//!	\name Multiplayer mode bits. UNTESTED.
//\{
#define SIOM_9600		0x0000	//!< Baud rate,   9.6 kbps.
#define SIOM_38400		0x0001	//!< Baud rate,  38.4 kbps.
#define SIOM_57600		0x0002	//!< Baud rate,  57.6 kbps.
#define SIOM_115200		0x0003	//!< Baud rate, 115.2 kbps.

#define SIOM_SI			0x0004	//!< SI port (R).
#define SIOM_SLAVE		0x0004	//!< Not the master (R).
#define SIOM_SD			0x0008	//!< SD port (R).
#define SIOM_CONNECTED	0x0008	//!< All GBAs connected (R)

#define SIOM_ERROR		0x0040	//!< Error in transfer (R).
#define SIOM_ENABLE		0x0080	//!< Start transfer/transfer enabled.


#define SIOM_BAUD_MASK	0x0003
#define SIOM_BAUD_SHIFT		 0
#define SIOM_BAUD(n)	((n)<<SIOM_BAUD_SHIFT)

#define SIOM_ID_MASK	0x0030	//!< Multi-player ID mask (R)
#define SIOM_ID_SHIFT		 4
#define SIOM_ID(n)		((n)<<SIOM_ID_SHIFT)
//\}

//!	\name UART mode bits. UNTESTED.
//!\{
#define SIOU_9600			0x0000	//!< Baud rate,   9.6 kbps.
#define SIOU_38400			0x0001	//!< Baud rate,  38.4 kbps.
#define SIOU_57600			0x0002	//!< Baud rate,  57.6 kbps.
#define SIOU_115200			0x0003	//!< Baud rate, 115.2 kbps.

#define SIOU_CTS			0x0004	//!< CTS enable.
#define SIOU_PARITY_EVEN	0x0000	//!< Use even parity.
#define SIOU_PARITY_ODD		0x0008	//!< Use odd parity.
#define	SIOU_SEND_FULL		0x0010	//!< Send data is full (R).
#define SIOU_RECV_EMPTY		0x0020	//!< Receive data is empty (R).
#define SIOU_ERROR			0x0040	//!< Error in transfer (R).
#define SIOU_7BIT			0x0000	//!< Data is 7bits long.
#define SIOU_8BIT			0x0080	//!< Data is 8bits long.
#define SIOU_SEND			0x0100	//!< Start sending data.
#define SIOU_RECV			0x0200	//!< Start receiving data.

#define SIOU_BAUD_MASK		0x0003
#define SIOU_BAUD_SHIFT			 0
#define SIOU_BAUD(n)		((n)<<SIOU_BAUD_SHIFT)
//\}

/*!	\}	*/


/*!	\defgroup grpCommR		Comm control.
	\ingroup grpMemBits
	\brief	Communication mode select and general purpose I/O (REG_RCNT).
*/
/*!	\{	*/

//!	\name Communication mode select.
//\{
#define R_MODE_NORMAL	0x0000	//!< Normal mode.
#define R_MODE_MULTI	0x0000	//!< Multiplayer mode.
#define R_MODE_UART		0x0000	//!< UART mode.
#define R_MODE_GPIO		0x8000	//!< General purpose mode.
#define R_MODE_JOYBUS	0xC000	//!< JOY mode.

#define R_MODE_MASK		0xC000
#define R_MODE_SHIFT		14
#define R_MODE(n)		((n)<<R_MODE_SHIFT)
//\}

//!	\name General purpose I/O data
//\{
#define	GPIO_SC			0x0001	// Data
#define	GPIO_SD			0x0002
#define	GPIO_SI			0x0004
#define	GPIO_SO			0x0008
#define	GPIO_SC_IO		0x0010	// Select I/O
#define	GPIO_SD_IO		0x0020
#define	GPIO_SI_IO		0x0040
#define	GPIO_SO_IO		0x0080
#define	GPIO_SC_INPUT	0x0000	// Input setting
#define GPIO_SD_INPUT	0x0000
#define	GPIO_SI_INPUT	0x0000
#define	GPIO_SO_INPUT	0x0000
#define	GPIO_SC_OUTPUT	0x0010	// Output setting
#define	GPIO_SD_OUTPUT	0x0020
#define	GPIO_SI_OUTPUT	0x0040
#define	GPIO_SO_OUTPUT	0x0080

#define GPIO_IRQ		0x0100	//! Interrupt on SI.
//\}

/*!	\}	*/


// --- REG_KEYINPUT --------------------------------------------------------

/*!	\defgroup grpInputKEY	Key Flags
	\ingroup grpMemBits
	\brief	Bits for REG_KEYINPUT and REG_KEYCNT
*/
/*!	\{	*/

#define KEY_A			0x0001	//!< Button A
#define KEY_B			0x0002	//!< Button B
#define KEY_SELECT		0x0004	//!< Select button
#define KEY_START		0x0008	//!< Start button
#define KEY_RIGHT		0x0010	//!< Right D-pad
#define KEY_LEFT		0x0020	//!< Left D-pad
#define KEY_UP			0x0040	//!< Up D-pad
#define KEY_DOWN		0x0080	//!< Down D-pad
#define KEY_R			0x0100	//!< Shoulder R
#define KEY_L			0x0200	//!< Shoulder L

#define KEY_ACCEPT		0x0009	//!< Accept buttons: A or start
#define KEY_CANCEL		0x0002	//!< Cancel button: B (well, it usually is)
#define KEY_RESET		0x030C	//!< St+Se+L+R

#define KEY_FIRE		0x0003	//!< Fire buttons: A or B
#define KEY_SPECIAL		0x000C	//!< Special buttons: Select or Start
#define KEY_DIR			0x00F0	//!< Directions: left, right, up down
#define KEY_SHOULDER	0x0300	//!< L or R

#define KEY_ANY			0x03FF	//!< Here's the Any key :)

#define KEY_MASK		0x03FF

/*!	\}	/defgroup	*/

// --- REG_KEYCNT ------------------------------------------------------

/*!	\defgroup grpInputKCNT	Key Control Flags
	\ingroup grpMemBits
	\brief	Bits for REG_KEYCNT
*/

/*!	\{	*/

#define KCNT_IRQ		0x4000	//!< Enable key irq
#define KCNT_OR				 0	//!< Interrupt on any of selected keys
#define KCNT_AND		0x8000	//!< Interrupt on all of selected keys

/*!	\}	/defgroup	*/

// --- REG_IE, REG_IF, REG_IF_BIOS -------------------------------------

/*!	\defgroup grpIrqIRQ	Interrupt Flags
	\ingroup grpMemBits
	\brief	Bits for REG_IE, REG_IF and REG_IFBIOS
*/
/*!	\{	*/

#define IRQ_VBLANK		0x0001	//!< Catch VBlank irq
#define IRQ_HBLANK		0x0002	//!< Catch HBlank irq
#define IRQ_VCOUNT		0x0004	//!< Catch VCount irq
#define IRQ_TIMER0		0x0008	//!< Catch timer 0 irq
#define IRQ_TIMER1		0x0010	//!< Catch timer 1 irq
#define IRQ_TIMER2		0x0020	//!< Catch timer 2 irq
#define IRQ_TIMER3		0x0040	//!< Catch timer 3 irq
#define IRQ_SERIAL		0x0080	//!< Catch serial comm irq
#define IRQ_DMA0		0x0100	//!< Catch DMA 0 irq
#define IRQ_DMA1		0x0200	//!< Catch DMA 1 irq
#define IRQ_DMA2		0x0400	//!< Catch DMA 2 irq
#define IRQ_DMA3		0x0800	//!< Catch DMA 3 irq
#define IRQ_KEYPAD		0x1000	//!< Catch key irq
#define IRQ_GAMEPAK		0x2000	//!< Catch cart irq

/*!	\}	/defgroup	*/

// --- REG_WSCNT -------------------------------------------------------

/*!	\defgroup grpMiscWS	Waitstate Control Flags
	\ingroup grpMemBits
	\brief	Bits for REG_WAITCNT
*/
/*!	\{	*/

#define WS_SRAM_4			 0
#define WS_SRAM_3		0x0001
#define WS_SRAM_2		0x0002
#define WS_SRAM_8		0x0003
#define WS_ROM0_N4			 0
#define WS_ROM0_N3		0x0004
#define WS_ROM0_N2		0x0008
#define WS_ROM0_N8		0x000C
#define WS_ROM0_S2			 0
#define WS_ROM0_S1		0x0010
#define WS_ROM1_N4			 0
#define WS_ROM1_N3		0x0020
#define WS_ROM1_N2		0x0040
#define WS_ROM1_N8		0x0060
#define WS_ROM1_S4			 0
#define WS_ROM1_S1		0x0080
#define WS_ROM2_N4			 0
#define WS_ROM2_N3		0x0100
#define WS_ROM2_N2		0x0200
#define WS_ROM2_N8		0x0300
#define WS_ROM2_S8			 0
#define WS_ROM2_S1		0x0400
#define WS_PHI_OFF			 0
#define WS_PHI_4		0x0800
#define WS_PHI_2		0x1000
#define WS_PHI_1		0x1800
#define WS_PREFETCH		0x4000
#define WS_GBA				 0
#define WS_CGB			0x8000

#define WS_STANDARD		0x4317

/*!	\}	/defgroup	*/

// --- Reg screen entries ----------------------------------------------

/*!	\defgroup grpVideoSE	Screen-entry Flags
	\ingroup grpMemBits
*/
/*!	\{	*/

#define SE_HFLIP		0x0400	//!< Horizontal flip
#define SE_VFLIP		0x0800	//!< Vertical flip

#define SE_ID_MASK		0x03FF
#define SE_ID_SHIFT			 0
#define SE_ID(n)		((n)<<SE_ID_SHIFT)

#define SE_FLIP_MASK	0x0C00
#define SE_FLIP_SHIFT		10
#define SE_FLIP(n)		((n)<<SE_FLIP_SHIFT)

#define SE_PALBANK_MASK		0xF000
#define SE_PALBANK_SHIFT		12
#define SE_PALBANK(n)		((n)<<SE_PALBANK_SHIFT)


#define SE_BUILD(id, PALBANK, hflip, vflip)	\
( ((id)&0x03FF) | (((hflip)&1)<<10) | (((vflip)&1)<<11) | ((PALBANK)<<12) )


/*!	\}	/defgroup	*/

// --- OAM attribute 0 -------------------------------------------------

/*!	\defgroup grpVideoAttr0	Object Attribute 0 Flags
	\ingroup grpMemBits
*/
/*!	\{	*/

#define ATTR0_REG				 0	//!< Regular object
#define ATTR0_AFF			0x0100	//!< Affine object
#define ATTR0_HIDE			0x0200	//!< Inactive object
#define ATTR0_AFF_DBL		0x0300	//!< Double-size affine object
#define ATTR0_AFF_DBL_BIT	0x0200
#define ATTR0_BLEND			0x0400	//!< Enable blend
#define ATTR0_WINDOW		0x0800	//!< Use for object window
#define ATTR0_MOSAIC		0x1000	//!< Enable mosaic
#define ATTR0_4BPP				 0	//!< Use 4bpp (16 color) tiles
#define ATTR0_8BPP			0x2000	//!< Use 8bpp (256 color) tiles
#define ATTR0_SQUARE			 0	//!< Square shape
#define ATTR0_WIDE			0x4000	//!< Tall shape (height &gt; width)
#define ATTR0_TALL			0x8000	//!< Wide shape (height &lt; width)

#define ATTR0_Y_MASK		0x00FF
#define ATTR0_Y_SHIFT			 0
#define ATTR0_Y(n)			((n)<<ATTR0_Y_SHIFT)

#define ATTR0_MODE_MASK		0x0300
#define ATTR0_MODE_SHIFT		 8
#define ATTR0_MODE(n)		((n)<<ATTR0_MODE_SHIFT)

#define ATTR0_SHAPE_MASK	0xC000
#define ATTR0_SHAPE_SHIFT		14
#define ATTR0_SHAPE(n)		((n)<<ATTR0_SHAPE_SHIFT)


#define ATTR0_BUILD(y, shape, bpp, mode, mos, bld, win)				\
(																	\
	((y)&255) | (((mode)&3)<<8) | (((bld)&1)<<10) | (((win)&1)<<11) \
	| (((mos)&1)<<12) | (((bpp)&8)<<10)| (((shape)&3)<<14)			\
)

/*!	\}	/defgroup	*/

// --- OAM attribute 1 -------------------------------------------------

/*!	\defgroup grpVideoAttr1	Object Attribute 1 Flags
	\ingroup grpMemBits
*/
/*!	\{	*/

#define ATTR1_HFLIP			0x1000	//!< Horizontal flip (reg obj only)
#define ATTR1_VFLIP			0x2000	//!< Vertical flip (reg obj only)
// Base sizes
#define ATTR1_SIZE_8			 0
#define ATTR1_SIZE_16		0x4000
#define ATTR1_SIZE_32		0x8000
#define ATTR1_SIZE_64		0xC000
// Square sizes
#define ATTR1_SIZE_8x8			 0	//!< Size flag for  8x8 px object
#define ATTR1_SIZE_16x16	0x4000	//!< Size flag for 16x16 px object
#define ATTR1_SIZE_32x32	0x8000	//!< Size flag for 32x32 px object
#define ATTR1_SIZE_64x64	0xC000	//!< Size flag for 64x64 px object
// Tall sizes
#define ATTR1_SIZE_8x16			 0	//!< Size flag for  8x16 px object
#define ATTR1_SIZE_8x32		0x4000	//!< Size flag for  8x32 px object
#define ATTR1_SIZE_16x32	0x8000	//!< Size flag for 16x32 px object
#define ATTR1_SIZE_32x64	0xC000	//!< Size flag for 32x64 px object
// Wide sizes
#define ATTR1_SIZE_16x8			 0	//!< Size flag for 16x8 px object
#define ATTR1_SIZE_32x8		0x4000	//!< Size flag for 32x8 px object
#define ATTR1_SIZE_32x16	0x8000	//!< Size flag for 32x16 px object
#define ATTR1_SIZE_64x32	0xC000	//!< Size flag for 64x64 px object


#define ATTR1_X_MASK		0x01FF
#define ATTR1_X_SHIFT			 0
#define ATTR1_X(n)			((n)<<ATTR1_X_SHIFT)

#define ATTR1_AFF_ID_MASK	0x3E00
#define ATTR1_AFF_ID_SHIFT		 9
#define ATTR1_AFF_ID(n)		((n)<<ATTR1_AFF_ID_SHIFT)

#define ATTR1_FLIP_MASK		0x3000
#define ATTR1_FLIP_SHIFT		12
#define ATTR1_FLIP(n)		((n)<<ATTR1_FLIP_SHIFT)

#define ATTR1_SIZE_MASK		0xC000
#define ATTR1_SIZE_SHIFT		14
#define ATTR1_SIZE(n)		((n)<<ATTR1_SIZE_SHIFT)


#define ATTR1_BUILDR(x, size, hflip, vflip)	\
( ((x)&511) | (((hflip)&1)<<12) | (((vflip)&1)<<13) | (((size)&3)<<14) )

#define ATTR1_BUILDA(x, size, affid)			\
( ((x)&511) | (((affid)&31)<<9) | (((size)&3)<<14) )

/*!	\}	/defgroup	*/

// --- OAM attribute 2 -------------------------------------------------

/*!	\defgroup grpVideoAttr2	Object Attribute 2 Flags
	\ingroup grpMemBits
*/
/*!	\{	*/

#define ATTR2_ID_MASK		0x03FF
#define ATTR2_ID_SHIFT			 0
#define ATTR2_ID(n)			((n)<<ATTR2_ID_SHIFT)

#define ATTR2_PRIO_MASK		0x0C00
#define ATTR2_PRIO_SHIFT		10
#define ATTR2_PRIO(n)		((n)<<ATTR2_PRIO_SHIFT)

#define ATTR2_PALBANK_MASK	0xF000
#define ATTR2_PALBANK_SHIFT		12
#define ATTR2_PALBANK(n)	((n)<<ATTR2_PALBANK_SHIFT)


#define ATTR2_BUILD(id, pb, prio)			\
( ((id)&0x3FF) | (((pb)&15)<<12) | (((prio)&3)<<10) )

/*!	\}	//defgroup	*/


#endif // TONC_MEMDEF
