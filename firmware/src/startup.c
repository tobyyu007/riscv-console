#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ManageSprite.h"
#include "ControlSprite.h"
#include <stdbool.h>

#define MTIME_LOW       (*((volatile uint32_t *)0x40000008))
#define MTIME_HIGH      (*((volatile uint32_t *)0x4000000C))
#define MTIMECMP_LOW    (*((volatile uint32_t *)0x40000010))
#define MTIMECMP_HIGH   (*((volatile uint32_t *)0x40000014))
#define CONTROLLER      (*((volatile uint32_t *)0x40000018))
#define INTERRUPT_ENABLE_REGISTER (*((volatile uint32_t *)0x40000000))
#define INTERRUPT_PENDING_REGISTER (*((volatile uint32_t *)0x40000004))
#define MODE_REGISTER     (*((volatile uint32_t *)0x500F6780))
#define VIE_BIT 1
#define CMIE_BIT 2

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


volatile uint32_t video_interrupt_count = 0;
volatile uint32_t CMD_interrupt_count = 0;

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
}

extern volatile int global;
// volatile uint32_t CMD_interrupt_count;
volatile uint32_t controller_status;
volatile bool CMDInterrupted = false;

void c_interrupt_handler(void){
    uint64_t NewCompare = (((uint64_t)MTIMECMP_HIGH)<<32) | MTIMECMP_LOW;
    NewCompare += 100;
    MTIMECMP_HIGH = NewCompare>>32;
    MTIMECMP_LOW = NewCompare;
    global++;
    controller_status = CONTROLLER;

    if(INTERRUPT_PENDING_REGISTER & (1 << CMIE_BIT)){
        CMDInterrupted = true;
        // CMD_interrupt_count++;
    }
}

typedef uint32_t *TThreadContext;
typedef void (*TThreadEntry)(void *);

TThreadContext InitThread(uint32_t *stacktop, TThreadEntry entry,void *param);
void SwitchThread(TThreadContext *oldcontext, TThreadContext newcontext);

int timerStart = 0;
int timerEnd = 0;

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

extern char *VIDEO_MEMORY;

uint32_t c_system_call(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t call){
    if(1 == call){ // GetTicks
        return global;
    }
    else if(2 == call){ // GetController
        return controller_status;
    }
    else if(3 == call){ // InitThread
        uint32_t OtherThreadStack[128];
        MainThread = InitThread(OtherThreadStack + 128, OtherThreadFunction, NULL);
        return *MainThread;
    }
    else if(4 == call){ // SwitchThread
        OtherThread = &arg0;
        SwitchThread(&MainThread,OtherThread);
        return 1;
    }

    else if(5 == call){  // event.h - checkControllerStatus()
        return controller_status ? 1 : 0;
    }
    else if(6 == call){  // event.h - DirectionPadLeft()
        return (controller_status & 0x1) ? 1 : 0;
    }
    else if(7 == call){  // event.h - DirectionPadUp()
        return (controller_status & 0x2) ? 1 : 0;
    }
    else if(8 == call){  // event.h - DirectionPadDown()
        return (controller_status & 0x4) ? 1 : 0;
    }
    else if(9 == call){  // event.h - DirectionPadRight()
        return (controller_status & 0x8) ? 1 : 0;
    }
    else if(10 == call){  // event.h - ToggleButtonsUp()
        return (controller_status & 0x10) ? 1 : 0;
    }
    else if(11 == call){  // event.h - ToggleButtonsRight()
        return (controller_status & 0x20) ? 1 : 0;
    }
    else if(12 == call){  // event.h - ToggleButtonsLeft()
        return (controller_status & 0x40) ? 1 : 0;
    }
    else if(13 == call){  // event.h - ToggleButtonsDown()
        return (controller_status & 0x80) ? 1 : 0;
    }
    else if(14 == call){  // event.h - EnableCMDInterrupt()
        INTERRUPT_ENABLE_REGISTER |= (1 << CMIE_BIT);
        return 1;
    }
    else if(15 == call){  // event.h - CMDInterrupted()
        if(CMDInterrupted){
            CMDInterrupted = false;
            INTERRUPT_PENDING_REGISTER |= (1 << CMIE_BIT);
            return 1;
        }
        else{
            return 0;
        }
    }
    else if(16 == call){  // event.h - DisableCMDInterrupt()
        INTERRUPT_ENABLE_REGISTER &= (0 << CMIE_BIT);
        return 1;
    }
    else if(17 == call){  // timer.h - StartTimer()
        timerStart = global;
        timerEnd = 0;
        return 1;
    }
    else if(18 == call){  // timer.h - StopTimer()
        timerEnd = global;
        return 1;
    }
    else if(19 == call){  // timer.h - TimeElpased()
        return timerEnd-timerStart;
    }
    else if(20 == call){  // timer.h - ResetTimer()
        timerStart = 0;
        timerEnd = 0;
        return 1;
    }
    else if(21 == call){  // timer.h - GetCurrentTime()
        return global;
    }
    else if(30 == call){ // CreateControlSprite
        int result = createControlSprite(arg0, arg1);
        return result;
    }
    else if(31 == call){ // FreeControlSprite
        int result = freeControlSprite(arg0, arg1);

        return result;
    }
    else if(32 == call){
        int result = controlSprite(arg0, arg1, arg2);

        return result;
    }
    else if(50 == call){
        int result = createSprite(arg0,(const uint8_t *)(uintptr_t)arg1, arg2);
        return result;
    }
    else if(51 == call){
        int result = freeSprite(arg0, arg1);
        return result;
    }
    else if(55 == call){
        clearTextData(); 
        return 1;
    }
    else if(56 == call){
        showTextToLine((const char *)(uintptr_t)arg0, arg1);
        return 1;
    }
    else if(60 == call){
        MODE_REGISTER = arg0; // 0: text mode/ 1: graphic mode
        return 1;
    }
    else if(100 == call){ // TestSendPointer
        uintptr_t address = arg0;
        char *Buffer = (char *)address;
        strcpy((char *)VIDEO_MEMORY, Buffer);
    }
    return -1;
}
