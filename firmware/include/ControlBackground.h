#ifndef CONTROLBACKGROUND_H
#define CONTROLBACKGROUND_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "Background.h"

#define NUM_BACKGROUND_CONTROL 64

#define BACKGROUND_CONTROL_BASE  0x500F5A00

// Bitmaps for tracking sprite usage
extern uint8_t backgroundControlBitmap[64 / 8];


void initBackgroundControlSystem(void);
int createBackgroundObject(BackgroundType type, uint32_t canvasId);
int controlBackgroundObject(BackgroundType type, uint32_t backgroundControl, int objectId);
int freeBackgroundObject(BackgroundType type, int objectId);


#endif // CONTROLBACKGROUND_H