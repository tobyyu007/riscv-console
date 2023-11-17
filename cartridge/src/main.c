#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <event.h>

volatile int global = 42;
//volatile uint32_t controller_status = 0;
bool controller_status = false;
uint32_t GetTicks(void);
uint32_t GetController(void);
uint32_t InitThread(void);
uint32_t SwitchThread(uint32_t);

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);
volatile uint8_t *MEDIUM_DATA_SQUARE = (volatile uint8_t *)(0x500D0000);
volatile uint8_t *MEDIUM_DATA_CIRCLE = (volatile uint8_t *)(0x500D0400);
volatile uint32_t *MEDIUM_PALETTE_PINK = (volatile uint32_t *)(0x500F2000);
volatile uint32_t *MEDIUM_PALETTE_WHITE = (volatile uint32_t *)(0x500F2400);
volatile uint32_t *MEDIUM_CONTROL = (volatile uint32_t *)(0x500F5F00);
volatile uint32_t *MODE_REGISTER = (volatile uint32_t *)(0x500F6780);
volatile uint32_t *INTERRUPT_PENDING_REGISTER = (volatile uint32_t * )(0x40000004);
volatile uint32_t *INTERRUPT_ENABLE_REGISTER = (volatile uint32_t *)(0x40000000);

uint32_t MediumControl(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index);

uint32_t thread_addr;

#define CMIE_BIT 2
void fillOutData();
int main() {
    int countdown = 1;
    int a = 4;
    int b = 12;
    int last_global = 42;
    int x_pos = 0;
    fillOutData();
    MEDIUM_PALETTE_PINK[1] = 0xFFFF00FF; // A R G B
    MEDIUM_PALETTE_WHITE[1] = 0xFFFFFFFF; // A R G B
    MEDIUM_CONTROL[0] = MediumControl(0, 0, 0, 0, 0);  // Square
    *MODE_REGISTER = 1; // 0: text mode/ 1: graphic mode
    int currentIndex = 0;

    while (1) {
        int c = a + b + global;
        enableCMDInterrupt();
        if(CMDPressed()){
            if (currentIndex == 0){
                MEDIUM_CONTROL[0] = MediumControl(1, (x_pos & 0x3F)<<3, (x_pos>>6)<<3, 0, 1);
                currentIndex = 1;
            }
            else{
                MEDIUM_CONTROL[0] = MediumControl(0, (x_pos & 0x3F)<<3, (x_pos>>6)<<3, 0, 0);
                currentIndex = 0;
            }
            disableCMDInterrupt();
        }

        if(global != last_global){
            if(controllerEventTriggered()){
                //VIDEO_MEMORY[x_pos] = ' ';

                if(checkDirectionTrigger(DirectionPad, DirectionLeft)){  // Left
                    if(x_pos & 0x3F){
                        x_pos--;
                    }
                }

                if(checkDirectionTrigger(DirectionPad, DirectionUp)){  // Up
                    if(x_pos >= 0x40){
                        x_pos -= 0x40;
                    }
                }
                if(checkDirectionTrigger(DirectionPad, DirectionDown)){  // Down
                    if(x_pos < 0x8C0){
                        x_pos += 0x40;
                    }
                }
                if(checkDirectionTrigger(DirectionPad, DirectionRight)){  // Right
                    if((x_pos & 0x3F) != 0x3F){
                        x_pos++;
                    }
                }
                if (currentIndex == 1){
                    MEDIUM_CONTROL[0] = MediumControl(1, (x_pos & 0x3F)<<3, (x_pos>>6)<<3, 0, 1);
                }
                else{
                    MEDIUM_CONTROL[0] = MediumControl(0, (x_pos & 0x3F)<<3, (x_pos>>6)<<3, 0, 0);
                }

                if(checkDirectionTrigger(ToggleButtons, DirectionUp)){  // u button
                    // thread_addr = InitThread();
                    // MEDIUM_CONTROL[0] = MediumControl(1, (x_pos & 0x3F)<<3, (x_pos>>6)<<3, 0, 1);
                    if(x_pos >= 0x40){
                        x_pos -= 0x40;
                    }
                }
                if(checkDirectionTrigger(ToggleButtons, DirectionRight)){  // i button
                    // uint32_t thread_id = SwitchThread(thread_addr);
                    // MEDIUM_CONTROL[0] = MediumControl(0, (x_pos & 0x3F)<<3, (x_pos>>6)<<3, 0, 0);
                    if((x_pos & 0x3F) != 0x3F){
                        x_pos++;
                    }
                }
                if(checkDirectionTrigger(ToggleButtons, DirectionLeft)){  // j button
                    if(x_pos & 0x3F){
                        x_pos--;
                    }
                }
                if(checkDirectionTrigger(ToggleButtons, DirectionDown)){  // k button
                    if(x_pos < 0x8C0){
                        x_pos += 0x40;
                    }
                }
            }
            last_global = global;
        }
        countdown--;
        if(!countdown){
            global++;
            countdown = 5000;
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
    // Fill out square data
    for(int y = 0; y < 32; y++){
        for(int x = 0; x < 32; x++){
            if(y%2==0){
                if(x%2==0) MEDIUM_DATA_SQUARE[y*32+x] = 1;
            }
            else{
                if(x%2==1) MEDIUM_DATA_SQUARE[y*32+x] = 1;
            }
            
        }
    }

    // Fill out circle data
    int centerX = 16;  // X-coordinate of the center of the circle
    int centerY = 16;  // Y-coordinate of the center of the circle
    int radius = 12;   // Radius of the circle

    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            int dx = x - centerX;
            int dy = y - centerY;
            int distance = dx * dx + dy * dy; // Calculate squared distance to the center

            if (distance <= radius * radius) {
                MEDIUM_DATA_CIRCLE[y * 32 + x] = 1; // Set pixel to 1 if it's inside the circle
            } else {
                MEDIUM_DATA_CIRCLE[y * 32 + x] = 0; // Set pixel to 0 if it's outside the circle
            }
        }
    }
}