#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define BACKGROUND_PIXEL_SIZE  0x24000 // 144KiB
#define BACKGROUND_TILE_SIZE   0x12000 // 72KiB
#define TILE_ENTRY_SIZE        0x100  // 256B
#define TILE_SUBIMAGE_SIZE     0x240 // 576B


#define NUM_BACKGROUND_PIXEL 4
#define NUM_BACKGROUND_TILE  4
#define NUM_TILE_ENTRIES       256
#define NUM_TILE_SUBIMAGES     8

#define BACKGROUND_PIXEL_BASE  0x50000000
#define BACKGROUND_TILE_ENTRIES_BASE   0x50048000
#define BACKGROUND_TILE_SUBIMAGES_BASE   0x50058000


typedef enum {
    BACKGROUND_PIXEL = 0,
    BACKGROUND_TILE = 1
} BackgroundType;

typedef struct {
    int tileIndex;
    int subimageIndex;
} TileSubimageIndex;

// Bitmaps for tracking sprite usage
extern uint8_t backgroundPixelBitmap[64 / 8];
extern uint8_t backgroundTileBitmap[64 / 8];

void initBackgroundSystem(void);
int createBackgroundCanvas(BackgroundType type, const uint8_t *buffer, size_t bufferSize);
int createBackgroundTileEntry(int tileIndex, int entryIndex, const uint8_t *buffer, size_t bufferSize);
int freeBackgroundCanvas(BackgroundType type, int backgroundIndex);


#endif // BACKGROUND_H