#ifndef GRAPH_H
#define GRAPH_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sprite_sizes.h"
#include "colors_index.h"
#include "api.h"


// system calls
int EnableTextMode(void);
int EnableGraphicsMode(void);

typedef enum{
    GRAPHICS_MODE = 1,
    TEXT_MODE = 0
} DisplayMode;

typedef enum {
    BACKGROUND_PIXEL = 0,
    BACKGROUND_TILE = 1
} BackgroundType;

// typedef enum {
//     SQUARE = 0, 
//     CIRCLE = 1,
//     TRIANGLE = 2,
//     PARALLELOGRAM = 3,
//     NOUGHT = 4, 
//     CROSS = 5
// } ShapeType;

struct windowSize {
    int width;
    int height;
};

void displayMode(DisplayMode mode);
// int createObject(SpriteSize size,int palette, int x, int y, int z, enum ShapeType shape);
struct windowSize getWindowSize();
#endif // GRAPH_H