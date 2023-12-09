#ifndef EVENT_H
#define EVENT_H
#include <stdint.h>
#include <stdbool.h>

#include "api.h"

typedef enum
{
    DirectionPad,
    ToggleButtons,
} ControllerType;

typedef enum
{
    DirectionUp,
    DirectionDown,
    DirectionLeft,
    DirectionRight
} EventType;

typedef struct
{
    ControllerType controllerType;
} Controller;

typedef struct
{
    EventType direction;
} Event;

typedef enum
{
    VideoInterrupt,
    CMDInterrupt
} InterruptType;

typedef struct
{
    InterruptType interruptType;
} Interrupt;


/**
 * @brief Checks if a controller event has been triggered.
 *
 * @return true if a controller event has been triggered, false otherwise.
 */
bool controllerEventTriggered();

/**
 * @brief Checks if a specific direction trigger has been activated for a given controller type and event type.
 *
 * @param controllerType The type of controller.
 * @param eventType The type of event.
 * @return true if the direction trigger has been activated, false otherwise.
 */
bool checkDirectionTrigger(ControllerType controllerType, EventType eventType);

int interruptCount(InterruptType interruptType);

#endif // EVENT_H
