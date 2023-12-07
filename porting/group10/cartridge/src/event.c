# include "event.h"

bool controllerEventTriggered() {  // Check if event is triggered
    if(getController() == 0) {
        return false;
    }
    return true;
}


bool checkDirectionTrigger(ControllerType controllerType, EventType eventType) {  // Check if the given controller's button is triggered
    Controller controller = {controllerType};
    Event event = {eventType};
    switch (controller.controllerType) {
        case DirectionPad:
            switch (event.direction) {
                case DirectionUp:
                    return (getController() & 0x2) ? 1 : 0;
                case DirectionDown:
                    return (getController() & 0x4) ? 1 : 0;
                case DirectionRight:
                    return (getController() & 0x8) ? 1 : 0;
                case DirectionLeft:
                    return (getController() & 0x1) ? 1 : 0;
            }
            break;

        case ToggleButtons:
            switch (event.direction) {
                case DirectionUp:  // u
                    return (getController() & 0x10) ? 1 : 0;
                case DirectionDown:  // k
                    return (getController() & 0x80) ? 1 : 0;
                case DirectionRight:  // i
                    return (getController() & 0x20) ? 1 : 0;
                case DirectionLeft:  // j
                    return (getController() & 0x40) ? 1 : 0;
            }
            break;
    }

    return false;
}


void enableInterrupt(InterruptType interruptType) {  // Enable interrupt
    Interrupt interrupt = {interruptType};
    switch (interrupt.interruptType) {
        case VideoInterrupt:
            INTERRUPT_ENABLE_REGISTER &= (0 << VIE_BIT);
            break;
        case CMDInterrupt:
            INTERRUPT_ENABLE_REGISTER &= (0 << CMIE_BIT);
            break;
    }
}


bool checkInterruptTrigger(InterruptType interruptType) {  // Check if interrupt is triggered
    Interrupt interrupt = {interruptType};
    switch (interrupt.interruptType) {
        case VideoInterrupt:
            return (INTERRUPT_PENDING_REGISTER & (1 << VIE_BIT)) ? 1 : 0;
        case CMDInterrupt:
            return (INTERRUPT_PENDING_REGISTER & (1 << CMIE_BIT)) ? 1 : 0;
    }

    return false;
}


void clearInterruptTrigger(InterruptType interruptType) {  // Clear interrupt
    Interrupt interrupt = {interruptType};
    switch (interrupt.interruptType) {
        case VideoInterrupt:
            INTERRUPT_PENDING_REGISTER = (1 << VIE_BIT);
            break;
        case CMDInterrupt:
            INTERRUPT_PENDING_REGISTER = (1 << CMIE_BIT);
            break;
    }
}

void disableInterrupt(InterruptType interruptType) {  // Disable interrupt
    Interrupt interrupt = {interruptType};
    switch (interrupt.interruptType) {
        case VideoInterrupt:
            INTERRUPT_ENABLE_REGISTER &= (0 << VIE_BIT);
            break;
        case CMDInterrupt:
            INTERRUPT_ENABLE_REGISTER &= (0 << CMIE_BIT);
            break;
    }
}
