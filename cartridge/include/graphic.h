#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <stdint.h>
#include "sprite_sizes.h"
#include "colors_index.h"

// System calls
uint32_t CreateControlSprite(uint32_t, uint32_t);
uint32_t ControlSprite(uint32_t, uint32_t,int);
uint32_t FreeControlSprite(uint32_t, uint32_t);
uint32_t ChangeMode(uint32_t);

uint32_t CreateSprite(uint32_t, uint8_t*, uint32_t);
uint32_t FreeSprite(uint32_t, uint32_t);
uint32_t ClearTextData();
uint32_t ShowTextToLine(const char *, uint32_t);

uint32_t CreateBackgroundCanvas(uint32_t, uint8_t*, uint32_t);
uint32_t FreeBackgroundCanvas(uint32_t, uint32_t);
uint32_t CreateBackgroundTileEntry(uint32_t, uint32_t, uint8_t*, uint32_t);

// Declaration only, no memory allocation
extern volatile char *VIDEO_MEMORY;

// Screen dimensions in characters
#define SCREEN_COLS 64
#define SCREEN_ROWS 36

typedef enum{
    GRAPHICS_MODE = 1,
    TEXT_MODE = 0
} DisplayMode;

typedef enum {
    BACKGROUND_PIXEL = 0,
    BACKGROUND_TILE = 1
} BackgroundType;

struct windowSize {
    int width;
    int height;
};

// Graphic APIs
int createObject(SpriteSize size,int palette, int x, int y, int z, int dataId);
void controlObject(SpriteSize size,int palette, int x, int y, int z, int dataId, int index);
int freeObject(SpriteSize size, int objectId);
int createCanvas(SpriteSize size, uint8_t* buffer, uint32_t bufferSize);
int freeCanvas(SpriteSize size, int canvasId);
int createBackgroundCanvas(BackgroundType type, uint8_t *buffer, uint32_t bufferSize);
int createBackgroundTileEntry(int tileIndex, int entryIndex, uint8_t *buffer, uint32_t bufferSize);
int freeBackgroundCanvas(BackgroundType type, int backgroundIndex);
void displayMode(DisplayMode mode);
struct windowSize getWindowSize();
void clearTextData();
void showTextToLine(const char* text, int line);

static uint32_t setObject(SpriteSize size, uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index);
#endif // GRAPHIC_H