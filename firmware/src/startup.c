#include <stdint.h>
#include <stddef.h>

extern uint8_t _erodata[];
extern uint8_t _data[];
extern uint8_t _edata[];
extern uint8_t _sdata[];
extern uint8_t _esdata[];
extern uint8_t _bss[];
extern uint8_t _ebss[];

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
#define INTERRUPT_ENABLE_REGISTER (*((volatile uint32_t *)0x40000000))
#define INTERRUPT_PENDING_REGISTER (*((volatile uint32_t *)0x40000004))

#define VIE_BIT 1
#define CMIE_BIT 2
volatile uint32_t video_interrupt_count = 0;

void init(void){
    uint8_t *Source = _erodata;
    uint8_t *Base = _data < _sdata ? _data : _sdata;
    uint8_t *End = _edata > _esdata ? _edata : _esdata;

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

    // INTERRUPT_ENABLE_REGISTER |= (1 << VIE_BIT) | (1 << CMIE_BIT);
    INTERRUPT_ENABLE_REGISTER |= (1 << CMIE_BIT);

}

extern volatile int global;
extern volatile uint32_t controller_status;

void c_interrupt_handler(void){
    uint64_t NewCompare = (((uint64_t)MTIMECMP_HIGH)<<32) | MTIMECMP_LOW;
    NewCompare += 100;
    MTIMECMP_HIGH = NewCompare>>32;
    MTIMECMP_LOW = NewCompare;
    global++;
    if(INTERRUPT_PENDING_REGISTER & (1 << VIE_BIT)){
        video_interrupt_count++;
        INTERRUPT_PENDING_REGISTER |= (1 << VIE_BIT);
    }
    controller_status = CONTROLLER;

}

typedef uint32_t *TThreadContext;

typedef void (*TThreadEntry)(void *);


TThreadContext InitThread(uint32_t *stacktop, TThreadEntry entry,void *param);
void SwitchThread(TThreadContext *oldcontext, TThreadContext newcontext);

TThreadContext MainThread;
TThreadContext OtherThread;
void OtherThreadFunction(void *){
    int last_global = global;
    while(1){
        if(global != last_global){
            SwitchThread(&OtherThread,MainThread);
            last_global = global;
        }
    }
}

uint32_t c_system_call(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t call){
    if(1 == call){
        return global;
    }
    else if(2 == call){
        return controller_status;
    }
    else if(3 == call){
        uint32_t OtherThreadStack[128];
        MainThread = InitThread(OtherThreadStack + 128, OtherThreadFunction, NULL);
        return *MainThread;
    }
    else if(4 == call){
        OtherThread = &arg0;
        SwitchThread(&MainThread,OtherThread);
        return 1;
    }
    else if(5 == call){  // checkControllerStatus()
        volatile uint32_t controller_status = (*((volatile uint32_t *)0x40000018));
        return controller_status ? 1 : 0;
    }
    else if(6 == call){ // DirectionPadLeft()
        volatile uint32_t controller_status = (*((volatile uint32_t *)0x40000018));
        return (controller_status & 0x1) ? 1 : 0;
    }
    else if(7 == call){ // DirectionPadUp()
        volatile uint32_t controller_status = (*((volatile uint32_t *)0x40000018));
        return (controller_status & 0x2) ? 1 : 0;
    }
    else if(8 == call){ // DirectionPadDown()
        volatile uint32_t controller_status = (*((volatile uint32_t *)0x40000018));
        return (controller_status & 0x4) ? 1 : 0;
    }
    else if(9 == call){ // DirectionPadRight()
        volatile uint32_t controller_status = (*((volatile uint32_t *)0x40000018));
        return (controller_status & 0x8) ? 1 : 0;
    }
    else if(10 == call){ // ToggleButtonsUp()
        volatile uint32_t controller_status = (*((volatile uint32_t *)0x40000018));
        return (controller_status & 0x10) ? 1 : 0;
    }
    else if(11 == call){ // ToggleButtonsRight()
        volatile uint32_t controller_status = (*((volatile uint32_t *)0x40000018));
        return (controller_status & 0x20) ? 1 : 0;
    }
    else if(12 == call){ // ToggleButtonsLeft()
        volatile uint32_t controller_status = (*((volatile uint32_t *)0x40000018));
        return (controller_status & 0x40) ? 1 : 0;
    }
    else if(13 == call){ // ToggleButtonsDown()
        volatile uint32_t controller_status = (*((volatile uint32_t *)0x40000018));
        return (controller_status & 0x80) ? 1 : 0;
    }

    return -1;

}

