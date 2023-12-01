# include "event.h"

bool controllerEventTriggered() {  // Check if event is triggered
    return checkControllerStatus();
}


bool checkDirectionTrigger(ControllerType controllerType, EventType eventType) {  // Check if the given controller's button is triggered
    Controller controller = {controllerType};
    Event event = {eventType};
    switch (controller.controllerType) {
        case DirectionPad:
            switch (event.direction) {
                case DirectionUp:
                    return DirectionPadUp();
                case DirectionDown:
                    return DirectionPadDown();
                case DirectionRight:
                    return DirectionPadRight();
                case DirectionLeft:
                    return DirectionPadLeft();
            }
            break;

        case ToggleButtons:
            switch (event.direction) {
                case DirectionUp:  // u
                    return ToggleButtonsUp();
                case DirectionDown:  // k
                    return ToggleButtonsDown();
                case DirectionRight:  // i
                    return ToggleButtonsRight();
                case DirectionLeft:  // j
                    return ToggleButtonsLeft();
            }
            break;
    }

    return false;
}


void enableInterrupt(InterruptType interruptType) {  // Enable interrupt
    Interrupt interrupt = {interruptType};
    switch (interrupt.interruptType) {
        case VideoInterrupt:
            EnableVideoInterrupt();
            break;
        case CMDInterrupt:
            EnableCMDInterrupt();
            break;
    }
}


bool checkInterruptTrigger(InterruptType interruptType) {  // Check if interrupt is triggered
    Interrupt interrupt = {interruptType};
    switch (interrupt.interruptType) {
        case VideoInterrupt:
            return VideoInterrupted();
        case CMDInterrupt:
            return CMDInterrupted();
    }

    return false;
}


bool clearInterruptTrigger(InterruptType interruptType) {  // Clear interrupt
    Interrupt interrupt = {interruptType};
    switch (interrupt.interruptType) {
        case VideoInterrupt:
            ClearVideoInterrupt();
            break;
        case CMDInterrupt:
            ClearCMDInterrupt();
            break;
    }

    return false;
}

void disableInterrupt(InterruptType interruptType) {  // Disable interrupt
    Interrupt interrupt = {interruptType};
    switch (interrupt.interruptType) {
        case VideoInterrupt:
            DisableVideoInterrupt();
            break;
        case CMDInterrupt:
            DisableCMDInterrupt();
            break;
    }
}
