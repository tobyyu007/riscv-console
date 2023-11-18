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
volatile uint32_t *smallPaletteOrange = (volatile uint32_t *)(0x500F3000);

// Canvas
volatile uint8_t *player1RectangleCanvas = (volatile uint8_t *)(0x50090000);
volatile uint8_t *player2RectangleCanvas = (volatile uint8_t *)(0x50091000);
volatile uint8_t *pingPongBallCanvas = (volatile uint8_t *)(0x500E0000);

// Objects
volatile uint32_t *player1BatObject = (volatile uint32_t *)(0x500F5B00);
volatile uint32_t *player2BatObject = (volatile uint32_t *)(0x500F5B04);
volatile uint32_t *pingPongBallObject = (volatile uint32_t *)(0x500F6300);

// Graphic setting
volatile uint32_t *GRAPHICS_MODE = (volatile uint32_t *)(0x500F6780);

// Screen Resolution
int xPosMax = 512;
int yPosMax = 288;

// Rectangle size
int rectangleHeight = 64; // Height of the rectangle
int rectangleWidth = 8;   // Width of the rectangle

// Ball size
int ballRadius = 8; // Radius of the ball
float minSpeed = 0; // Minimum speed of the ball
float maxSpeed = 0.3; // Maximum speed of the ball

uint32_t setLargeGraphicObject(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t canvasIndex);
uint32_t setSmallGraphicObject(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t canvasIndex);
void createGraphicObject();

int main()
{
    int countdown = 1;
    int last_global = 42;
    int batXOffset = 20; // Offset for the bat in the X axis
    int player1X = 0 + batXOffset;
    int player1Y = yPosMax / 2 - rectangleHeight / 2;
    int player2X = xPosMax - rectangleWidth - batXOffset;
    int player2Y = yPosMax / 2 - rectangleHeight / 2;
    float pingPongX = xPosMax / 2 - ballRadius / 2;
    float pingPongY = yPosMax / 2 - ballRadius / 2;
    *GRAPHICS_MODE = 1; // 0: text mode/ 1: graphic mode

    largePaletteWhite[1] = 0xFFFFFFFF;  // A R G B
    largePaletteOrange[1] = 0xFFF05E1C; // A R G B
    smallPaletteOrange[1] = 0xFFF05E1C; // A R G B

    createGraphicObject();

    player1BatObject[0] = setLargeGraphicObject(0, player1X, player1Y, 0, 0);                                           // Player 1
    player2BatObject[0] = setLargeGraphicObject(0, player2X, player2Y, 0, 1);                                           // Player 2
    pingPongBallObject[0] = setSmallGraphicObject(0, pingPongX, pingPongY, 0, 0); // Ping Pong Ball

    // Set random speed for the ball
    srand(42);
    float ballSpeedX = minSpeed + (rand() / (float) RAND_MAX) * (maxSpeed - minSpeed);
    float ballSpeedY = minSpeed + (rand() / (float) RAND_MAX) * (maxSpeed - minSpeed);

    while (1)
    {
        if (global != last_global)
        {   
            // Move the ball
            pingPongX += ballSpeedX;
            pingPongY += ballSpeedY;
            pingPongBallObject[0] = setSmallGraphicObject(0, pingPongX, pingPongY, 0, 0);

            if (controllerEventTriggered())
            {

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

                if (checkDirectionTrigger(ToggleButtons, DirectionUp))
                { // u button (Up)
                    if (player2Y > 0)
                    {
                        player2Y -= 1;
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
uint32_t setSmallGraphicObject(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t canvasIndex)
{
    // Look at the upper left for starting anchor
    return (((uint32_t)canvasIndex) << 24) | (((uint32_t)z) << 21) | (((uint32_t)y + 16) << 12) | (((uint32_t)x + 16) << 2) | (palette & 0x3);
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
    // Fill out Player 1 Rectangle data (large canvas)
    for (int y = 0; y < rectangleHeight; y++) // Iterate over the height of the rectangle
    {
        for (int x = 0; x < rectangleWidth; x++) // Iterate over the width of the rectangle
        {
            player1RectangleCanvas[y * 64 + x] = 1;
        }
    }

    // Fill out Player 2 Rectangle data (large canvas)
    for (int y = 0; y < rectangleHeight; y++) // Iterate over the height of the rectangle
    {
        for (int x = 0; x < rectangleWidth; x++) // Iterate over the width of the rectangle
        {
            player2RectangleCanvas[y * 64 + x] = 1;
        }
    }

    // Fill out circle data (small canvas)
    int centerX = 8; // X-coordinate of the center of the circle
    int centerY = 8; // Y-coordinate of the center of the circle

    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            int dx = x - centerX;
            int dy = y - centerY;
            int distance = dx * dx + dy * dy; // Calculate squared distance to the center

            if (distance <= ballRadius * ballRadius)
            {
                pingPongBallCanvas[y * 16 + x] = 1; // Set pixel to 1 if it's inside the circle
            }
            else
            {
                pingPongBallCanvas[y * 16 + x] = 0; // Set pixel to 0 if it's outside the circle
            }
        }
    }
}