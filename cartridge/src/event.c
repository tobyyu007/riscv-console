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


void enableCMDInterrupt(){
    EnableCMDInterrupt();
}


bool CMDPressed(){
    return CMDInterrupted();
}


void disableCMDInterrupt(){
    DisableCMDInterrupt();
}
