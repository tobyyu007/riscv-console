#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"

volatile int global = 42;
volatile uint32_t controller_status = 0;
volatile int count = 0;
volatile uint32_t mcause = 0;

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);
volatile uint32_t *MODE_REGISTER = (volatile uint32_t *)(0x500F6780);

int main() {
    int a = 4;
    int b = 12;
    int last_global = 42;
    int x_pos = 12; // read last position from save (from before power-off), if
                    // none exists, set to 12
    int countdown = 1;

    last_global = GetTicks();

    // EnableTextMode();
    EnableTextMode();
    
    // Output to screen
    char Buffer[64];
    snprintf(Buffer, sizeof(Buffer), "GAME INSERTED");
    strcpy((char *)VIDEO_MEMORY, Buffer);

    while (1) {
        int c = a + b + global;
        if (a == 11) {
            b++;
        }
        global = GetTicks();
        if (global != last_global) {
            controller_status = GetController();
            if (controller_status) {
                VIDEO_MEMORY[x_pos] = ' ';
                if (controller_status & 0x1) {
                    if (x_pos & 0x3F) {
                        x_pos--;
                    }
                }
                if (controller_status & 0x2) {
                    if (x_pos >= 0x40) {
                        x_pos -= 0x40;
                    }
                }
                if (controller_status & 0x4) {
                    if (x_pos < 0x8C0) {
                        x_pos += 0x40;
                    }
                }
                if (controller_status & 0x8) {
                    if ((x_pos & 0x3F) != 0x3F) {
                        x_pos++;
                    }
                }
                VIDEO_MEMORY[x_pos] = 'X';
            }
            last_global = global;
        }
    }
    return 0;
}
