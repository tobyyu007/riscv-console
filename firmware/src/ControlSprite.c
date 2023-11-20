// ControlSprite.c

#include <ControlSprite.h>

uint32_t largeSpriteControlBitmap[NUM_LARGE_CONTROLS / 32];    // Each element is 32 bits
uint32_t mediumSpriteControlBitmap[NUM_MEDIUM_CONTROLS / 32];  // Each element is 32 bits
uint32_t smallSpriteControlBitmap[NUM_SMALL_CONTROLS / 32];    // Each element is 32 bits

// Internal functions for bitmap manipulation
static int findFreeControlSprite(uint32_t *bitmap, size_t size);
static void setBitmap(uint32_t *bitmap, int index);
static void clearBitmap(uint32_t *bitmap, int index);

void initSpriteControlSystem(void) {
    memset(largeSpriteControlBitmap, 0, sizeof(largeSpriteControlBitmap));
    memset(mediumSpriteControlBitmap, 0, sizeof(mediumSpriteControlBitmap));
    memset(smallSpriteControlBitmap, 0, sizeof(smallSpriteControlBitmap));
}

int createControlSprite(SpriteSize size, uint32_t canvasId) {
    uint32_t *bitmap;
    size_t bitmapSize;
    size_t spriteSize;
    volatile uint32_t *spriteBase;

    switch (size) {
        case LARGE_SPRITE:
            bitmap = largeSpriteControlBitmap;
            bitmapSize = NUM_LARGE_CONTROLS;
            spriteSize = LARGE_SPRITE_CONTROL_SIZE;
            spriteBase = (volatile uint32_t *)LARGE_SPRITE_CONTROL_BASE;
            break;
        case MEDIUM_SPRITE:
            bitmap = mediumSpriteControlBitmap;
            bitmapSize = NUM_MEDIUM_CONTROLS;
            spriteSize = MEDIUM_SPRITE_CONTROL_SIZE;
            spriteBase = (volatile uint32_t *)MEDIUM_SPRITE_CONTROL_BASE;
            break;
        case SMALL_SPRITE:
            bitmap = smallSpriteControlBitmap;
            bitmapSize = NUM_SMALL_CONTROLS;
            spriteSize = SMALL_SPRITE_CONTROL_SIZE;
            spriteBase = (volatile uint32_t *)SMALL_SPRITE_CONTROL_BASE;
            break;
        default:
            return -1; // Invalid sprite size
    }

    int index = findFreeControlSprite(bitmap, bitmapSize);
    if (index == -1) {
        return -1; // No free sprite slot available
    }

    // Copy the uint32_t spriteControl to the spriteBase
    spriteBase[index] = canvasId;

    setBitmap(bitmap, index);

    return index;
}

int controlSprite(SpriteSize size, uint32_t spriteControl, int objectId){
    uint32_t *bitmap;
    size_t bitmapSize;
    size_t spriteSize;
    volatile uint32_t *spriteBase;
      switch (size) {
        case LARGE_SPRITE:
            bitmap = largeSpriteControlBitmap;
            bitmapSize = NUM_LARGE_CONTROLS;
            spriteSize = LARGE_SPRITE_CONTROL_SIZE;
            spriteBase = (volatile uint32_t *)LARGE_SPRITE_CONTROL_BASE;
            break;
        case MEDIUM_SPRITE:
            bitmap = mediumSpriteControlBitmap;
            bitmapSize = NUM_MEDIUM_CONTROLS;
            spriteSize = MEDIUM_SPRITE_CONTROL_SIZE;
            spriteBase = (volatile uint32_t *)MEDIUM_SPRITE_CONTROL_BASE;
            break;
        case SMALL_SPRITE:
            bitmap = smallSpriteControlBitmap;
            bitmapSize = NUM_SMALL_CONTROLS;
            spriteSize = SMALL_SPRITE_CONTROL_SIZE;
            spriteBase = (volatile uint32_t *)SMALL_SPRITE_CONTROL_BASE;
            break;
        default:
            return -1; // Invalid sprite size
    }
     spriteBase[objectId] = spriteControl;
     return 1;
}

int freeControlSprite(SpriteSize size, int spriteIndex) {
    uint32_t *bitmap;
    size_t bitmapSize;

    switch (size) {
        case LARGE_SPRITE:
            bitmap = largeSpriteControlBitmap;
            bitmapSize = NUM_LARGE_CONTROLS;
            break;
        case MEDIUM_SPRITE:
            bitmap = mediumSpriteControlBitmap;
            bitmapSize = NUM_MEDIUM_CONTROLS;
            break;
        case SMALL_SPRITE:
            bitmap = smallSpriteControlBitmap;
            bitmapSize = NUM_SMALL_CONTROLS;
            break;
        default:
            return -1; // Invalid sprite size
    }

    if (spriteIndex >= 0 && spriteIndex < bitmapSize) {
        clearBitmap(bitmap, spriteIndex);
    }
    return 1;
}

// Finds the first free sprite slot in the bitmap
static int findFreeControlSprite(uint32_t *bitmap, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (!(bitmap[i] & (1 << i))) {
            return i;
        }
    }
    return -1;
}

// Sets the bit at the specified index in the bitmap
static void setBitmap(uint32_t *bitmap, int index) {
    bitmap[index] |= (1 << index);
}

// Clears the bit at the specified index in the bitmap
static void clearBitmap(uint32_t *bitmap, int index) {
    bitmap[index] &= ~(1 << index);
}
