#include <stdint.h>
#include "ManageSprite.h"
#include "colors.h"

volatile uint32_t *BACKGROUND_PALETTE_0 = (volatile uint32_t *)(0x500F0000);
volatile uint32_t *BACKGROUND_PALETTE_1 = (volatile uint32_t *)(0x500F0400);
volatile uint32_t *BACKGROUND_PALETTE_2 = (volatile uint32_t *)(0x500F0800);
volatile uint32_t *BACKGROUND_PALETTE_3 = (volatile uint32_t *)(0x500F0C00);
volatile uint32_t *LARGE_PALETTE_0 = (volatile uint32_t *)(0x500F1000);
volatile uint32_t *LARGE_PALETTE_1 = (volatile uint32_t *)(0x500F1400);
volatile uint32_t *LARGE_PALETTE_2 = (volatile uint32_t *)(0x500F1800);
volatile uint32_t *LARGE_PALETTE_3 = (volatile uint32_t *)(0x500F1C00);
volatile uint32_t *MEDIUM_PALETTE_0 = (volatile uint32_t *)(0x500F2000);
volatile uint32_t *MEDIUM_PALETTE_1 = (volatile uint32_t *)(0x500F2400);
volatile uint32_t *MEDIUM_PALETTE_2 = (volatile uint32_t *)(0x500F2800);
volatile uint32_t *MEDIUM_PALETTE_3 = (volatile uint32_t *)(0x500F2C00);
volatile uint32_t *SMALL_PALETTE_0 = (volatile uint32_t *)(0x500F3000);
volatile uint32_t *SMALL_PALETTE_1 = (volatile uint32_t *)(0x500F3400);
volatile uint32_t *SMALL_PALETTE_2 = (volatile uint32_t *)(0x500F3800);
volatile uint32_t *SMALL_PALETTE_3 = (volatile uint32_t *)(0x500F3C00);
// Define and allocate memory for VIDEO_MEMORY
volatile char *VIDEO_MEMORY = (volatile char *)(0x500F4800);
// Screen dimensions in characters
#define SCREEN_COLS 64
#define SCREEN_ROWS 36

uint8_t largeSpriteBitmap[NUM_LARGE_SPRITES / 8];
uint8_t mediumSpriteBitmap[NUM_MEDIUM_SPRITES / 8];
uint8_t smallSpriteBitmap[NUM_SMALL_SPRITES / 8];

// Internal functions for bitmap manipulation
static int findFreeSprite(uint8_t *bitmap, size_t size);
static void setBitmap(uint8_t *bitmap, int index);
static void clearBitmap(uint8_t *bitmap, int index);

void initSpriteSystem(void) {
    memset(largeSpriteBitmap, 0, sizeof(largeSpriteBitmap));
    memset(mediumSpriteBitmap, 0, sizeof(mediumSpriteBitmap));
    memset(smallSpriteBitmap, 0, sizeof(smallSpriteBitmap));
}

int createSprite(SpriteSize size, const uint8_t *buffer, size_t bufferSize) {
    uint8_t *bitmap;
    size_t bitmapSize;
    size_t spriteSize;
    volatile uint8_t *spriteBase;

    switch (size) {
        case LARGE_SPRITE:
            bitmap = largeSpriteBitmap;
            bitmapSize = NUM_LARGE_SPRITES;
            spriteSize = LARGE_SPRITE_SIZE;
            spriteBase = (volatile uint8_t *)LARGE_SPRITE_BASE;
            break;
        case MEDIUM_SPRITE:
            bitmap = mediumSpriteBitmap;
            bitmapSize = NUM_MEDIUM_SPRITES;
            spriteSize = MEDIUM_SPRITE_SIZE;
            spriteBase = (volatile uint8_t *)MEDIUM_SPRITE_BASE;
            break;
        case SMALL_SPRITE:
            bitmap = smallSpriteBitmap;
            bitmapSize = NUM_SMALL_SPRITES;
            spriteSize = SMALL_SPRITE_SIZE;
            spriteBase = (volatile uint8_t *)SMALL_SPRITE_BASE;
            break;
        default:
            return -1; // Invalid sprite size
    }

    if (bufferSize != spriteSize) {
        return -1; // Buffer size does not match sprite size
    }

    int index = findFreeSprite(bitmap, bitmapSize);
    if (index == -1) {
        return -1; // No free sprite slot available
    }

    memcpy((void *)(spriteBase + index * spriteSize), buffer, spriteSize);
    setBitmap(bitmap, index);
    return index;
}

int freeSprite(SpriteSize size, int spriteIndex) {
    uint8_t *bitmap;
    size_t bitmapSize;

    switch (size) {
        case LARGE_SPRITE:
            bitmap = largeSpriteBitmap;
            bitmapSize = NUM_LARGE_SPRITES;
            break;
        case MEDIUM_SPRITE:
            bitmap = mediumSpriteBitmap;
            bitmapSize = NUM_MEDIUM_SPRITES;
            break;
        case SMALL_SPRITE:
            bitmap = smallSpriteBitmap;
            bitmapSize = NUM_SMALL_SPRITES;
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
static int findFreeSprite(uint8_t *bitmap, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (!(bitmap[i / 8] & (1 << (i % 8)))) {
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

// Initialize the colors to the palette
void initializePalette() {
    size_t numColors = sizeof(colorList) / sizeof(colorList[0]);
    if (numColors > 255) {
        numColors = 255;
    }

    for (size_t i = 0; i < numColors; i++) {
        uint32_t color = colorList[i];
        BACKGROUND_PALETTE_0[i] = (color & 0x00FFFFFF) | (0xFF << 24);  // 100% opacity
        BACKGROUND_PALETTE_1[i] = (color & 0x00FFFFFF) | (0x99 << 24);  // 60% opacity
        BACKGROUND_PALETTE_2[i] = (color & 0x00FFFFFF) | (0x4C << 24);  // 30% opacity
        BACKGROUND_PALETTE_3[i] = (color & 0x00FFFFFF) | (0x00 << 24);  // 0% opacity
        LARGE_PALETTE_0[i] = (color & 0x00FFFFFF) | (0xFF << 24);  // 100% opacity
        LARGE_PALETTE_1[i] = (color & 0x00FFFFFF) | (0x99 << 24);  // 60% opacity
        LARGE_PALETTE_2[i] = (color & 0x00FFFFFF) | (0x4C << 24);  // 30% opacity
        LARGE_PALETTE_3[i] = (color & 0x00FFFFFF) | (0x00 << 24);  // 0% opacity
        MEDIUM_PALETTE_0[i] = (color & 0x00FFFFFF) | (0xFF << 24);  // 100% opacity
        MEDIUM_PALETTE_1[i] = (color & 0x00FFFFFF) | (0x99 << 24);  // 60% opacity
        MEDIUM_PALETTE_2[i] = (color & 0x00FFFFFF) | (0x4C << 24);  // 30% opacity
        MEDIUM_PALETTE_3[i] = (color & 0x00FFFFFF) | (0x00 << 24);  // 0% opacity
        SMALL_PALETTE_0[i] = (color & 0x00FFFFFF) | (0xFF << 24);  // 100% opacity
        SMALL_PALETTE_1[i] = (color & 0x00FFFFFF) | (0x99 << 24);  // 60% opacity
        SMALL_PALETTE_2[i] = (color & 0x00FFFFFF) | (0x4C << 24);  // 30% opacity
        SMALL_PALETTE_3[i] = (color & 0x00FFFFFF) | (0x00 << 24);  // 0% opacity
    }
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