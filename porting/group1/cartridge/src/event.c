#include "event.h"

bool controllerEventTriggered()
{ // Check if event is triggered
    if (GetController() == 0)
    {
        return false;
    }
    return true;
}

bool checkDirectionTrigger(ControllerType controllerType, EventType eventType)
{ // Check if the given controller's button is triggered
    Controller controller = {controllerType};
    Event event = {eventType};
    switch (controller.controllerType)
    {
    case DirectionPad:
        switch (event.direction)
        {
        case DirectionUp:
            return (GetController() & 0x2) ? 1 : 0;
        case DirectionDown:
            return (GetController() & 0x4) ? 1 : 0;
        case DirectionRight:
            return (GetController() & 0x8) ? 1 : 0;
        case DirectionLeft:
            return (GetController() & 0x1) ? 1 : 0;
        }
        break;

    case ToggleButtons:
        switch (event.direction)
        {
        case DirectionUp: // u
            return (GetController() & 0x10) ? 1 : 0;
        case DirectionDown: // k
            return (GetController() & 0x80) ? 1 : 0;
        case DirectionRight: // i
            return (GetController() & 0x20) ? 1 : 0;
        case DirectionLeft: // j
            return (GetController() & 0x40) ? 1 : 0;
        }
        break;
    }

    return false;
}

void enableInterrupt(InterruptType interruptType)
{ // Enable interrupt
    Interrupt interrupt = {interruptType};
    volatile unsigned int* interruptEnableRegister = (volatile unsigned int*)INTERRUPT_ENABLE_REGISTER;
    switch (interrupt.interruptType)
    {
    case VideoInterrupt:
        *interruptEnableRegister &= (0 << 1);
        break;
    case CMDInterrupt:
        *interruptEnableRegister &= (0 << 2);
        break;
    }
}

bool checkInterruptTrigger(InterruptType interruptType)
{ // Check if interrupt is triggered
    Interrupt interrupt = {interruptType};
    volatile unsigned int* interruptPendingRegister = (volatile unsigned int*)INTERRUPT_PENDING_REGISTER;
    switch (interrupt.interruptType)
    {
    case VideoInterrupt:
        return (*interruptPendingRegister & (1 << 1)) ? 1 : 0;
    case CMDInterrupt:
        return (*interruptPendingRegister & (1 << 2)) ? 1 : 0;
    }

    return false;
}

void clearInterruptTrigger(InterruptType interruptType)
{ // Clear interrupt
    Interrupt interrupt = {interruptType};
    volatile unsigned int* interruptPendingRegister = (volatile unsigned int*)INTERRUPT_PENDING_REGISTER;
    switch (interrupt.interruptType)
    {
    case VideoInterrupt:
        *interruptPendingRegister = (1 << 1);
        break;
    case CMDInterrupt:
        *interruptPendingRegister = (1 << 2);
        break;
    }
}

void disableInterrupt(InterruptType interruptType)
{ // Disable interrupt
    Interrupt interrupt = {interruptType};
    volatile unsigned int* interruptEnableRegister = (volatile unsigned int*)INTERRUPT_ENABLE_REGISTER;
    switch (interrupt.interruptType)
    {
    case VideoInterrupt:
        *interruptEnableRegister &= (0 << 1);
        break;
    case CMDInterrupt:
        *interruptEnableRegister &= (0 << 2);
        break;
    }
}