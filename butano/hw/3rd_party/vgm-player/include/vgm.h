#ifndef VGM_H
#define VGM_H

#include <cstdint>

void VgmInit(void);
void VgmPlay(const uint8_t* pFile, bool isLoop);
void VgmStop(void);
bool VgmIntrVblank(void);

bool VgmActive(void);
unsigned VgmGetOffsetPlay(void);
void VgmSetOffsetPlay(unsigned offset);

void VgmPause(void);
void VgmResume(void);

#endif
