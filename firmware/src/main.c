#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>  // For sprintf


volatile int global = 42;
volatile uint32_t controller_status = 0;

volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);
volatile uint32_t *CartridgeStatus = (volatile uint32_t *)(0x4000001C);
typedef void (*FunctionPtr)(void);
extern volatile uint32_t video_interrupt_count;


int main() {
    int a = 4;
    int b = 12;
    int last_global = 42;
    int x_pos = 12;
    char *Buffer = malloc(32);
    strcpy(Buffer,"OS STARTED");
    strcpy((char *)VIDEO_MEMORY,Buffer);


    char buffer[32];
    while (1){
        if(*CartridgeStatus & 0x1){
            FunctionPtr Fun = (FunctionPtr)((*CartridgeStatus) & 0xFFFFFFFC);
            Fun();
        }
        sprintf(buffer, "Video Interrupts: %u", video_interrupt_count);
        strcpy((char *)VIDEO_MEMORY + 16, buffer);

    }

    return 0;
}

extern char _heap_base[];
extern char _stack[];

char *_sbrk(int numbytes){
  static char *heap_ptr = NULL;
  char *base;

  if (heap_ptr == NULL) {
    heap_ptr = (char *)&_heap_base;
  }

  if((heap_ptr + numbytes) <=_stack) {
    base = heap_ptr;
    heap_ptr += numbytes;
    return (base);
  }
  else {
    //errno = ENOMEM;
    return NULL;
  }

}