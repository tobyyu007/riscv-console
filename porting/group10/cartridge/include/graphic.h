#ifndef GRAPH_H
#define GRAPH_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sprite_sizes.h"
#include "colors_index.h"
#include "api.h"


// Screen dimensions in characters
#define SCREEN_COLS 64
#define SCREEN_ROWS 36

// system calls
int EnableTextMode(void);
int EnableGraphicsMode(void);


typedef enum{
    GRAPHICS_MODE = 1,
    TEXT_MODE = 0
} DisplayMode;

struct windowSize {
    int width;
    int height;
};

void displayMode(DisplayMode mode);
struct windowSize getWindowSize();
int createBackground1(uint8_t *buffer, uint32_t bufferSize);
int createBackground2(uint8_t *buffer, uint32_t bufferSize);
void controlBackground1(int palette, int x, int y, int z);
void controlBackground2(int palette, int x, int y, int z);
int createBatCanvas(SpriteSize size, uint8_t *buffer, uint32_t bufferSize);
int createPauseCanvas(SpriteSize size, uint8_t *buffer, uint32_t bufferSize);
int createBallCanvas(SpriteSize size, uint8_t *buffer, uint32_t bufferSize);
void controlPlayer1(SpriteSize size, int palette, int x, int y, int z);
void controlPlayer2(SpriteSize size, int palette, int x, int y, int z);
void controlPause(SpriteSize size, int palette, int x, int y, int z);
void controlBall(SpriteSize size, int palette, int x, int y, int z);

void initializePalette();
// shoe text lines
void clearTextData();
void showTextToLine(const char* text, int line);
#endif // GRAPH_H