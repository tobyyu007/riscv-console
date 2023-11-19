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

// Text Data
volatile char *VIDEO_MEMORY = (volatile char *)(0x500F4800);

// Graphic setting
volatile uint32_t *GRAPHICS_MODE = (volatile uint32_t *)(0x500F6780);

// Screen Resolution
int xPosMax = 512;
int yPosMax = 288;

// Screen dimensions in characters
#define SCREEN_COLS 64
#define SCREEN_ROWS 36

// Rectangle size
int rectangleHeight = 64; // Height of the rectangle
int rectangleWidth = 12;  // Width of the rectangle
int batXOffset = 20;      // Offset for the bat in the X axis

// Ball size
int ballRadius = 8;   // Radius of the ball
float minSpeed = 0.3; // Minimum speed of the ball
float maxSpeed = 0.5; // Maximum speed of the ball

uint32_t setLargeGraphicObject(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t canvasIndex);
uint32_t setSmallGraphicObject(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t canvasIndex);
void createGraphicObject();

bool collision(int playerTopLeftX, int playerTopLeftY, int pingPongX, int pingPongY, int rectangleWidth, int rectangleHeight, int ballRadius);
void handleCollision(float *speedX, float *speedY, float *pingPongX, int playerX);
void clearTextData();
void showTextToLine(const char* text, int line);

