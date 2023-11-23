#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "event.h"
#include "graphic.h"
#include "memory.h"


uint32_t GetTicks(void);

void fillOutData();

volatile uint32_t global;

uint8_t squareCanvas[MEDIUM_SPRITE_SIZE * MEDIUM_SPRITE_SIZE]; //Create data buffer
uint8_t circleCanvas[MEDIUM_SPRITE_SIZE * MEDIUM_SPRITE_SIZE];

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);


int main() {
    global = GetTicks();
    int countdown = 1;
    int a = 4;
    int b = 12;
    int last_global = 42;
    int x_pos = 0;

    fillOutData(); //Fill in the data buffer
    uint32_t square =  createCanvas(MEDIUM_SPRITE, squareCanvas, 1024);
    uint32_t circle =  createCanvas(MEDIUM_SPRITE, circleCanvas, 1024);


    uint32_t squareObjectID =  createObject(MEDIUM_SPRITE,FULLY_OPAQUE, 0, 0, 0, square);
    
    char *Buffer = AllocateMemory(32);
    strcpy(Buffer, "Press CMD to start");
    strcpy((char *)VIDEO_MEMORY, Buffer);
    displayMode(TEXT_MODE);
    bool start = false;
    int currentIndex = 0;

    while (1) {
        int c = a + b + global;
        if(global != last_global){
            if(!start && CMDPressed()){
                start = true;
                displayMode(GRAPHICS_MODE);
            }
            
            if(controllerEventTriggered()){
                if(checkDirectionTrigger(DirectionPad, DirectionLeft)){
                    if(x_pos & 0x3F){
                        x_pos--;
                    }
                }
                if(checkDirectionTrigger(DirectionPad, DirectionUp)){
                    if(x_pos >= 0x40){
                        x_pos -= 0x40;
                    }
                }
                if(checkDirectionTrigger(DirectionPad, DirectionDown)){
                    if(x_pos < 0x8C0){
                        x_pos += 0x40;
                    }
                }
                if(checkDirectionTrigger(DirectionPad, DirectionRight)){
                    if((x_pos & 0x3F) != 0x3F){
                        x_pos++;
                    }
                }
                controlObject(MEDIUM_SPRITE,FULLY_OPAQUE, (x_pos & 0x3F)<<3, (x_pos>>6)<<3, 0, square, squareObjectID);
                // if(controller_status & 0x10){
                //     // thread_addr = InitThread();
                //     controlObject(MEDIUM_SPRITE,SEMI_OPAQUE_60, (x_pos & 0x3F)<<3, (x_pos>>6)<<3, 0, circle, medium_control_id);

                // }
            }
            last_global = global;
        }
        countdown--;
        if(!countdown){
            global++;
            countdown = 10000;
        }
    }
    return 0;
}

void fillOutData(){
    // // Fill out square data
    for(int y = 0; y < MEDIUM_SPRITE_SIZE; y++){
        for(int x = 0; x < MEDIUM_SPRITE_SIZE; x++){
            if(y%2==0){
                if(x%2==0) squareCanvas[y*MEDIUM_SPRITE_SIZE+x] = AMBER;
            }
            else{
                if(x%2==1) squareCanvas[y*MEDIUM_SPRITE_SIZE+x] = AMBER;
            }
            
        }
    }

    // Fill out circle data
    int centerX = 16;  // X-coordinate of the center of the circle
    int centerY = 16;  // Y-coordinate of the center of the circle
    int radius = 12;   // Radius of the circle

    for (int y = 0; y < MEDIUM_SPRITE_SIZE; y++) {
        for (int x = 0; x < MEDIUM_SPRITE_SIZE; x++) {
            int dx = x - centerX;
            int dy = y - centerY;
            int distance = dx * dx + dy * dy; // Calculate squared distance to the center

            if (distance <= radius * radius) {
                circleCanvas[y * MEDIUM_SPRITE_SIZE + x] = VIOLET; // Set pixel to 1 if it's inside the circle
            } else {
                circleCanvas[y * MEDIUM_SPRITE_SIZE + x] = NO_COLOR; // Set pixel to 0 if it's outside the circle
            }
        }
    }
}