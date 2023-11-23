#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "event.h"
#include "graphic.h"
#include "memory.h"
#include "timer.h"

uint32_t CreateSprite(uint32_t, uint8_t *, uint32_t);
uint32_t FreeSprite(uint32_t, uint32_t);
uint32_t MediumControl(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index);
void fillOutData();

volatile uint32_t global;
volatile uint32_t controller_status;
uint32_t thread_addr;

uint8_t spriteDataSquare[MEDIUM_SPRITE_SIZE * MEDIUM_SPRITE_SIZE]; // Create data buffer
uint8_t spriteDataCircle[MEDIUM_SPRITE_SIZE * MEDIUM_SPRITE_SIZE];

int main()
{
    startTimer();
    controller_status = GetController();
    int countdown = 1;
    int a = 4;
    int b = 12;
    int last_global = 42;
    int x_pos = 0;

    fillOutData(); // Fill in the data buffer
    uint32_t square = createCanvas(MEDIUM_SPRITE, spriteDataSquare, 1024);
    uint32_t circle = createCanvas(MEDIUM_SPRITE, spriteDataCircle, 1024);

    uint32_t objectID = createObject(MEDIUM_SPRITE, FULLY_OPAQUE, 0, 0, 0, square);

    enableCMDInterrupt();
    char *Buffer = AllocateMemory(32);
    strcpy(Buffer, "Press CMD to start");
    displayMode(TEXT_MODE);

    if(CMDPressed()){
        disableCMDInterrupt();
        displayMode(GRAPHICS_MODE);
    }

    int currentIndex = 0;

    while (1)
    {
        int c = a + b + global;
        if (global != last_global)
        {
            if (controllerEventTriggered())
            {
                if (checkDirectionTrigger(DirectionPad, DirectionUp))
                {
                    if (x_pos & 0x3F)
                    {
                        x_pos--;
                    }
                }
                if (checkDirectionTrigger(DirectionPad, DirectionLeft))
                {
                    if (x_pos >= 0x40)
                    {
                        x_pos -= 0x40;
                    }
                }
                if (checkDirectionTrigger(DirectionPad, DirectionRight))
                {
                    if (x_pos < 0x8C0)
                    {
                        x_pos += 0x40;
                    }
                }
                if (checkDirectionTrigger(DirectionPad, DirectionDown))
                {
                    if ((x_pos & 0x3F) != 0x3F)
                    {
                        x_pos++;
                    }
                }
                controlObject(MEDIUM_SPRITE, FULLY_OPAQUE, (x_pos & 0x3F) << 3, (x_pos >> 6) << 3, 0, square, objectID);
                if (controller_status & 0x10)
                {
                    controlObject(MEDIUM_SPRITE, SEMI_OPAQUE_60, (x_pos & 0x3F) << 3, (x_pos >> 6) << 3, 0, circle, objectID);
                }
            }
            last_global = global;
        
        countdown--;
        if (!countdown)
        {
            global++;
            countdown = 1000;
        }
    }
    return 0;
}

uint32_t MediumControl(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index)
{
    return (((uint32_t)index) << 24) | (((uint32_t)z) << 21) | (((uint32_t)y + 32) << 12) | (((uint32_t)x + 32) << 2) | (palette & 0x3);
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

void fillOutData()
{
    // // Fill out square data
    for (int y = 0; y < MEDIUM_SPRITE_SIZE; y++)
    {
        for (int x = 0; x < MEDIUM_SPRITE_SIZE; x++)
        {
            if (y % 2 == 0)
            {
                if (x % 2 == 0)
                    spriteDataSquare[y * MEDIUM_SPRITE_SIZE + x] = AMBER;
            }
            else
            {
                if (x % 2 == 1)
                    spriteDataSquare[y * MEDIUM_SPRITE_SIZE + x] = AMBER;
            }
        }
    }

    // Fill out circle data
    int centerX = 16; // X-coordinate of the center of the circle
    int centerY = 16; // Y-coordinate of the center of the circle
    int radius = 12;  // Radius of the circle

    for (int y = 0; y < MEDIUM_SPRITE_SIZE; y++)
    {
        for (int x = 0; x < MEDIUM_SPRITE_SIZE; x++)
        {
            int dx = x - centerX;
            int dy = y - centerY;
            int distance = dx * dx + dy * dy; // Calculate squared distance to the center

            if (distance <= radius * radius)
            {
                spriteDataCircle[y * MEDIUM_SPRITE_SIZE + x] = VIOLET; // Set pixel to 1 if it's inside the circle
            }
            else
            {
                spriteDataCircle[y * MEDIUM_SPRITE_SIZE + x] = NO_COLOR; // Set pixel to 0 if it's outside the circle
            }
        }
    }
}