//
//  tonc_asminc.h : header file with goodies for assembly.
//
//! \file tonc_asminc.h
//! \author J Vijn
//! \date 20081019 - 20120519
//
/* === NOTES ===
  * Cleaned up the macros so that they work with comma-directives as well.
  * For use in assembly only!
*/

#ifndef TONC_ASMINC_H
#define TONC_ASMINC_H

#if !__ASSEMBLER__
	#error This header file is for use in assembly only!
#endif	// /asm only


// --------------------------------------------------------------------
// MACROS 
// --------------------------------------------------------------------

#define DEF_SIZE(_name)		.size	_name, .-_name

//! \name Section definitions for assembly.
//\{

#define CSEC_TEXT	.text								//!< Standard code section directive.
#define CSEC_EWRAM	.ewram	//!< EWRAM code section directive.
#define CSEC_IWRAM	.iwram	//!< IWRAM code section directive.

#define DSEC_DATA	.data						//<! Standard data section directive.
#define DSEC_ROM	.section .rodata			//!< ROM data section directive.
#define DSEC_BSS	.section .bss				//!< Uninited data (RAM) section directive.
#define DSEC_SBSS	.section .sbss				//!< Uninited data (DTCM?) section directive.

#define ARM_FUNC	.arm						//!< Indicates an ARM function.
#define THUMB_FUNC	.thumb_func					//!< Indicates a Thumb function.

//! \name Function definition macros.
//\{

//! Start an assembly function.
/*!	
	\param _name	Name of function.
	\param _section	Section to place function in (like .text)
*/
#define BEGIN_FUNC(_name, _section, _iset)	\
	.section _section._name , "ax", %progbits;				\
	_iset;									\
	.align 2;								\
	.global _name;							\
	.type _name STT_FUNC;					\
_name:

//! End of a function.
#define END_FUNC(_name)		DEF_SIZE(_name)

//! Begin an ARM function
/*!	
	\param _name	Name of function.
	\param _section	Section to place function in (like .text)
*/
#define BEGIN_FUNC_ARM(_name, _section)		BEGIN_FUNC(_name, _section, ARM_FUNC)

//! Begin a THUMB function.
/*!	
	\param _name	Name of function.
	\param _section	Section to place function in (like .text)
*/
#define BEGIN_FUNC_THUMB(_name, _section)	BEGIN_FUNC(_name, _section, THUMB_FUNC)
//\}

//! \name Data definition macros.
//\{
#define BEGIN_SYMBOL(_name, _section)			\
	_section;									\
	.align;										\
	.global _name;								\
_name:

#define END_SYMBOL(_name)	DEF_SIZE(_name)
//\}

// --------------------------------------------------------------------
// CONSTANTS
// --------------------------------------------------------------------

//! \name TSurface member offsets.
//\{
#define TSRF_data			 0
#define TSRF_pitch			 4
#define TSRF_width			 8
#define TSRF_height			10
#define TSRF_bpp			12
#define TSRF_type			13
#define TSRF_palSize		14
#define TSRF_pal			16
//\}


// --------------------------------------------------------------------
// GLOBALS 
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// PROTOTYPES 
// --------------------------------------------------------------------
// --------------------------------------------------------------------
// INLINES 
// --------------------------------------------------------------------

#endif	// /TONC_ASMINC_H

// EOF
