#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <event.h>

volatile int global = 42;

// Palettes
volatile uint32_t *largePaletteWhite = (volatile uint32_t *)(0x500F1000);
volatile uint32_t *largePaletteOrange = (volatile uint32_t *)(0x500F1400);

// Canvas
volatile uint8_t *player1RectangleCanvas = (volatile uint8_t *)(0x50090000);
volatile uint8_t *player2RectangleCanvas = (volatile uint8_t *)(0x50091000);
volatile uint8_t *pingPongBallCanvas = (volatile uint8_t *)(0x50092000);

// Objects
volatile uint32_t *player1BatObject = (volatile uint32_t *)(0x500F5B00);
volatile uint32_t *player2BatObject = (volatile uint32_t *)(0x500F5B04);
volatile uint32_t *pingPongBallObject = (volatile uint32_t *)(0x500F5B08);

// Graphic setting
volatile uint32_t *GRAPHICS_MODE = (volatile uint32_t *)(0x500F6780);

// Screen Resolution
int xPosMax = 512;
int yPosMax = 288;

// Rectangle size
int rectangleHeight = 64; // Height of the rectangle
int rectangleWidth = 8;   // Width of the rectangle

// Ball size
int ballRadius = 16;

uint32_t setLargeGraphicObject(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t canvasIndex);
uint32_t setMediumGraphicObject(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t canvasIndex);
void createGraphicObject();

int main()
{
    int countdown = 1;
    int last_global = 42;
    int batXOffset = 20;  // Offset for the bat in the X axis
    int player1X = 0 + batXOffset;
    int player1Y = yPosMax / 2 - rectangleHeight / 2;
    int player2X = xPosMax - rectangleWidth - batXOffset;
    int player2Y = yPosMax / 2 - rectangleHeight / 2;
    int pingPongX = xPosMax/2;
    int pingPongY = yPosMax/2;
    *GRAPHICS_MODE = 1; // 0: text mode/ 1: graphic mode

    largePaletteWhite[1] = 0xFFFFFFFF;   // A R G B
    largePaletteOrange[1] = 0xFFF05E1C; // A R G B

    createGraphicObject();

    player1BatObject[0] = setLargeGraphicObject(0, player1X, player1Y, 0, 0); // Player 1
    player2BatObject[0] = setLargeGraphicObject(0, player2X, player2Y, 0, 1); // Player 2
    pingPongBallObject[0] = setLargeGraphicObject(1, xPosMax/2-ballRadius*2, yPosMax/2-ballRadius*2, 0, 2); // Ping Pong Ball

    while (1)
    {
        if (global != last_global)
        {
            if (controllerEventTriggered())
            {

                if (checkDirectionTrigger(DirectionPad, DirectionLeft))
                { // Left
                    if (player1X > 0)
                    {
                        player1X--;
                    }
                }

                if (checkDirectionTrigger(DirectionPad, DirectionUp))
                { // Up
                    if (player1Y > 0)
                    {
                        player1Y -= 1;
                    }
                }
                if (checkDirectionTrigger(DirectionPad, DirectionDown))
                { // Down
                    if (player1Y + rectangleHeight < yPosMax)
                    {
                        player1Y += 1;
                    }
                }
                if (checkDirectionTrigger(DirectionPad, DirectionRight))
                { // Right
                    if (player1X + rectangleWidth < xPosMax)
                    {
                        player1X++;
                    }
                }

                if (checkDirectionTrigger(ToggleButtons, DirectionUp))
                { // u button (Up)
                    if (player2Y > 0)
                    {
                        player2Y -= 1;
                    }
                }
                if (checkDirectionTrigger(ToggleButtons, DirectionRight))
                { // i button (Right)
                    if (player2X + rectangleWidth < xPosMax)
                    {
                        player2X++;
                    }
                }
                if (checkDirectionTrigger(ToggleButtons, DirectionLeft))
                { // j button (Left)
                    if (player2X > 0)
                    {
                        player2X--;
                    }
                }
                if (checkDirectionTrigger(ToggleButtons, DirectionDown))
                { // k button (Down)
                    if (player2Y + rectangleHeight < yPosMax)
                    {
                        player2Y += 1;
                    }
                }

                player1BatObject[0] = setLargeGraphicObject(0, player1X, player1Y, 0, 0);
                player2BatObject[0] = setLargeGraphicObject(0, player2X, player2Y, 0, 1);
            }
            last_global = global;
        }
        countdown--;
        if (!countdown)
        {
            global++;
            countdown = 10000;
        }
    }
    return 0;
}

uint32_t setLargeGraphicObject(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t canvasIndex)
{
    // Look at the upper left for starting anchor
    return (((uint32_t)canvasIndex) << 24) | (((uint32_t)z) << 21) | (((uint32_t)y + 64) << 12) | (((uint32_t)x + 64) << 2) | (palette & 0x3);
}

uint32_t setMediumGraphicObject(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t canvasIndex)
{
    // Look at the upper left for starting anchor
    return (((uint32_t)canvasIndex) << 24) | (((uint32_t)z) << 21) | (((uint32_t)y + 32) << 12) | (((uint32_t)x + 32) << 2) | (palette & 0x3);
}

extern char _heap_base[];
extern char _stack[];

char *_sbrk(int numbytes)
{
    static char *heap_ptr = NULL;
    char *base;
    if (heap_ptr == NULL)
    {
        heap_ptr = (char *)&_heap_base;
    }

    if ((heap_ptr + numbytes) <= _stack)
    {
        base = heap_ptr;
        heap_ptr += numbytes;
        return (base);
    }
    else
    {
        // erro = ENOMEM;
        return NULL;
    }
}

void createGraphicObject()
{
    // Fill out Player 1 Rectangle data
    for (int y = 0; y < rectangleHeight; y++) // Iterate over the height of the rectangle
    {
        for (int x = 0; x < rectangleWidth; x++) // Iterate over the width of the rectangle
        {
            player1RectangleCanvas[y * 64 + x] = 1;
        }
    }

    // Fill out Player 2 Rectangle data
    for (int y = 0; y < rectangleHeight; y++) // Iterate over the height of the rectangle
    {
        for (int x = 0; x < rectangleWidth; x++) // Iterate over the width of the rectangle
        {
            player2RectangleCanvas[y * 64 + x] = 1;
        }
    }

    // Fill out circle data
    int centerX = 32; // X-coordinate of the center of the circle
    int centerY = 32; // Y-coordinate of the center of the circle
    int ballRadius = 12;  // Radius of the circle

    for (int y = 0; y < 64; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            int dx = x - centerX;
            int dy = y - centerY;
            int distance = dx * dx + dy * dy; // Calculate squared distance to the center

            if (distance <= ballRadius * ballRadius)
            {
                pingPongBallCanvas[y * 64 + x] = 1; // Set pixel to 1 if it's inside the circle
            }
            else
            {
                pingPongBallCanvas[y * 64 + x] = 0; // Set pixel to 0 if it's outside the circle
            }
        }
    }
}