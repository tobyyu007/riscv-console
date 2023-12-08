#include "graphic.h"
#include "sprite_sizes.h"
#include <stdio.h>
#include <string.h>
#include "api.h"
#include "colors_index.h"

volatile uint32_t *MODE_REGISTER = (volatile uint32_t *)(MODE_CONTROL_REGISTER);
volatile char *VIDEO_MEMORY = (volatile char *)(0x500F4800);

#define SCREEN_COLS 64
#define SCREEN_ROWS 36


void displayMode(DisplayMode mode)
{
    if (mode == GRAPHICS_MODE)
    {
        setGraphicMode();
    }
    else if (mode == TEXT_MODE)
    {
        MODE_REGISTER[0] = 0;
    }
}

struct windowSize getWindowSize()
{
    struct windowSize size;
    size.width = 512;
    size.height = 288;
    return size;
}

int createBackground1(uint8_t *buffer, uint32_t bufferSize)
{
    volatile uint8_t *spriteBase = (volatile uint8_t *)0x50000000;
    memcpy((void *)(spriteBase + 0 * 0x24000), buffer, bufferSize);
    return 0;
}

int createBackground2(uint8_t *buffer, uint32_t bufferSize)
{
    volatile uint8_t *spriteBase = (volatile uint8_t *)0x50000000;
    memcpy((void *)(spriteBase + 1 * 0x24000), buffer, bufferSize);
    return 1;
}

void controlBackground1(int palette, int x, int y, int z)
{
    int index = 0;
    volatile uint32_t *BackgroundBase = (volatile uint32_t *)0x500F5A00;
    BackgroundBase[0] = (((uint32_t)0) << 31) | (((uint32_t)index) << 29) | (((uint32_t)z) << 22) | (((uint32_t)y + 288) << 12) | (((uint32_t)x + 512) << 2) | (palette & 0x3);
}

void controlBackground2(int palette, int x, int y, int z)
{
    int index = 1;
    volatile uint32_t *BackgroundBase = (volatile uint32_t *)0x500F5A00;
    BackgroundBase[1] = (((uint32_t)0) << 31) | (((uint32_t)index) << 29) | (((uint32_t)z) << 22) | (((uint32_t)y + 288) << 12) | (((uint32_t)x + 512) << 2) | (palette & 0x3);
}

int createBatCanvas(SpriteSize size, uint8_t *buffer, uint32_t bufferSize)
{
    volatile uint8_t *spriteBase = (volatile uint8_t *)0x50090000;
    memcpy((void *)(spriteBase + 0 * 0x1000), buffer, bufferSize);
    return 0;
}

int createPauseCanvas(SpriteSize size, uint8_t *buffer, uint32_t bufferSize)
{
    volatile uint8_t *spriteBase = (volatile uint8_t *)0x50090000;
    memcpy((void *)(spriteBase + 1 * 0x1000), buffer, bufferSize);
    return 1;
}

int createBallCanvas(SpriteSize size, uint8_t *buffer, uint32_t bufferSize)
{
    volatile uint8_t *spriteBase = (volatile uint8_t *)0x500E0000;
    memcpy((void *)(spriteBase + 0 * 0x1000), buffer, bufferSize);
    return 0;
}

void controlPlayer1(SpriteSize size, int palette, int x, int y, int z)
{
    int index = 0;
    volatile uint32_t *ControlBase = (volatile uint32_t *)0x500F5B00;
    ControlBase[index] = (((uint32_t)0) << 24) | (((uint32_t)z) << 21) | (((uint32_t)y + 64) << 12) | (((uint32_t)x + 64) << 2) | (palette & 0x3);
}

void controlPlayer2(SpriteSize size, int palette, int x, int y, int z)
{
    int index = 1;
    volatile uint32_t *ControlBase = (volatile uint32_t *)0x500F5B00;
    ControlBase[index] = (((uint32_t)0) << 24) | (((uint32_t)z) << 21) | (((uint32_t)y + 64) << 12) | (((uint32_t)x + 64) << 2) | (palette & 0x3);
}

void controlPause(SpriteSize size, int palette, int x, int y, int z)
{
    int index = 2;
    volatile uint32_t *ControlBase = (volatile uint32_t *)0x500F5B00;
    ControlBase[index] = (((uint32_t)1) << 24) | (((uint32_t)z) << 21) | (((uint32_t)y + 64) << 12) | (((uint32_t)x + 64) << 2) | (palette & 0x3);
}

void controlBall(SpriteSize size, int palette, int x, int y, int z)
{
    int index = 0;
    volatile uint32_t *ControlBase = (volatile uint32_t *)0x500F6300;
    ControlBase[index] = (((uint32_t)0) << 24) | (((uint32_t)z) << 21) | (((uint32_t)y + 16) << 12) | (((uint32_t)x + 16) << 2) | (palette & 0x3);
}

// getWindowSize() is a function that returns the size of the window in pixels

struct windowSize getWindowSize() {
    struct windowSize size;
    size.width = 512;
    size.height = 288;
    return size;
}


void clearTextData() {
    for (int i = 0; i < SCREEN_ROWS * SCREEN_COLS; ++i) {
        VIDEO_MEMORY[i] = ' ';  // ASCII value for space
    }
}

void showTextToLine(const char* text, int line) {
    if (line < 0 || line >= SCREEN_ROWS) {
        return; // Invalid line number
    }

    int textLen = strlen(text);
    int middleCol = (SCREEN_COLS - textLen) / 2;
    if (middleCol < 0) {
        middleCol = 0; // Avoid negative starting position
    }
    int offset = line * SCREEN_COLS + middleCol;

    for (int i = 0; i < textLen; ++i) {
        if (middleCol + i < SCREEN_COLS) {
            VIDEO_MEMORY[offset + i] = text[i];
        } else {
            break; // Avoid writing past the end of the screen
        }
    }
}

void colorInitialization() {
    int i;
    int j;
    int k;
    int numSmallParams = sizeof(smallParams) / sizeof(smallParams[0]); // Calculate the number of elements in the array
    int numLargeParams = sizeof(largeParams) / sizeof(largeParams[0]);
    int numBackgroundParams = sizeof(backgroundParams) / sizeof(backgroundParams[0]);
    for (i = 0; i < numSmallParams; i++) {
        setSmallColorPalette(smallParams[i].palette_number, smallParams[i].color, smallParams[i].entry_number);
    }
    for (j = 0; j < numLargeParams; j++) {
        setLargeColorPalette(largeParams[j].palette_number, largeParams[j].color, largeParams[j].entry_number);
    }
    for (k = 0; k < numBackgroundParams; k++) {
        setBackgroundPalette(backgroundParams[k].palette_number, backgroundParams[k].color, backgroundParams[k].entry_number);
    }
}
