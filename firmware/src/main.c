#include <stdint.h>
#include <stdlib.h>
#include <string.h>
volatile int global = 42;
volatile uint32_t controller_status = 0;

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);
volatile uint32_t *CartridgeStatus = (volatile uint32_t *)(0x4000001C);
typedef void(*FunctionPtr)(void);
volatile uint8_t *MEDIUM_DATA = (volatile uint8_t *)(0x500D0000);
volatile uint32_t *MEDIUM_PALETTE = (volatile uint32_t *)(0x500F2000);
volatile uint32_t *MEDIUM_CONTROL = (volatile uint32_t *)(0x500F5F00);
volatile uint32_t *MODE_REGISTER = (volatile uint32_t *)(0x500F6780);

uint32_t MediumControl(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index);

// char * _strcpy(char *dest, char * src){
//     while(*src){
//         *dest++ = *src++;
//     }
// }

int main() {
    int a = 4;
    int b = 12;
    int last_global = 42;
    int x_pos = 12;
    char *Buffer = malloc(32);
    strcpy(Buffer, "OS Started");
    strcpy((char *)VIDEO_MEMORY, Buffer);
    free(Buffer);
    // VIDEO_MEMORY[0] = 'H';
    // VIDEO_MEMORY[1] = 'e';
    // VIDEO_MEMORY[2] = 'l';
    // VIDEO_MEMORY[3] = 'l';
    // VIDEO_MEMORY[4] = 'o';
    // VIDEO_MEMORY[5] = ' ';
    // VIDEO_MEMORY[6] = 'W';
    // VIDEO_MEMORY[7] = 'o';
    // VIDEO_MEMORY[8] = 'r';
    // VIDEO_MEMORY[9] = 'l';
    // VIDEO_MEMORY[10] = 'd';
    // VIDEO_MEMORY[11] = '!';
    // VIDEO_MEMORY[12] = 'X';

    // Fill out sprite data
    for(int y = 0; y < 32; y++){
        for(int x = 0; x < 32; x++){
            if(y%2==0){
                if(x%2==0) MEDIUM_DATA[y*32+x] = 1;
            }
            else{
                if(x%2==1) MEDIUM_DATA[y*32+x] = 1;
            }
            
        }
    }
    MEDIUM_PALETTE[1] = 0xFFFF00FF; // A R G B
    // MEDIUM_CONTROL[0] = MediumControl(0, 0, 0, 0, 0);
    *MODE_REGISTER = 0; // 0: text mode/ 1: graphic mode 
    while (1) {
        if(*CartridgeStatus & 0x1){
            FunctionPtr Fun = (FunctionPtr)((*CartridgeStatus) & 0xFFFFFFFC);
            Fun();
        }
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