#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ManageSprite.h"
#include "ControlSprite.h"
#include "colors.h"

volatile int global = 42;
extern volatile char *VIDEO_MEMORY;
// volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);
volatile uint32_t *CartridgeStatus = (volatile uint32_t *)(0x4000001C);
typedef void(*FunctionPtr)(void);
extern volatile uint32_t video_interrupt_count;
extern volatile uint32_t CMD_interrupt_count;
volatile uint32_t *GRAPHICS_MODE = (volatile uint32_t *)(0x500F6780);
volatile uint8_t *BACKGROUND_DATA_PIXEL_0 = (volatile uint8_t *)(0x50000000);
volatile uint8_t *BACKGROUND_DATA_TILE_0 = (volatile uint8_t *)(0x50000000);
volatile uint8_t *BACKGROUND_DATA_TILE_1 = (volatile uint8_t *)(0x50000100);
volatile uint8_t *BACKGROUND_DATA_TILE_2 = (volatile uint8_t *)(0x50000200);
volatile uint8_t *TILE_DISPLAY_LOCATION = (volatile uint8_t *)(0x50010000);
volatile uint32_t *BACKGROUND_CONTROL = (volatile uint32_t *)(0x500F5A00);

uint32_t BackgroundControl(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index, uint8_t mode);

void initSpriteSystem(void);
void initializePalette(void);


int main() {
    int a = 4;
    int b = 12;
    int last_global = 42;
    int x_pos = 12;
    char *Buffer = malloc(128);
    strcpy(Buffer, "OS Started");
    strcpy((char *)VIDEO_MEMORY, Buffer);

    initializePalette();
    initSpriteSystem();
    initSpriteControlSystem();
    // for(int y = 0; y < 288; y++){
    //     for(int x = 0; x < 512; x++){
    //         if(y%2==0){
    //             if(x%2==0) BACKGROUND_DATA_PIXEL_0[y*512+x] = 1;
    //         }
    //         else{
    //             if(x%2==1) BACKGROUND_DATA_PIXEL_0[y*512+x] = 1;
    //         }
            
    //     }
    // }
    for(int y = 0; y < 16; y++){
        for(int x = 0; x < 16; x++){
            if(y%2==0){
                if(x%2==0) TILE_DISPLAY_LOCATION[y*16+x] = 25;

            }
            else{
                if(x%2==1) TILE_DISPLAY_LOCATION[y*16+x] = 25;
            }
            BACKGROUND_DATA_TILE_0[y*16+x] = 0;
            BACKGROUND_DATA_TILE_1[y*16+x] = 7;
            // BACKGROUND_DATA_TILE_2[y*16+x] = 25;
        }
    }
    for(int y = 0; y < 18; y++){
        for(int x = 0; x < 32; x++){
            if(y%2==0){
                if(x%2==0) TILE_DISPLAY_LOCATION[y*32+x] = 2;
                else TILE_DISPLAY_LOCATION[y*32+x] = 1;

            }
            else{
                if(x%2==1) TILE_DISPLAY_LOCATION[y*32+x] = 2;
                else TILE_DISPLAY_LOCATION[y*32+x] = 1;
            }
            // TILE_DISPLAY_LOCATION[0] = 1;
        }
    }
    // for(int x = 0; x < 32; x++){
    //     BACKGROUND_DATA_TILE_0[x] = 2;

    // }
    
    // BACKGROUND_DATA_TILE_0[1] = 2;
    *GRAPHICS_MODE = 1; // 0: text mode/ 1: graphic mode 
    BACKGROUND_CONTROL[0] = BackgroundControl(0, 0, 0, 0, 0, 1);
    while (1) {
        if(*CartridgeStatus & 0x1){
            FunctionPtr Fun = (FunctionPtr)((*CartridgeStatus) & 0xFFFFFFFC);
            Fun();
        }
        // sprintf(Buffer, "CMD Interrupts: %u", CMD_interrupt_count);
        // strcpy((char *)VIDEO_MEMORY + 16, Buffer);
    }
    return 0;
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

uint32_t BackgroundControl(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index, uint8_t mode) {
    return (((uint32_t)mode & 0x1) << 31) | (((uint32_t)index) << 28) | (((uint32_t)z) << 25) | (((uint32_t)(y + 288) & 0x1FF) << 12) | (((uint32_t)(x + 512) & 0x3FF) << 2) | (palette & 0x3);
}