#include "graphic.h"
#include "colors.h"


void displayMode(DisplayMode mode)
{
    if (mode == GRAPHICS_MODE)
    {
        enableGraphicsMode();
    }
    else if (mode == TEXT_MODE)
    {
        enableTextMode();
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

// Initialize the colors to the palette
void initializePalette() {
    int numColors = sizeof(colorList) / sizeof(colorList[0]);
    if (numColors > 255) {
        numColors = 255;
    }

    // Set index 0 to transparent for all palettes
    BACKGROUND_PALETTE_0[0] = 0x00000000; // Transparent
    BACKGROUND_PALETTE_1[0] = 0x00000000; // Transparent
    BACKGROUND_PALETTE_2[0] = 0x00000000; // Transparent
    BACKGROUND_PALETTE_3[0] = 0x00000000; // Transparent
    LARGE_PALETTE_0[0] = 0x00000000; // Transparent
    LARGE_PALETTE_1[0] = 0x00000000; // Transparent
    LARGE_PALETTE_2[0] = 0x00000000; // Transparent
    LARGE_PALETTE_3[0] = 0x00000000; // Transparent
    MEDIUM_PALETTE_0[0] = 0x00000000; // Transparent
    MEDIUM_PALETTE_1[0] = 0x00000000; // Transparent
    MEDIUM_PALETTE_2[0] = 0x00000000; // Transparent
    MEDIUM_PALETTE_3[0] = 0x00000000; // Transparent
    SMALL_PALETTE_0[0] = 0x00000000; // Transparent
    SMALL_PALETTE_1[0] = 0x00000000; // Transparent
    SMALL_PALETTE_2[0] = 0x00000000; // Transparent
    SMALL_PALETTE_3[0] = 0x00000000; // Transparent

    for (int i = 1; i < numColors; i++) {
        uint32_t color = colorList[i];
        BACKGROUND_PALETTE_0[i] = (color & 0x00FFFFFF) | (0xFF << 24);  // 100% opacity
        BACKGROUND_PALETTE_1[i] = (color & 0x00FFFFFF) | (0x99 << 24);  // 60% opacity
        BACKGROUND_PALETTE_2[i] = (color & 0x00FFFFFF) | (0x4C << 24);  // 30% opacity
        BACKGROUND_PALETTE_3[i] = 0x00000000;  // 0% opacity
        LARGE_PALETTE_0[i] = (color & 0x00FFFFFF) | (0xFF << 24);  // 100% opacity
        LARGE_PALETTE_1[i] = (color & 0x00FFFFFF) | (0x99 << 24);  // 60% opacity
        LARGE_PALETTE_2[i] = (color & 0x00FFFFFF) | (0x4C << 24);  // 30% opacity
        LARGE_PALETTE_3[i] = 0x00000000;  // 0% opacity
        MEDIUM_PALETTE_0[i] = (color & 0x00FFFFFF) | (0xFF << 24);  // 100% opacity
        MEDIUM_PALETTE_1[i] = (color & 0x00FFFFFF) | (0x99 << 24);  // 60% opacity
        MEDIUM_PALETTE_2[i] = (color & 0x00FFFFFF) | (0x4C << 24);  // 30% opacity
        MEDIUM_PALETTE_3[i] = 0x00000000;  // 0% opacity
        SMALL_PALETTE_0[i] = (color & 0x00FFFFFF) | (0xFF << 24);  // 100% opacity
        SMALL_PALETTE_1[i] = (color & 0x00FFFFFF) | (0x99 << 24);  // 60% opacity
        SMALL_PALETTE_2[i] = (color & 0x00FFFFFF) | (0x4C << 24);  // 30% opacity
        SMALL_PALETTE_3[i] = 0x00000000;  // 0% opacity
    }
}

// Define and allocate memory for VIDEO_MEMORY
volatile char *VIDEO_MEMORY = (volatile char *)(0x500F4800);

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