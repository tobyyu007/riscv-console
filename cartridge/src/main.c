#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "event.h"
#include "graphic.h"
#include "memory.h"
#include "timer.h"

// Canvas
uint8_t batCanvas[LARGE_SPRITE_SIZE * LARGE_SPRITE_SIZE];
uint8_t ballCanvas[SMALL_SPRITE_SIZE * SMALL_SPRITE_SIZE];

// Background Canvas
uint8_t backgroundPixel[BACKGROUND_PIXEL_SIZE];
uint8_t backgroundTile[BACKGROUND_TILE_SIZE];
uint8_t backgroundTileEntry0[TILE_ENTRY_SIZE];
uint8_t backgroundTileEntry1[TILE_ENTRY_SIZE];
uint8_t backgroundTileEntry2[TILE_ENTRY_SIZE];


// Screen Resolution
int xPosMax = 0;
int yPosMax = 0;



// Rectangle size
int rectangleHeight = 60; // Height of the rectangle (must be 6's multiple)
int rectangleWidth = 12;  // Width of the rectangle
int batXOffset = 20;      // Offset for the bat in the X axis

// Ball size
int ballRadius = 8;      // Radius of the ball
float minSpeedX = 0.5;   // Minimum X axis speed of the ball
float maxSpeedX = 0.7;   // Maximum X axis speed of the ball
float minSpeedY = -1.0; // Minimum Y axis speed of the ball
float maxSpeedY = 1.0;  // Maximum Y axis speed of the ball

// Player 1 and 2 position
int player1X = 0;
int player1Y = 0;
int player2X = 0;
int player2Y = 0;

// Ball position
float pingPongX = 0;
float pingPongY = 0;

// Ball moving speed
float ballSpeedX = 0;
float ballSpeedY = 0;

// Game timer
int timeStart = 0;
int timeEnd = 0;

int last_global = 0;
int global = 0;

void fillCanvas();

bool collision(int playerTopLeftX, int playerTopLeftY, int pingPongX, int pingPongY, int rectangleWidth, int rectangleHeight, int ballRadius);
void handleCollision(float *speedX, float *speedY, float *pingPongX, float *pingPongY, int batX, int batY);
bool checkCollision(int playerTopLeftX, int playerTopLeftY, int pingPongX, int pingPongY, int rectangleWidth, int rectangleHeight, int ballRadius);
void clearTextData();
void showTextToLine(const char* text, int line);
void initGame();

// Background Control (SHOULD BE IN FIRMWARE)
volatile uint32_t *BACKGROUND_CONTROL = (volatile uint32_t *)(0x500F5A00);
uint32_t BackgroundControl(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index, uint8_t mode);

