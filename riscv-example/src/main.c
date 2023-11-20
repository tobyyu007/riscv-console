#include <stdint.h>

volatile int global = 42;
volatile uint32_t controller_status = 0;

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);
volatile uint8_t *MEDIUM_DATA = (volatile uint8_t *)(0x50000000);
volatile uint32_t *MEDIUM_PALETTE = (volatile uint8_t *)(0x500F2000);
volatile uint32_t *MEDIUM_CONTROL = (volatile uint8_t *)(0x500F5F00);
volatile uint32_t *MODE_REGISTER = (volatile uint8_t *)(0x500F6780);

uint32_t MediumControl(uint8_t palette, uint16_t x,uint16_t y, uint16_t z,uint8_t index);

int main() {
    int a = 4;
    int b = 12;
    int last_global = 42;
    int x_pos = 12;

    VIDEO_MEMORY[0] = 'H';
    VIDEO_MEMORY[1] = 'e';
    VIDEO_MEMORY[2] = 'l';
    VIDEO_MEMORY[3] = 'l';
    VIDEO_MEMORY[4] = 'o';
    VIDEO_MEMORY[5] = ' ';
    VIDEO_MEMORY[6] = 'W';
    VIDEO_MEMORY[7] = 'o';
    VIDEO_MEMORY[8] = 'r';
    VIDEO_MEMORY[9] = 'l';
    VIDEO_MEMORY[10] = 'd';
    VIDEO_MEMORY[11] = '!';
    VIDEO_MEMORY[12] = 'X';

    // Fill out sprite data
    for(int y=0;y<32;y++){
        for(int x=0;x<32;x++){
            MEDIUM_DATA[y*32+x] = 1;
        }
    }

    MEDIUM_PALETTE[1] = 0xFFFF0000; // A R G B
    MEDIUM_CONTROL[0] = (0<<24)|(0<<21)|(32<<12)|(32<<2)|(0x00);
    *MODE_REGISTER = 1;
    while (1) {
        int c = a + b + global;
        if(global != last_global){
            if(controller_status){
                VIDEO_MEMORY[x_pos] = ' ';
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
                VIDEO_MEMORY[x_pos] = 'X';
            }
            last_global = global;
        }
    }
    return 0;
}
