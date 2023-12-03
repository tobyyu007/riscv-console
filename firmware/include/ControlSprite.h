// ControlSprite.h

#ifndef CONTROL_SPRITE_H
#define CONTROL_SPRITE_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ManageSprite.h>

// set sprite control bases
#define LARGE_SPRITE_CONTROL_SIZE  0x4  // 4B
#define MEDIUM_SPRITE_CONTROL_SIZE  0x4  // 4B
#define SMALL_SPRITE_CONTROL_SIZE  0x4  // 4B

#define NUM_LARGE_CONTROLS  256
#define NUM_MEDIUM_CONTROLS 256
#define NUM_SMALL_CONTROLS  256

#define LARGE_SPRITE_CONTROL_BASE  0x500F5B00
#define MEDIUM_SPRITE_CONTROL_BASE 0x500F5F00
#define SMALL_SPRITE_CONTROL_BASE  0x500F6300

// Bitmaps for tracking sprite usage
extern uint8_t largeSpriteControlBitmap[NUM_LARGE_CONTROLS / 8];
extern uint8_t mediumSpriteControlBitmap[NUM_MEDIUM_CONTROLS / 8];
extern uint8_t smallSpriteControlBitmap[NUM_SMALL_CONTROLS / 8];

void initSpriteControlSystem(void);
int createControlSprite(SpriteSize size, uint32_t spriteControl);
int freeControlSprite(SpriteSize size, int spriteIndex);
int controlSprite(SpriteSize size, uint32_t spriteControl, int spriteIndex);

#endif // CONTROL_SPRITE_H
