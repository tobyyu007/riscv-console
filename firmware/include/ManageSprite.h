#ifndef MANAGE_SPRITE_H
#define MANAGE_SPRITE_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define LARGE_SPRITE_SIZE  0x1000 // 4KiB
#define MEDIUM_SPRITE_SIZE 0x400  // 1KiB
#define SMALL_SPRITE_SIZE  0x100  // 256B

#define NUM_LARGE_SPRITES  64
#define NUM_MEDIUM_SPRITES 64
#define NUM_SMALL_SPRITES  256

#define LARGE_SPRITE_BASE  0x50090000
#define MEDIUM_SPRITE_BASE 0x500D0000
#define SMALL_SPRITE_BASE  0x500E0000

typedef enum {
    LARGE_SPRITE = 0,
    MEDIUM_SPRITE = 1,
    SMALL_SPRITE = 2
} SpriteSize;

// Bitmaps for tracking sprite usage
extern uint8_t largeSpriteBitmap[NUM_LARGE_SPRITES / 8];
extern uint8_t mediumSpriteBitmap[NUM_MEDIUM_SPRITES / 8];
extern uint8_t smallSpriteBitmap[NUM_SMALL_SPRITES / 8];

void initSpriteSystem(void);
int createSprite(SpriteSize size, const uint8_t *buffer, size_t bufferSize);
int freeSprite(SpriteSize size, int spriteIndex);

#endif // MANAGE_SPRITE_H