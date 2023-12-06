#include <stdint.h>

#ifndef API_H
#define API_H

// retrieval system calls
uint32_t GetTicks(void);
uint32_t GetController(void);

// timer system calls
uint32_t GetTimeStart(void);
uint32_t GetTimeEnd(uint64_t start);

// graphic system calls
int EnableTextMode(void);
int EnableGraphicsMode(void);

int CreateSmallSprite();
int SetSmallPalette(uint32_t index, uint32_t color);
int SetSmallControl(uint32_t index, uint32_t palette, uint32_t x, uint32_t y,
                    uint32_t z);

int CreateMediumSprite();
int SetMediumPalette(uint32_t index, uint32_t color);
int SetMediumControl(uint32_t index, uint32_t palette, uint32_t x, uint32_t y,
                     uint32_t z);

int CreateLargeSprite();
int SetLargePalette(uint32_t index, uint32_t color);
int SetLargeControl(uint32_t index, uint32_t palette, uint32_t x, uint32_t y,
                    uint32_t z);

int SetBackgroundData(uint32_t x, uint32_t y);
int SetBackgroundPalette(uint32_t index, uint32_t color);
int SetBackgroundControl(uint32_t index, uint32_t palette, uint32_t x,
                         uint32_t y, uint32_t z);

uint32_t SystemCall(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3,
                    uint32_t arg4, uint32_t call);

#endif