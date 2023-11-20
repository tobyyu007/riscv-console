#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <graphic.h>
#include "colors_index.h"
#include "sprite_sizes.h"


uint32_t GetTicks(void);
uint32_t GetController(void);
uint32_t InitThread(void);
uint32_t SwitchThread(uint32_t);
uint32_t ChangeMode(uint32_t);
uint32_t MediumPalette(uint32_t);
uint32_t MediumSprite(uint32_t, uint8_t*, uint32_t);
uint32_t FreeSprite(uint32_t, uint32_t);
uint32_t MediumControl(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index);
void fillOutData();

volatile uint32_t *MEDIUM_CONTROL = (volatile uint32_t *)(0x500F5F00);

volatile int global = 42;
volatile uint32_t controller_status = 0;
uint32_t thread_addr;

uint8_t spriteDataSquare[MEDIUM_SPRITE_SIZE * MEDIUM_SPRITE_SIZE]; //Create data buffer
uint8_t spriteDataCircle[MEDIUM_SPRITE_SIZE * MEDIUM_SPRITE_SIZE];


int main() {
    int countdown = 1;
    int a = 4;
    int b = 12;
    int last_global = 42;
    int x_pos = 0;

    fillOutData(); //Fill in the data buffer
    uint32_t square =  MediumSprite(MEDIUM_SPRITE, spriteDataSquare, 1024);
    uint32_t circle =  MediumSprite(MEDIUM_SPRITE, spriteDataCircle, 1024);
    // uint32_t free = FreeSprite(MEDIUM_SPRITE, square);

    // MEDIUM_CONTROL[0] = MediumControl(FULLY_OPAQUE, 0, 0, 0, square);  // Initialize the Square Object TBD
    uint32_t medium_control_id =  createObject(MEDIUM_SPRITE,FULLY_OPAQUE, 0, 0, 0, square);
    
    uint32_t mode = ChangeMode(1);  // 0: text mode/ 1: graphic mode
    int currentIndex = 0;

    while (1) {
        int c = a + b + global;
        if(global != last_global){
            if(controller_status){
                if(controller_status & 0x1){
                    if(x_pos & 0x3F){
                        x_pos--;
                    }
                }
                if(controller_status & 0x2){
                    if(x_pos >= 0x40){
                        x_pos -= 0x40;
                    }
                }
                if(controller_status & 0x4){
                    if(x_pos < 0x8C0){
                        x_pos += 0x40;
                    }
                }
                if(controller_status & 0x8){
                    if((x_pos & 0x3F) != 0x3F){
                        x_pos++;
                    }
                }
                // MEDIUM_CONTROL[0] = MediumControl(FULLY_OPAQUE, (x_pos & 0x3F)<<3, (x_pos>>6)<<3, 0, square);
                controlObject(MEDIUM_SPRITE,FULLY_OPAQUE, (x_pos & 0x3F)<<3, (x_pos>>6)<<3, 0, square, medium_control_id);
                if(controller_status & 0x10){
                    // thread_addr = InitThread();
                    // MEDIUM_CONTROL[0] = MediumControl(SEMI_OPAQUE_60, (x_pos & 0x3F)<<3, (x_pos>>6)<<3, 0, circle);
                    controlObject(MEDIUM_SPRITE,SEMI_OPAQUE_60, (x_pos & 0x3F)<<3, (x_pos>>6)<<3, 0, circle, medium_control_id);

                }
            }
            last_global = global;
        }
        countdown--;
        if(!countdown){
            global++;
            controller_status = (*((volatile uint32_t *)0x40000018));
            countdown = 1000;
        }
        // *INTERRUPT_PENDING_REGISTER &= 0x02;  // Disable VIP Pending
    }
    return 0;
}

uint32_t MediumControl(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index){
    return (((uint32_t)index)<<24) | (((uint32_t)z)<<21) | (((uint32_t)y+32)<<12) | (((uint32_t)x+32)<<2) | (palette & 0x3);
}

extern char _heap_base[];
extern char _stack[];

char *_sbrk(int numbytes){
    static char *heap_ptr = NULL;
    char *base;
    if(heap_ptr == NULL){
        heap_ptr = (char *)& _heap_base;
    }

    if((heap_ptr + numbytes) <= _stack){
        base = heap_ptr;
        heap_ptr += numbytes;
        return (base);
    }
    else{
        //erro = ENOMEM;
        return NULL;
    }
}

void fillOutData(){
    // // Fill out square data
    for(int y = 0; y < MEDIUM_SPRITE_SIZE; y++){
        for(int x = 0; x < MEDIUM_SPRITE_SIZE; x++){
            if(y%2==0){
                if(x%2==0) spriteDataSquare[y*MEDIUM_SPRITE_SIZE+x] = AMBER;
            }
            else{
                if(x%2==1) spriteDataSquare[y*MEDIUM_SPRITE_SIZE+x] = AMBER;
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
                spriteDataCircle[y * MEDIUM_SPRITE_SIZE + x] = VIOLET; // Set pixel to 1 if it's inside the circle
            } else {
                spriteDataCircle[y * MEDIUM_SPRITE_SIZE + x] = NO_COLOR; // Set pixel to 0 if it's outside the circle
            }
        }
    }
}