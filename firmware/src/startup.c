#include <stdint.h>
#include <stdlib.h>
#include <string.h>



extern uint8_t _erodata[];
extern uint8_t _data[];
extern uint8_t _edata[];
extern uint8_t _sdata[];
extern uint8_t _esdata[];
extern uint8_t _bss[];
extern uint8_t _ebss[];
#define INTERRUPT_ENABLE    (*((volatile uint32_t *)0x40000000))
#define INTERRUPT_PENDING   (*((volatile uint32_t *)0x40000004))

#define CIE_BIT 0
#define VIE_BIT 1
#define CMIE_BIT 2

volatile char *VIDEO_MEMORY1 = (volatile char *)(0x50000000 + 0xF4800);
volatile uint32_t video_interrupt_count = 0;


// Adapted from https://stackoverflow.com/questions/58947716/how-to-interact-with-risc-v-csrs-by-using-gcc-c-code
__attribute__((always_inline)) inline uint32_t csr_mstatus_read(void){
    uint32_t result;
    asm volatile ("csrr %0, mstatus" : "=r"(result));
    return result;
}

__attribute__((always_inline)) inline void csr_mstatus_write(uint32_t val){
    asm volatile ("csrw mstatus, %0" : : "r"(val));
}

__attribute__((always_inline)) inline void csr_write_mie(uint32_t val){
    asm volatile ("csrw mie, %0" : : "r"(val));
}

__attribute__((always_inline)) inline void csr_enable_interrupts(void){
    asm volatile ("csrsi mstatus, 0x8");
}

__attribute__((always_inline)) inline void csr_disable_interrupts(void){
    asm volatile ("csrci mstatus, 0x8");
}

#define MTIME_LOW       (*((volatile uint32_t *)0x40000008))
#define MTIME_HIGH      (*((volatile uint32_t *)0x4000000C))
#define MTIMECMP_LOW    (*((volatile uint32_t *)0x40000010))
#define MTIMECMP_HIGH   (*((volatile uint32_t *)0x40000014))
#define CONTROLLER      (*((volatile uint32_t *)0x40000018))

void init(void){
    uint8_t *Source = _erodata;
    uint8_t *Base = _data < _sdata ? _data : _sdata;
    uint8_t *End = _edata > _esdata ? _edata : _esdata;
    INTERRUPT_ENABLE |= (1 << VIE_BIT) | (1 << CMIE_BIT);  // Enable video and command interrupts
    // INTERRUPT_ENABLE |= (1 << CMIE_BIT);  // Enable video and command interrupts
    while(Base < End){
        *Base++ = *Source++;
    }
    Base = _bss;
    End = _ebss;
    while(Base < End){
        *Base++ = 0;
    }

    csr_write_mie(0x888);       // Enable all interrupt soruces
    csr_enable_interrupts();    // Global interrupt enable
    MTIMECMP_LOW = 1;
    MTIMECMP_HIGH = 0;
}

extern volatile int global;
extern volatile uint32_t controller_status;

void c_interrupt_handler(void){
    uint64_t NewCompare = (((uint64_t)MTIMECMP_HIGH)<<32) | MTIMECMP_LOW;
    NewCompare += 100;
    MTIMECMP_HIGH = NewCompare>>32;
    MTIMECMP_LOW = NewCompare;
    global++;
    if (INTERRUPT_PENDING & (1 << VIE_BIT)) {
        video_interrupt_count++;

        INTERRUPT_PENDING |= (1 << VIE_BIT);
        // INTERRUPT_ENABLE &= ~(1 << VIE_BIT);  // Disable video interrupt

    }

    controller_status = CONTROLLER;

}

