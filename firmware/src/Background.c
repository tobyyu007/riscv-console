#include "Background.h"

uint8_t backgroundPixelBitmap[64 / 8]; // The length of the bitmap should not be under 1 byte
uint8_t backgroundTileBitmap[64 / 8];

static int findFreeBackground(uint8_t *bitmap, size_t size);
static void setBitmap(uint8_t *bitmap, int index);
static void clearBitmap(uint8_t *bitmap, int index);

void initBackgroundSystem(void) {
    memset(backgroundPixelBitmap, 0, sizeof(backgroundPixelBitmap));
    memset(backgroundTileBitmap, 0, sizeof(backgroundTileBitmap));
}

int createBackgroundCanvas(BackgroundType type, const uint8_t *buffer, size_t bufferSize) {
    uint8_t *bitmap;
    size_t bitmapSize;
    size_t backgroundSize;
    size_t subimageSize;
    volatile uint8_t *backgroundBase;
    int offset = 0;

    switch (type) {
        case BACKGROUND_PIXEL:
            bitmap = backgroundPixelBitmap;
            bitmapSize = NUM_BACKGROUND_PIXEL;
            backgroundSize = BACKGROUND_PIXEL_SIZE;
            subimageSize = BACKGROUND_PIXEL_SIZE;
            backgroundBase = (volatile uint8_t *)BACKGROUND_PIXEL_BASE;
            break;
        case BACKGROUND_TILE:
            bitmap = backgroundTileBitmap;
            bitmapSize = NUM_BACKGROUND_TILE;
            backgroundSize = BACKGROUND_TILE_SIZE;
            subimageSize = TILE_SUBIMAGE_SIZE;
            backgroundBase = (volatile uint8_t *)BACKGROUND_TILE_SUBIMAGES_BASE;
            offset = 4;  // Offset for tile backgrounds
            break;
        default:
            return -1; // Invalid background type
    }

    if (bufferSize != subimageSize) {
        return -1; // Buffer size does not match subimage size
    }

    int index = findFreeBackground(bitmap, bitmapSize);
    if (index == -1) {
        return -1; // No free background slot available
    }

    memcpy((void *)(backgroundBase + index * backgroundSize), buffer, backgroundSize);
    setBitmap(bitmap, index);
    return index + offset; // Adjust index for tile backgrounds
}

int createBackgroundTileEntry(int tileIndex, int entryIndex, const uint8_t *buffer, size_t bufferSize) {
    int offset = 4;  // Offset for tile backgrounds
    tileIndex -= offset; // Adjust index for tile backgrounds
    if (tileIndex < 0 || tileIndex >= NUM_BACKGROUND_TILE) {
        return -1; // Invalid tile index
    }
    if (entryIndex < 0 || entryIndex >= NUM_TILE_ENTRIES) {
        return -1; // Invalid entry index
    }
    
    // Assuming TILE_ENTRY_SIZE is the size of each tile entry
    if (bufferSize != TILE_ENTRY_SIZE) {
        return -1; // Buffer size does not match tile entry size
    }

    // Calculate the base address for the specified tile
    volatile uint8_t *tileBase = (volatile uint8_t *)(BACKGROUND_TILE_ENTRIES_BASE + tileIndex * BACKGROUND_TILE_SIZE);
    
    // Calculate the specific address for the tile entry
    volatile uint8_t *entryAddress = tileBase + entryIndex * TILE_ENTRY_SIZE;
    
    // Copy the buffer to the calculated address
    memcpy((void *)entryAddress, buffer, TILE_ENTRY_SIZE);

    return entryIndex; // Return the entry index as confirmation
}


int freeBackgroundCanvas(BackgroundType type, int backgroundIndex) {
    uint8_t *bitmap;
    size_t bitmapSize;
    int offset = 0;

    switch (type) {
        case BACKGROUND_PIXEL:
            bitmap = backgroundPixelBitmap;
            bitmapSize = NUM_BACKGROUND_PIXEL;
            break;
        case BACKGROUND_TILE:
            bitmap = backgroundTileBitmap;
            bitmapSize = NUM_BACKGROUND_TILE;
            offset = 4;  // Offset for tile backgrounds
            break;
        default:
            return -1; // Invalid background type
    }

    int adjustedIndex = backgroundIndex - offset; // Adjust index for tile backgrounds
    if (adjustedIndex >= 0 && adjustedIndex < bitmapSize) {
        clearBitmap(bitmap, adjustedIndex);
        return 1;
    }
    return -1;
}

static int findFreeBackground(uint8_t *bitmap, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (!(bitmap[i / 8] & (1 << (i % 8)))) {
            return i;
        }
    }
    return -1;
}

static void setBitmap(uint8_t *bitmap, int index) {
    bitmap[index / 8] |= (1 << (index % 8));
}

static void clearBitmap(uint8_t *bitmap, int index) {
    bitmap[index / 8] &= ~(1 << (index % 8));
}
