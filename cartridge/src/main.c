#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "event.h"
#include "graphic.h"
#include "memory.h"
#include "timer.h"

#define INTERRUPT_ENABLE_REGISTER (*((volatile uint32_t *)0x40000000))
#define INTERRUPT_PENDING_REGISTER (*((volatile uint32_t *)0x40000004))
#define MODE_REGISTER     (*((volatile uint32_t *)0x500F6780))
#define VIE_BIT 1
#define CMIE_BIT 2

// Canvas
uint8_t batCanvas[LARGE_SPRITE_SIZE * LARGE_SPRITE_SIZE];
uint8_t ballCanvas[SMALL_SPRITE_SIZE * SMALL_SPRITE_SIZE];
uint8_t pauseCanvas[LARGE_SPRITE_SIZE * LARGE_SPRITE_SIZE];

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


int main()
{
    int countdown = 1;
    global = getCurrentTime();
    bool gameStart = false;
    bool gamePaused = false;
    enableInterrupt(CMDInterrupt);
    int CMDCount = 0;
    int in = 0;

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
    uint32_t batCanvasID = createCanvas(LARGE_SPRITE, batCanvas, LARGE_SPRITE_SIZE * LARGE_SPRITE_SIZE);
    uint32_t ballCanvasID = createCanvas(SMALL_SPRITE, ballCanvas, SMALL_SPRITE_SIZE * SMALL_SPRITE_SIZE);
    uint32_t pauseCanvasID = createCanvas(LARGE_SPRITE, pauseCanvas, LARGE_SPRITE_SIZE * LARGE_SPRITE_SIZE);

    // create object
    uint32_t player1BatObjectID = createObject(LARGE_SPRITE, FULLY_OPAQUE, player1X, player1Y, 0, batCanvasID);
    uint32_t player2BatObjectID = createObject(LARGE_SPRITE, FULLY_OPAQUE, player2X, player2Y, 0, batCanvasID);
    uint32_t ballObjectID = createObject(SMALL_SPRITE, FULLY_OPAQUE, pingPongX, pingPongY, 0, ballCanvasID);
    // uint32_t pauseObjectID = createObject(LARGE_SPRITE, FULLY_TRANSPARENT, xPosMax / 2 - LARGE_SPRITE_SIZE / 2, yPosMax / 2 - LARGE_SPRITE_SIZE / 2, 0, pauseCanvasID);  // 這個沒有作用
    uint32_t pauseObjectID = 0;


    while (1)
    {
        if (global != last_global)
        {
            if (gameStart == false)
            {
                // Before starting the game
                strcpy(Buffer, "Press D and J to start");
                showTextToLine(Buffer, SCREEN_ROWS / 2);
                displayMode(TEXT_MODE);
                if (checkDirectionTrigger(DirectionPad, DirectionRight) && checkDirectionTrigger(ToggleButtons, DirectionLeft))
                {
                    gameStart = true;
                    displayMode(GRAPHICS_MODE);
                    clearTextData();
                    StartTimer();
                }
            }

            else  // Game is started
            {
                if(checkInterruptTrigger(CMDInterrupt)){
                    pauseObjectID = createObject(LARGE_SPRITE, FULLY_OPAQUE, xPosMax / 2 - LARGE_SPRITE_SIZE / 2, yPosMax / 2 - LARGE_SPRITE_SIZE / 2, 0, pauseCanvasID);
                    CMDCount += 1;
                    // enableInterrupt(VideoInterrupt);
                    INTERRUPT_ENABLE_REGISTER |= (1 << VIE_BIT);
                    // displayMode(TEXT_MODE);
                    while(checkInterruptTrigger(CMDInterrupt) == false){
                        CMDCount++;
                        INTERRUPT_ENABLE_REGISTER &= (0 << VIE_BIT);
                        freeObject(LARGE_SPRITE, pauseObjectID);
                    }
                    // INTERRUPT_PENDING_REGISTER |= (1 << VIE_BIT);
                    // INTERRUPT_ENABLE_REGISTER &= (0 << VIE_BIT);
                    controlObject(LARGE_SPRITE, FULLY_TRANSPARENT, xPosMax / 2 - LARGE_SPRITE_SIZE / 2, yPosMax / 2 - LARGE_SPRITE_SIZE / 2, 0, pauseCanvasID, pauseObjectID);
                    freeObject(LARGE_SPRITE, pauseObjectID);
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

    // Fill out pause icon (large canvas)
    centerX = LARGE_SPRITE_SIZE / 2;
    centerY = LARGE_SPRITE_SIZE / 2;
    int CIRCLE_THICKNESS = 5;
    int radius = LARGE_SPRITE_SIZE / 2 - CIRCLE_THICKNESS; // Adjust radius for the thickness

    // Dimensions for the inner rectangles (pause button bars)
    int rectWidth = LARGE_SPRITE_SIZE / 8; // Width of each rectangle
    int rectHeight = LARGE_SPRITE_SIZE / 2; // Height of each rectangle
    int rect1XStart = centerX - rectWidth * 3 / 2; // X start for the first rectangle
    int rect2XStart = centerX + rectWidth / 2; // X start for the second rectangle

    for (int y = 0; y < LARGE_SPRITE_SIZE; y++) {
        for (int x = 0; x < LARGE_SPRITE_SIZE; x++) {
            int dx = x - centerX;
            int dy = y - centerY;
            int distanceSquared = dx * dx + dy * dy;
            int radiusSquared = radius * radius;
            int thicknessSquared = CIRCLE_THICKNESS * CIRCLE_THICKNESS;
            // Drawing the thin outer ring of the circle
            if (distanceSquared >= radiusSquared - thicknessSquared && distanceSquared <= radiusSquared + thicknessSquared) {
                pauseCanvas[y * LARGE_SPRITE_SIZE + x] = YELLOW;
            } else {
                pauseCanvas[y * LARGE_SPRITE_SIZE + x] = 0; // Transparent or background color
            }

            // Drawing the rectangles inside the circle
            if ((x >= rect1XStart && x < rect1XStart + rectWidth && y >= centerY - rectHeight / 2 && y < centerY + rectHeight / 2) ||
                (x >= rect2XStart && x < rect2XStart + rectWidth && y >= centerY - rectHeight / 2 && y < centerY + rectHeight / 2)) {
                pauseCanvas[y * LARGE_SPRITE_SIZE + x] = YELLOW; // Inside the rectangle
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