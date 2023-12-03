#include "ControlBackground.h"

uint8_t backgroundControlBitmap[64 / 8]; // The length of the bitmap should not be under 1 byte

static int findFreeControlBackground(uint8_t *bitmap, size_t size);
static void setBitmap(uint8_t *bitmap, int index);
static void clearBitmap(uint8_t *bitmap, int index);

void initBackgroundControlSystem(void) {
    memset(backgroundControlBitmap, 0, sizeof(backgroundControlBitmap));
}

int createBackgroundObject(BackgroundType type, uint32_t canvasId) {
    uint8_t *bitmap;
    size_t bitmapSize;
    volatile uint32_t *backgroundBase;

    switch (type) {
        case BACKGROUND_PIXEL:
            bitmap = backgroundControlBitmap;
            bitmapSize = NUM_BACKGROUND_CONTROL;
            backgroundBase = (volatile uint32_t *)BACKGROUND_CONTROL_BASE;
            break;
        case BACKGROUND_TILE:
            bitmap = backgroundControlBitmap;
            bitmapSize = NUM_BACKGROUND_CONTROL;
            backgroundBase = (volatile uint32_t *)BACKGROUND_CONTROL_BASE;
            break;
        default:
            return -1; // Invalid background type
    }

    int index = findFreeControlBackground(bitmap, bitmapSize);
    if (index == -1) {
        return -1; // No free background slot available
    }

    backgroundBase[index] = canvasId;
    setBitmap(bitmap, index);
    return index;
}

int controlBackgroundObject(BackgroundType type, uint32_t backgroundControl, int objectId){
    uint8_t *bitmap;
    size_t bitmapSize;
    volatile uint32_t *backgroundBase;

    switch (type) {
        case BACKGROUND_PIXEL:
            bitmap = backgroundControlBitmap;
            bitmapSize = NUM_BACKGROUND_CONTROL;
            backgroundBase = (volatile uint32_t *)BACKGROUND_CONTROL_BASE;
            break;
        case BACKGROUND_TILE:
            bitmap = backgroundControlBitmap;
            bitmapSize = NUM_BACKGROUND_CONTROL;
            backgroundBase = (volatile uint32_t *)BACKGROUND_CONTROL_BASE;
            break;
        default:
            return -1; // Invalid background type
    }

    backgroundBase[objectId] = backgroundControl;
    return 1;
}

int freeBackgroundObject(BackgroundType type, int objectId) {
    uint8_t *bitmap;
    size_t bitmapSize;
    volatile uint32_t *backgroundBase;

    switch (type) {
        case BACKGROUND_PIXEL:
            bitmap = backgroundControlBitmap;
            bitmapSize = NUM_BACKGROUND_CONTROL;
            backgroundBase = (volatile uint32_t *)BACKGROUND_CONTROL_BASE;
            break;
        case BACKGROUND_TILE:
            bitmap = backgroundControlBitmap;
            bitmapSize = NUM_BACKGROUND_CONTROL;
            backgroundBase = (volatile uint32_t *)BACKGROUND_CONTROL_BASE;
            break;
        default:
            return -1; // Invalid background type
    }

    if (objectId >= 0 && objectId < bitmapSize) {
        clearBitmap(bitmap, objectId);
        backgroundBase[objectId] = 0;
        return 1;
    }
    return 0;
}

// Finds the first free control background in the bitmap
static int findFreeControlBackground(uint8_t *bitmap, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (!(bitmap[i] & (1 << i))) {
            return i;
        }
    }
    return -1;
}

// Sets the bit at the specified index in the bitmap
static void setBitmap(uint8_t *bitmap, int index) {
    bitmap[index / 8] |= (1 << (index % 8));
}

// Clears the bit at the specified index in the bitmap
static void clearBitmap(uint8_t *bitmap, int index) {
    bitmap[index / 8] &= ~(1 << (index % 8));
}
