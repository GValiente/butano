#ifndef GBA_MODERN_H
#define GBA_MODERN_H

#ifdef __cplusplus
extern "C"
{
#endif

unsigned isqrt32(unsigned x) __attribute__((section(".iwram"), long_call));

#ifdef __cplusplus
}
#endif

#endif
