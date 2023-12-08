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

uint32_t CreateBackgroundObject(uint32_t, uint32_t);
uint32_t ControlBackgroundObject(uint32_t, uint32_t,int);
uint32_t FreeBackgroundObject(uint32_t, uint32_t);

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
/**
 * Creates a background object with the specified parameters.
 *
 * @param type The type of background object.
 * @param palette The palette index for the object.
 * @param x The x-coordinate of the object.
 * @param y The y-coordinate of the object.
 * @param z The z-coordinate of the object.
 * @param canvasId The ID of the canvas to use for the object.
 * @return The ID of the created background object.
 */
int createBackgroundObject(BackgroundType type, int palette, int x, int y, int z, int canvasId);

/**
 * Controls the specified background object.
 *
 * @param type The type of background object.
 * @param palette The palette index for the object.
 * @param x The x-coordinate of the object.
 * @param y The y-coordinate of the object.
 * @param z The z-coordinate of the object.
 * @param canvasId The ID of the canvas used by the object.
 * @param objectId The ID of the background object to control.
 */
void controlBackgroundObject(BackgroundType type, int palette, int x, int y, int z, int canvasId, int objectId);

/**
 * Frees the specified background object.
 *
 * @param type The type of background object.
 * @param objectId The ID of the background object to free.
 * @return 0 if successful, -1 otherwise.
 */
int freeBackgroundObject(BackgroundType type, int objectId);

/**
 * Creates an object with the specified parameters.
 *
 * @param size The size of the object.
 * @param palette The palette index for the object.
 * @param x The x-coordinate of the object.
 * @param y The y-coordinate of the object.
 * @param z The z-coordinate of the object.
 * @param dataId The ID of the object's data.
 * @return The ID of the created object.
 */
int createObject(SpriteSize size, int palette, int x, int y, int z, int dataId);

/**
 * Controls the specified object.
 *
 * @param size The size of the object.
 * @param palette The palette index for the object.
 * @param x The x-coordinate of the object.
 * @param y The y-coordinate of the object.
 * @param z The z-coordinate of the object.
 * @param dataId The ID of the object's data.
 * @param index The index of the object to control.
 */
void controlObject(SpriteSize size, int palette, int x, int y, int z, int dataId, int index);

/**
 * Frees the specified object.
 *
 * @param size The size of the object.
 * @param objectId The ID of the object to free.
 * @return 0 if successful, -1 otherwise.
 */
int freeObject(SpriteSize size, int objectId);

/**
 * Creates a canvas with the specified parameters.
 *
 * @param size The size of the canvas.
 * @param buffer The buffer to use for the canvas.
 * @param bufferSize The size of the buffer.
 * @return The ID of the created canvas.
 */
int createCanvas(SpriteSize size, uint8_t* buffer, uint32_t bufferSize);

/**
 * Frees the specified canvas.
 *
 * @param size The size of the canvas.
 * @param canvasId The ID of the canvas to free.
 * @return 0 if successful, -1 otherwise.
 */
int freeCanvas(SpriteSize size, int canvasId);

/**
 * Creates a background canvas with the specified parameters.
 *
 * @param type The type of background.
 * @param buffer The buffer to use for the background canvas.
 * @param bufferSize The size of the buffer.
 * @return The ID of the created background canvas.
 */
int createBackgroundCanvas(BackgroundType type, uint8_t* buffer, uint32_t bufferSize);

/**
 * Creates a background tile entry with the specified parameters.
 *
 * @param tileIndex The index of the tile.
 * @param entryIndex The index of the entry.
 * @param buffer The buffer to use for the background tile entry.
 * @param bufferSize The size of the buffer.
 * @return The ID of the created background tile entry.
 */
int createBackgroundTileEntry(int tileIndex, int entryIndex, uint8_t* buffer, uint32_t bufferSize);

/**
 * Frees the specified background canvas.
 *
 * @param type The type of background.
 * @param backgroundIndex The index of the background canvas to free.
 * @return 0 if successful, -1 otherwise.
 */
int freeBackgroundCanvas(BackgroundType type, int backgroundIndex);

/**
 * Sets the display mode.
 *
 * @param mode The display mode to set.
 */
void displayMode(DisplayMode mode);

/**
 * Gets the window size.
 *
 * @return The window size.
 */
struct windowSize getWindowSize();

/**
 * Clears the text data.
 */
void clearTextData();

/**
 * Shows text on the specified line.
 *
 * @param text The text to show.
 * @param line The line number to show the text on.
 */
void showTextToLine(const char* text, int line);

/**
 * Sets the specified object.
 *
 * @param size The size of the object.
 * @param palette The palette index for the object.
 * @param x The x-coordinate of the object.
 * @param y The y-coordinate of the object.
 * @param z The z-coordinate of the object.
 * @param index The index of the object to set.
 * @return The ID of the set object.
 */
static uint32_t setObject(SpriteSize size, uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index);

/**
 * Sets the specified background object.
 *
 * @param type The type of background object.
 * @param palette The palette index for the object.
 * @param x The x-coordinate of the object.
 * @param y The y-coordinate of the object.
 * @param z The z-coordinate of the object.
 * @param index The index of the object to set.
 * @return The ID of the set background object.
 */
static uint32_t setBackgroundObject(BackgroundType type, uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index);

void colorInitialization();
#endif // GRAPHIC_H