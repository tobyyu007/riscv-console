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
uint8_t pauseCanvas[LARGE_SPRITE_SIZE * LARGE_SPRITE_SIZE];

uint8_t batBackgroundCanvas[LARGE_SPRITE_SIZE * LARGE_SPRITE_SIZE];
uint8_t ballBackgroundCanvas[SMALL_SPRITE_SIZE * SMALL_SPRITE_SIZE];
uint8_t pauseBackgroundCanvas[LARGE_SPRITE_SIZE * LARGE_SPRITE_SIZE];

// Background Canvas
uint8_t normalBackgroundCanvas[BACKGROUND_PIXEL_SIZE];
uint8_t halfTimeBackgroundCanvas[BACKGROUND_PIXEL_SIZE];
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
int ballRadius = 8;     // Radius of the ball
float minSpeedX = 0.7;  // Minimum X axis speed of the ball
float maxSpeedX = 0.8;  // Maximum X axis speed of the ball
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

// Game Time limit
int timeLimit = 10800; // Around 1 minute
bool halfTime = false;

int last_global = 0;
int global = 0;

void fillCanvas();
bool collision(int playerTopLeftX, int playerTopLeftY, int pingPongX, int pingPongY, int rectangleWidth, int rectangleHeight, int ballRadius);
void handleCollision(float *speedX, float *speedY, float *pingPongX, float *pingPongY, int batX, int batY);
bool checkCollision(int playerTopLeftX, int playerTopLeftY, int pingPongX, int pingPongY, int rectangleWidth, int rectangleHeight, int ballRadius);
void clearTextData();
void showTextToLine(const char *text, int line);
void initGame();



