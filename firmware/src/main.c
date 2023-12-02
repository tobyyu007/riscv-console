#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ManageSprite.h"
#include "ControlSprite.h"
#include "Background.h"
#include "colors.h"

volatile int global = 42;
extern volatile char *VIDEO_MEMORY;
// volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);
volatile uint32_t *CartridgeStatus = (volatile uint32_t *)(0x4000001C);
typedef void(*FunctionPtr)(void);
extern volatile uint32_t video_interrupt_count;
extern volatile uint32_t CMD_interrupt_count;
volatile uint32_t *GRAPHICS_MODE = (volatile uint32_t *)(0x500F6780);


void initSpriteSystem(void);
void initializePalette(void);
void initBackgroundSystem(void);

int createBackgroundCanvas(BackgroundType type, const uint8_t *buffer, size_t bufferSize);
int createBackgroundTileEntry(int tileIndex, int entryIndex, const uint8_t *buffer, size_t bufferSize);



int main() {
    int a = 4;
    int b = 12;
    int last_global = 42;
    int x_pos = 12;
    char *Buffer = malloc(128);
    strcpy(Buffer, "OS Started");
    strcpy((char *)VIDEO_MEMORY, Buffer);

    initBackgroundSystem();
    initializePalette();
    initSpriteSystem();
    initSpriteControlSystem();


    *GRAPHICS_MODE = 0; // 0: text mode/ 1: graphic mode 

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