int main()
{
    int countdown = 1;
    int last_global = 42;
    *GRAPHICS_MODE = 1; // 0: text mode/ 1: graphic mode
    clearTextData();

    // Player 1 and 2 starting position
    int player1X = 0 + batXOffset;
    int player1Y = yPosMax / 2 - rectangleHeight / 2;
    int player2X = xPosMax - rectangleWidth - batXOffset;
    int player2Y = yPosMax / 2 - rectangleHeight / 2;

    // Ball starting position
    float pingPongX = xPosMax / 2 - ballRadius / 2;
    float pingPongY = yPosMax / 2 - ballRadius / 2;

    // Set palettes
    largePaletteWhite[1] = 0xFFFFFFFF;  // A R G B
    largePaletteOrange[1] = 0xFFF05E1C; // A R G B
    smallPaletteOrange[1] = 0xFFF05E1C; // A R G B

    createGraphicObject();

    // Show objects
    player1BatObject[0] = setLargeGraphicObject(0, player1X, player1Y, 0, 0);     // Player 1
    player2BatObject[0] = setLargeGraphicObject(0, player2X, player2Y, 0, 1);     // Player 2
    pingPongBallObject[0] = setSmallGraphicObject(0, pingPongX, pingPongY, 0, 0); // Ping Pong Ball

    // Set random speed for the ball
    srand(global);
    float ballSpeedX = minSpeed + (rand() / (float)RAND_MAX) * (maxSpeed - minSpeed);
    float ballSpeedY = minSpeed + (rand() / (float)RAND_MAX) * (maxSpeed - minSpeed);

    bool start = false;
    char *Buffer = malloc(32);

    while (1)
    {
        if (global != last_global)
        {
            // Check if the game is started
            if (start == false)
            {
                strcpy(Buffer, "Press D and J to start");
                showTextToLine(Buffer, SCREEN_ROWS/2);
                //strcpy((char *)VIDEO_MEMORY, Buffer);
                *GRAPHICS_MODE = 0; // 0: text mode/ 1: graphic mode

                // enableCMDInterrupt();
                if (checkDirectionTrigger(DirectionPad, DirectionRight) && checkDirectionTrigger(ToggleButtons, DirectionLeft))
                {
                    start = true;
                    *GRAPHICS_MODE = 1; // 0: text mode/ 1: graphic mode
                    // disableCMDInterrupt();
                    clearTextData();
                }
            }

            else
            {
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

                // Check if the ball touches the upper or lower edge of the screen
                if (pingPongY < 0 || pingPongY + ballRadius > yPosMax)
                {
                    ballSpeedY = -ballSpeedY;
                }

                // Player 1's bat check
                if (collision(player1X, player1Y, pingPongX, pingPongY, rectangleWidth, rectangleHeight, ballRadius))
                {
                    handleCollision(&ballSpeedX, &ballSpeedY, &pingPongX, player1X);
                }

                // Player 2's bat check
                if (collision(player2X, player2Y, pingPongX, pingPongY, rectangleWidth, rectangleHeight, ballRadius))
                {
                    handleCollision(&ballSpeedX, &ballSpeedY, &pingPongX, player2X);
                }

                // // Reset position if needed
                if (pingPongX <= 0 || pingPongX + ballRadius * 2 >= xPosMax)
                {
                    if(pingPongX <= 0){
                        strcpy(Buffer, "Player 2 wins!");
                        showTextToLine(Buffer, SCREEN_ROWS/2 - 1);
                        strcpy(Buffer, "Press D and J to restart");
                        showTextToLine(Buffer, SCREEN_ROWS/2 + 1);
                    }
                    else{
                        strcpy(Buffer, "Player 1 wins!");
                        showTextToLine(Buffer, SCREEN_ROWS/2 - 1);
                        strcpy(Buffer, "Press D and J to restart");
                        showTextToLine(Buffer, SCREEN_ROWS/2 + 1);
                    }

                    *GRAPHICS_MODE = 0;  // 0: text mode/ 1: graphic mode

                    //enableCMDInterrupt();
                    // if (CMDPressed())
                    // {   
                    //     clearTextData();
                    //     //disableCMDInterrupt();
                    //     srand(global);

                    //     // Randomize ball speed
                    //     ballSpeedX = minSpeed + (rand() / (float)RAND_MAX) * (maxSpeed - minSpeed);
                    //     ballSpeedY = minSpeed + (rand() / (float)RAND_MAX) * (maxSpeed - minSpeed);

                    //     // Reset ball position to the center
                    //     pingPongX = xPosMax / 2 - ballRadius / 2;
                    //     pingPongY = yPosMax / 2 - ballRadius / 2;
                    //     *GRAPHICS_MODE = 1;  // 0: text mode/ 1: graphic mode
                    // }

                    if(checkDirectionTrigger(DirectionPad, DirectionRight) && checkDirectionTrigger(ToggleButtons, DirectionLeft)){
                        clearTextData();
                        //disableCMDInterrupt();
                        srand(global);

                        // Randomize ball speed
                        ballSpeedX = minSpeed + (rand() / (float)RAND_MAX) * (maxSpeed - minSpeed);
                        ballSpeedY = minSpeed + (rand() / (float)RAND_MAX) * (maxSpeed - minSpeed);

                        // Reset ball position to the center
                        pingPongX = xPosMax / 2 - ballRadius / 2;
                        pingPongY = yPosMax / 2 - ballRadius / 2;
                        *GRAPHICS_MODE = 1;  // 0: text mode/ 1: graphic mode
                    }
                }

                // Update bat location
                pingPongX += ballSpeedX;
                pingPongY += ballSpeedY;
                pingPongBallObject[0] = setSmallGraphicObject(0, pingPongX, pingPongY, 0, 0);

                last_global = global;
            }
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

bool collision(int playerTopLeftX, int playerTopLeftY, int pingPongX, int pingPongY, int rectangleWidth, int rectangleHeight, int ballRadius)
{
    int playerCenterX = playerTopLeftX + rectangleWidth / 2;
    int playerCenterY = playerTopLeftY + rectangleHeight / 2;
    int ballCenterX = pingPongX + ballRadius;
    int ballCenterY = pingPongY + ballRadius;

    int dx = abs(ballCenterX - playerCenterX);
    int dy = abs(ballCenterY - playerCenterY);

    if (dx > (rectangleWidth / 2 + ballRadius))
    {
        return false;
    }
    if (dy > (rectangleHeight / 2 + ballRadius))
    {
        return false;
    }

    if (dx <= (rectangleWidth / 2))
    {
        return true;
    }

    if (dy <= (rectangleHeight / 2))
    {
        return true;
    }

    int cornerDistance_sq = (dx - rectangleWidth / 2) * (dx - rectangleWidth / 2) + (dy - rectangleHeight / 2) * (dy - rectangleHeight / 2);

    return (cornerDistance_sq <= (ballRadius * ballRadius));
}

void handleCollision(float *speedX, float *speedY, float *pingPongX, int playerX)
{
    *speedX = -(*speedX);
    *speedY = -(*speedY);

    // Adjust the horizontal speed
    if (*speedX < 0)
    {
        *speedX -= 0.05;
    }
    else
    {
        *speedX += 0.05;
    }

    // Adjust the ball's position to avoid multiple collisions (teleporting)
    if (*speedX > 0 && *pingPongX < playerX + rectangleWidth)
    {
        *pingPongX = batXOffset + rectangleWidth;
    }
    if (*speedX < 0 && *pingPongX + ballRadius * 2 > playerX)
    {
        *pingPongX = xPosMax - batXOffset - rectangleWidth - ballRadius * 2;
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