int main()
{
    int countdown = 1;
    global = getCurrentTime();
    displayMode(GRAPHICS_MODE);

    struct windowSize screenResolution = getWindowSize();

    xPosMax = screenResolution.width;
    yPosMax = screenResolution.height;
    // Initialize game
    initGame();

    // fill in Canvas bufffer
    fillCanvas();

    // create background canvas
    uint32_t backgroundPixelCanvas0 = createBackgroundCanvas(BACKGROUND_PIXEL, backgroundPixel, BACKGROUND_PIXEL_SIZE);
    uint32_t backgroundSubimageCanvas0 = createBackgroundCanvas(BACKGROUND_TILE, backgroundTile, BACKGROUND_TILE_SIZE);
    uint32_t entry0 = createBackgroundTileEntry(backgroundSubimageCanvas0, 0, backgroundTileEntry0, TILE_ENTRY_SIZE);
    uint32_t entry1 = createBackgroundTileEntry(backgroundSubimageCanvas0, 1, backgroundTileEntry1, TILE_ENTRY_SIZE);
    uint32_t entry2 = createBackgroundTileEntry(backgroundSubimageCanvas0, 2, backgroundTileEntry2, TILE_ENTRY_SIZE);
    // create canvas
    uint32_t batCanvasID = createCanvas(LARGE_SPRITE, batCanvas, LARGE_SPRITE_SIZE * LARGE_SPRITE_SIZE);
    uint32_t ballCanvasID = createCanvas(SMALL_SPRITE, ballCanvas, SMALL_SPRITE_SIZE * SMALL_SPRITE_SIZE);

    
    
    // create background object (SHOULD BE IN FIRMWARE)
    BACKGROUND_CONTROL[0] = BackgroundControl(0, 0, 0, 0, backgroundSubimageCanvas0, BACKGROUND_TILE);

    // create object
    uint32_t player1BatObjectID = createObject(LARGE_SPRITE, FULLY_OPAQUE, player1X, player1Y, 0, batCanvasID);
    uint32_t player2BatObjectID = createObject(LARGE_SPRITE, FULLY_OPAQUE, player2X, player2Y, 0, batCanvasID);
    uint32_t ballObjectID = createObject(SMALL_SPRITE, FULLY_OPAQUE, pingPongX, pingPongY, 0, ballCanvasID);



    bool start = false;
    char *Buffer = AllocateMemory(32);

    while (1)
    {
        if (global != last_global)
        {
            // Check if the game is started
            if (start == false)
            {
                strcpy(Buffer, "Press D and J to start");
                showTextToLine(Buffer, SCREEN_ROWS / 2);
                displayMode(TEXT_MODE); // 0: text mode/ 1: graphic mode

                if (checkDirectionTrigger(DirectionPad, DirectionRight) && checkDirectionTrigger(ToggleButtons, DirectionLeft))
                {
                    start = true;
                    displayMode(GRAPHICS_MODE);
                    clearTextData();
                    StartTimer();
                }
            }

            else  // Game is started
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
                    // control players
                    controlObject(LARGE_SPRITE, FULLY_OPAQUE, player1X, player1Y, 0, batCanvasID, player1BatObjectID);
                    controlObject(LARGE_SPRITE, FULLY_OPAQUE, player2X, player2Y, 0, batCanvasID, player2BatObjectID);
                }

                // Check if the ball touches the upper or lower edge of the screen
                if (pingPongY < 0 || pingPongY + ballRadius > yPosMax)
                {
                    ballSpeedY = -ballSpeedY;
                }

                // Player 1's bat check
                if (checkCollision(player1X, player1Y, pingPongX, pingPongY, rectangleWidth, rectangleHeight, ballRadius))
                {
                    handleCollision(&ballSpeedX, &ballSpeedY, &pingPongX, &pingPongY, player1X, player1Y);
                }

                // Player 2's bat check
                if (checkCollision(player2X, player2Y, pingPongX, pingPongY, rectangleWidth, rectangleHeight, ballRadius))
                {
                    handleCollision(&ballSpeedX, &ballSpeedY, &pingPongX, &pingPongY, player2X, player2Y);
                }

                // Reset position if needed
                if (pingPongX <= 0 || pingPongX + ballRadius * 2 >= xPosMax)
                {
                    if (pingPongX <= 0)
                    {
                        strcpy(Buffer, "Player 2 wins!");
                        showTextToLine(Buffer, SCREEN_ROWS / 2 - 1);
                        strcpy(Buffer, "Press D and J to restart");
                        showTextToLine(Buffer, SCREEN_ROWS / 2 + 2);
                    }
                    else
                    {
                        strcpy(Buffer, "Player 1 wins!");
                        showTextToLine(Buffer, SCREEN_ROWS / 2 - 1);
                        strcpy(Buffer, "Press D and J to restart");
                        showTextToLine(Buffer, SCREEN_ROWS / 2 + 2);
                    }
                    if(timeEnd == 0){
                        timeEnd = global;
                    }
                    sprintf(Buffer, "Playing Time: %d (sec)", (timeEnd - timeStart)/181);
                    showTextToLine(Buffer, SCREEN_ROWS / 2);
                    displayMode(TEXT_MODE);

                    // Play again
                    if (checkDirectionTrigger(DirectionPad, DirectionRight) && checkDirectionTrigger(ToggleButtons, DirectionLeft))
                    {
                        // Initialize game
                        initGame();
                        displayMode(GRAPHICS_MODE);
                    }
                }

                // Update ball location
                pingPongX += ballSpeedX;
                pingPongY += ballSpeedY;
                controlObject(SMALL_SPRITE, FULLY_OPAQUE, pingPongX, pingPongY, 0, ballCanvasID, ballObjectID);

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

void fillCanvas()
{
    // Fill out bat canvas (large canvas)
    for (int y = 0; y < rectangleHeight; y++) // Iterate over the height of the rectangle
    {
        for (int x = 0; x < rectangleWidth; x++) // Iterate over the width of the rectangle
        {
            batCanvas[y * LARGE_SPRITE_SIZE + x] = WHITE;
        }
    }

    // Fill out circle data (small canvas)
    int centerX = 8; // X-coordinate of the center of the circle
    int centerY = 8; // Y-coordinate of the center of the circle

    for (int y = 0; y < SMALL_SPRITE_SIZE; y++)
    {
        for (int x = 0; x < SMALL_SPRITE_SIZE; x++)
        {
            int dx = x - centerX;
            int dy = y - centerY;
            int distance = dx * dx + dy * dy; // Calculate squared distance to the center

            if (distance <= ballRadius * ballRadius)
            {
                ballCanvas[y * SMALL_SPRITE_SIZE + x] = ORANGE;
            }
            else
            {
                ballCanvas[y * SMALL_SPRITE_SIZE + x] = NO_COLOR;
            }
        }
    }

    for(int y = 0; y < BACKGROUND_PIXEL_HEIGHT; y++){
        for(int x = 0; x < BACKGROUND_PIXEL_WIDTH; x++){
            if(y%2==0){
                if(x%2==0) backgroundPixel[y * BACKGROUND_PIXEL_WIDTH + x] = 25;
                else backgroundPixel[y * BACKGROUND_PIXEL_WIDTH + x] = 2;
            }
            else{
                if(x%2==1) backgroundPixel[y * BACKGROUND_PIXEL_WIDTH + x] = 25;
                else backgroundPixel[y * BACKGROUND_PIXEL_WIDTH + x] = 2;
            }
            
        }
    }
    for(int y = 0; y < TILE_ENTRY_HEIGHT; y++){
        for(int x = 0; x < TILE_ENTRY_WIDTH; x++){

            backgroundTileEntry0[y*TILE_ENTRY_WIDTH+x] = 1;
            backgroundTileEntry1[y*TILE_ENTRY_WIDTH+x] = 7;
            backgroundTileEntry2[y*TILE_ENTRY_WIDTH+x] = 25;
        }
    }
    for(int y = 0; y < BACKGROUND_TILE_HEIGHT; y++){
        for(int x = 0; x < BACKGROUND_TILE_WIDTH; x++){
            if(y%2==0){
                if(x%2==0) backgroundTile[y*BACKGROUND_TILE_WIDTH+x] = 2;
            }
            else{
                if(x%2==1) backgroundTile[y*BACKGROUND_TILE_WIDTH+x] = 2;
            }
        }
    }
}

bool checkCollision(int playerTopLeftX, int playerTopLeftY, int pingPongX, int pingPongY, int rectangleWidth, int rectangleHeight, int ballRadius)
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

void handleCollision(float *speedX, float *speedY, float *pingPongX, float *pingPongY, int batX, int batY)
{
    if (*speedX < 0) // If the ball is moving to the left
    {
        *speedX -= 0.1;
    }
    else // If the ball is moving to the right
    {
        *speedX += 0.1;
    }

    *speedX = -(*speedX); // Change ball X direction

    // Change ball angle based on where it hits the bat
    int ballCenterY = *pingPongY + ballRadius;
    int hitPos = batY + rectangleHeight - ballCenterY;
    if (hitPos > rectangleHeight)
    { // hit at the bottom
        hitPos = rectangleHeight;
    }
    else if (hitPos < 0)
    { // hit at the top
        hitPos = 0;
    }
    float hitSegment = hitPos / ((float)rectangleHeight / 6.0);
    float newSpeedY = 0;
    if (0 <= hitSegment && hitSegment < 1)
    {
        newSpeedY = 0.75;
    }
    else if (1 <= hitSegment && hitSegment < 2)
    {
        newSpeedY = 0.5;
    }
    else if (2 <= hitSegment && hitSegment < 3)
    {
        newSpeedY = 0.25;
    }
    else if (hitSegment == 3.0)
    {
        newSpeedY = 0;
    }
    else if (3 < hitSegment && hitSegment < 4)
    {
        newSpeedY = 0.25;
    }
    else if (4 <= hitSegment && hitSegment < 5)
    {
        newSpeedY = 0.5;
    }
    else if (5 <= hitSegment && hitSegment <= 6)
    {
        newSpeedY = 0.75;
    }

    // Reflect the ball on the Y-axis
    if (*speedY >= 0) // Ball moving downwards
    {
        *speedY = newSpeedY; // Continue moving downwards, with adjusted speed
    }
    else if (*speedY < 0) // Ball moving upwards
    {
        *speedY = -newSpeedY; // Continue moving upwards, with adjusted speed
    }

    // Adjust the ball's position to avoid multiple collisions (teleporting)
    if (*speedX > 0 && *pingPongX < batX + rectangleWidth)
    {
        *pingPongX = batXOffset + rectangleWidth;
    }
    if (*speedX < 0 && *pingPongX + ballRadius * 2 > batX)
    {
        *pingPongX = xPosMax - batXOffset - rectangleWidth - ballRadius * 2;
    }
}


void initGame(){
    clearTextData();

    // Player 1 and 2 starting position
    player1X = 0 + batXOffset;
    player1Y = yPosMax / 2 - rectangleHeight / 2;
    player2X = xPosMax - rectangleWidth - batXOffset;
    player2Y = yPosMax / 2 - rectangleHeight / 2;

    // Ball starting position
    pingPongX = xPosMax / 2 - ballRadius / 2;
    pingPongY = yPosMax / 2 - ballRadius / 2;

    // Set random speed for the ball
    global = getCurrentTime();
    srand(global);
    ballSpeedX = minSpeedX + (rand() / (float)RAND_MAX) * (maxSpeedX - minSpeedX);
    ballSpeedY = minSpeedY + (rand() / (float)RAND_MAX) * (maxSpeedY - minSpeedY);

    // Reset game timer
    timeStart = 0;
    timeEnd = 0;
    resetTimer();
}

uint32_t BackgroundControl(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index, uint8_t mode) {
    return (((uint32_t)mode & 0x1) << 31) | (((uint32_t)index) << 28) | (((uint32_t)z) << 25) | (((uint32_t)(y + 288) & 0x1FF) << 12) | (((uint32_t)(x + 512) & 0x3FF) << 2) | (palette & 0x3);
}
