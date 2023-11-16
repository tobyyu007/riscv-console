#ifndef EVENT_H
#define EVENT_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
    DirectionPad,
    ToggleButtons,
} ControllerType;

typedef enum {
    DirectionUp,
    DirectionDown,
    DirectionLeft,
    DirectionRight
} EventType;

typedef struct {
    ControllerType controllerType;
} Controller;

typedef struct {
    EventType direction;
} Event;

uint32_t checkControllerStatus(void);
uint32_t DirectionPadUp(void);
uint32_t DirectionPadDown(void);
uint32_t DirectionPadRight(void);
uint32_t DirectionPadLeft(void);
uint32_t ToggleButtonsUp(void);
uint32_t ToggleButtonsRight(void);
uint32_t ToggleButtonsLeft(void);
uint32_t ToggleButtonsDown(void);

bool eventTriggered() {  // Check if event is triggered
    return checkControllerStatus();
}


bool checkTriggerDirection(ControllerType controllerType, EventType eventType) {  // Check if the given controller's button is triggered
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
                case DirectionDown:  // i
                    return ToggleButtonsDown();
                case DirectionRight:  // j
                    return ToggleButtonsRight();
                case DirectionLeft:  // k
                    return ToggleButtonsLeft();
            }
            break;
    }

    return false;
}

#endif // EVENT_H