int main()
{
    int countdown = 1;
    global = getCurrentTick();
    bool gameStart = false;
    enableInterrupt(CMDInterrupt);

    // Graphics intialization
    displayMode(GRAPHICS_MODE);
    struct windowSize screenResolution = getWindowSize();
    xPosMax = screenResolution.width;
    yPosMax = screenResolution.height;
    char *Buffer = AllocateMemory(32);

    // Initialize game
    initGame();

    // fill in Canvas bufffer
    fillCanvas();

    // create canvas
    int batCanvasID = createCanvas(LARGE_SPRITE, batCanvas, LARGE_SPRITE_SIZE * LARGE_SPRITE_SIZE);
    int ballCanvasID = createCanvas(SMALL_SPRITE, ballCanvas, SMALL_SPRITE_SIZE * SMALL_SPRITE_SIZE);
    int pauseCanvasID = createCanvas(LARGE_SPRITE, pauseCanvas, LARGE_SPRITE_SIZE * LARGE_SPRITE_SIZE);

    // uint32_t batCanvasBackgroundID = createCanvas(LARGE_SPRITE, batCanvas, LARGE_SPRITE_SIZE * LARGE_SPRITE_SIZE);
    // uint32_t ballCanvasBackgroundID = createCanvas(SMALL_SPRITE, ballCanvas, SMALL_SPRITE_SIZE * SMALL_SPRITE_SIZE);
    // uint32_t pauseCanvasBackgroundID = createCanvas(LARGE_SPRITE, pauseCanvas, LARGE_SPRITE_SIZE * LARGE_SPRITE_SIZE);

    // create object
    int player1BatObjectID = createObject(LARGE_SPRITE, FULLY_OPAQUE, player1X, player1Y, 0, batCanvasID);
    int player2BatObjectID = createObject(LARGE_SPRITE, FULLY_OPAQUE, player2X, player2Y, 0, batCanvasID);
    int ballObjectID = createObject(SMALL_SPRITE, FULLY_OPAQUE, pingPongX, pingPongY, 0, ballCanvasID);
    int pauseObjectID = 0;
    // create background canvas
    int normalBackgroundCanvasID = createBackgroundCanvas(BACKGROUND_PIXEL, normalBackgroundCanvas, BACKGROUND_PIXEL_SIZE);
    int halfTimeBackgroundCanvasID = createBackgroundCanvas(BACKGROUND_PIXEL, halfTimeBackgroundCanvas, BACKGROUND_PIXEL_SIZE);

    // create background object
    int BackgroundObjectID = createBackgroundObject(BACKGROUND_PIXEL, FULLY_OPAQUE, 0, 0, 0, halfTimeBackgroundCanvasID);
    controlBackgroundObject(BACKGROUND_PIXEL, FULLY_OPAQUE, 0, 0, 0, normalBackgroundCanvasID, BackgroundObjectID);

    while (1)
    {
        if (global != last_global)
        {
            if (gameStart == false)
            {
                // Before starting the game

                // Instructions
                sprintf(Buffer, "Welcome to Pong!");
                showTextToLine(Buffer, 2);
                sprintf(Buffer, "Player 1: Use \"W\" and \"X\" to move up and down");
                showTextToLine(Buffer, 16);
                sprintf(Buffer, "Player 2: Use \"U\" and \"K\" to move up and down");
                showTextToLine(Buffer, 18);
                sprintf(Buffer, "Press \"CMD\" to pause or resume the game");
                showTextToLine(Buffer, 20);
                sprintf(Buffer, "Press D and J together to start the game");
                showTextToLine(Buffer, SCREEN_ROWS / 2 + 10);
                sprintf(Buffer, "Time limit: %d seconds", timeLimit/180);
                showTextToLine(Buffer, SCREEN_ROWS / 2 + 12);
                sprintf(Buffer, "Background color changes at half the time limit");
                showTextToLine(Buffer, SCREEN_ROWS / 2 + 13);
                displayMode(TEXT_MODE);

                if (checkDirectionTrigger(DirectionPad, DirectionRight) && checkDirectionTrigger(ToggleButtons, DirectionLeft))
                {
                    startTimer();
                    gameStart = true;
                    displayMode(GRAPHICS_MODE);
                    clearTextData();
                }
            }

            else // Game is started
            {
                if (checkInterruptTrigger(CMDInterrupt))
                {
                    clearInterruptTrigger(CMDInterrupt);
                    pauseObjectID = createObject(LARGE_SPRITE, FULLY_TRANSPARENT, xPosMax / 2 - LARGE_SPRITE_SIZE / 2, yPosMax / 2 - LARGE_SPRITE_SIZE / 2, 0, pauseCanvasID);
                    controlObject(LARGE_SPRITE, FULLY_TRANSPARENT, xPosMax / 2 - LARGE_SPRITE_SIZE / 2, yPosMax / 2 - LARGE_SPRITE_SIZE / 2, 0, pauseCanvasID, pauseObjectID);
                    enableInterrupt(VideoInterrupt);
                    while (checkInterruptTrigger(VideoInterrupt))
                    {
                        if (checkInterruptTrigger(CMDInterrupt))
                        {
                            clearInterruptTrigger(CMDInterrupt);
                            clearInterruptTrigger(VideoInterrupt);
                            disableInterrupt(VideoInterrupt);
                            startTimer();
                        }
                    }
                    freeObject(LARGE_SPRITE, pauseObjectID);
                }

                if (timeElapsed() >= timeLimit / 2 && !halfTime)
                {
                    controlBackgroundObject(BACKGROUND_PIXEL, FULLY_OPAQUE, 0, 0, 0, halfTimeBackgroundCanvasID, BackgroundObjectID);
                    halfTime = true;
                }

                else if (controllerEventTriggered())
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

                // Game Ends
                if (pingPongX <= 0 || pingPongX + ballRadius * 2 >= xPosMax || timeElapsed() / 100 >= timeLimit)
                {
                    if (pingPongX <= 0)
                    {
                        strcpy(Buffer, "Player 2 wins!");
                        showTextToLine(Buffer, SCREEN_ROWS / 2 - 1);
                    }
                    else if (pingPongX + ballRadius * 2 >= xPosMax)
                    {
                        strcpy(Buffer, "Player 1 wins!");
                        showTextToLine(Buffer, SCREEN_ROWS / 2 - 1);
                    }
                    else
                    {
                        strcpy(Buffer, "Time's up!");
                        showTextToLine(Buffer, SCREEN_ROWS / 2 - 1);
                    }

                    endTimer();
                    strcpy(Buffer, "Press D and J together to restart");
                    showTextToLine(Buffer, SCREEN_ROWS / 2 + 2);
                    sprintf(Buffer, "Playing Time: %d seconds", timeElapsed() / 180);
                    showTextToLine(Buffer, SCREEN_ROWS / 2 + 4);
                    displayMode(TEXT_MODE);

                    enableInterrupt(VideoInterrupt);
                    while (checkInterruptTrigger(VideoInterrupt))
                    {
                        // Play again
                        if (checkDirectionTrigger(DirectionPad, DirectionRight) && checkDirectionTrigger(ToggleButtons, DirectionLeft))
                        {
                            // Re-Initialize game
                            initGame();
                            controlBackgroundObject(BACKGROUND_PIXEL, FULLY_OPAQUE, 0, 0, 0, normalBackgroundCanvasID, BackgroundObjectID);
                            startTimer();
                            displayMode(GRAPHICS_MODE);
                            clearInterruptTrigger(VideoInterrupt);
                            disableInterrupt(VideoInterrupt);
                        }
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
    for (int y = 0; y < LARGE_SPRITE_SIZE; y++)
    { // Iterate over each row
        for (int x = 0; x < LARGE_SPRITE_SIZE; x++)
        { // Iterate over each column
            if (y < rectangleHeight && x < rectangleWidth)
            {
                // Inside the rectangle area
                batCanvas[y * LARGE_SPRITE_SIZE + x] = WHITE;
            }
            else
            {
                // Outside the rectangle (background area)
                batCanvas[y * LARGE_SPRITE_SIZE + x] = 0;
            }
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

    // Fill out pause icon (large canvas)
    centerX = LARGE_SPRITE_SIZE / 2;
    centerY = LARGE_SPRITE_SIZE / 2;
    int CIRCLE_THICKNESS = 5;
    int radius = LARGE_SPRITE_SIZE / 2 - CIRCLE_THICKNESS; // Adjust radius for the thickness

    // Dimensions for the inner rectangles (pause button bars)
    int rectWidth = LARGE_SPRITE_SIZE / 8;         // Width of each rectangle
    int rectHeight = LARGE_SPRITE_SIZE / 2;        // Height of each rectangle
    int rect1XStart = centerX - rectWidth * 3 / 2; // X start for the first rectangle
    int rect2XStart = centerX + rectWidth / 2;     // X start for the second rectangle

    for (int y = 0; y < LARGE_SPRITE_SIZE; y++)
    {
        for (int x = 0; x < LARGE_SPRITE_SIZE; x++)
        {
            int dx = x - centerX;
            int dy = y - centerY;
            int distanceSquared = dx * dx + dy * dy;
            int radiusSquared = radius * radius;
            int thicknessSquared = CIRCLE_THICKNESS * CIRCLE_THICKNESS;
            // Drawing the thin outer ring of the circle
            if (distanceSquared >= radiusSquared - thicknessSquared && distanceSquared <= radiusSquared + thicknessSquared)
            {
                pauseCanvas[y * LARGE_SPRITE_SIZE + x] = YELLOW;
            }
            else
            {
                pauseCanvas[y * LARGE_SPRITE_SIZE + x] = NO_COLOR; // Transparent or background color
            }

            // Drawing the rectangles inside the circle
            if ((x >= rect1XStart && x < rect1XStart + rectWidth && y >= centerY - rectHeight / 2 && y < centerY + rectHeight / 2) ||
                (x >= rect2XStart && x < rect2XStart + rectWidth && y >= centerY - rectHeight / 2 && y < centerY + rectHeight / 2))
            {
                pauseCanvas[y * LARGE_SPRITE_SIZE + x] = YELLOW; // Inside the rectangle
            }
        }
    }

    // Draw background
    for (int y = 0; y < BACKGROUND_PIXEL_HEIGHT; y++)
    {
        for (int x = 0; x < BACKGROUND_PIXEL_WIDTH; x++)
        {
            normalBackgroundCanvas[y * BACKGROUND_PIXEL_WIDTH + x] = BLACK;
        }
    }
    for (int y = 0; y < BACKGROUND_PIXEL_HEIGHT; y++)
    {
        for (int x = 0; x < BACKGROUND_PIXEL_WIDTH; x++)
        {
            halfTimeBackgroundCanvas[y * BACKGROUND_PIXEL_WIDTH + x] = PLUM;
        }
    }
    // for(int y = 0; y < TILE_ENTRY_HEIGHT; y++){
    //     for(int x = 0; x < TILE_ENTRY_WIDTH; x++){

    //         backgroundTileEntry0[y*TILE_ENTRY_WIDTH+x] = 1;
    //         backgroundTileEntry1[y*TILE_ENTRY_WIDTH+x] = 7;
    //         backgroundTileEntry2[y*TILE_ENTRY_WIDTH+x] = 25;
    //     }
    // }
    // for(int y = 0; y < BACKGROUND_TILE_HEIGHT; y++){
    //     for(int x = 0; x < BACKGROUND_TILE_WIDTH; x++){
    //         if(y%2==0){
    //             if(x%2==0) backgroundTile[y*BACKGROUND_TILE_WIDTH+x] = 2;
    //         }
    //         else{
    //             if(x%2==1) backgroundTile[y*BACKGROUND_TILE_WIDTH+x] = 2;
    //         }
    //     }
    // }
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
        *speedX -= 0.05;
    }
    else // If the ball is moving to the right
    {
        *speedX += 0.05;
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

void initGame()
{
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
    global = getCurrentTick();
    srand(global);
    ballSpeedX = minSpeedX + (rand() / (float)RAND_MAX) * (maxSpeedX - minSpeedX);
    if (rand() % 2 == 0)
    {
        ballSpeedX = -ballSpeedX;
    }
    ballSpeedY = minSpeedY + (rand() / (float)RAND_MAX) * (maxSpeedY - minSpeedY);

    // Reset game timer
    resetTimer();
}
