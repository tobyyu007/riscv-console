#ifndef EVENT_H
#define EVENT_H
#include <stdint.h>
#include <stdbool.h>

#include "api.h"

#define INTERRUPT_ENABLE_REGISTER (*((volatile uint32_t *)0x40000000))
#define INTERRUPT_PENDING_REGISTER (*((volatile uint32_t *)0x40000004))
#define VIE_BIT 1
#define CMIE_BIT 2

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

/**
 * Enables the specified interrupt type.
 *
 * @param interruptType The type of interrupt to enable.
 */
void enableInterrupt(InterruptType interruptType);

/**
 * Checks if the specified interrupt type has been triggered.
 *
 * @param interruptType The type of interrupt to check.
 * @return True if the interrupt has been triggered, false otherwise.
 */
bool checkInterruptTrigger(InterruptType interruptType);

/**
 * @brief Clears the trigger for the specified interrupt type.
 *
 * This function clears the trigger for the specified interrupt type.
 *
 * @param interruptType The type of interrupt to clear the trigger for.
 */
void clearInterruptTrigger(InterruptType interruptType);

/**
 * Disables the specified interrupt type.
 *
 * @param interruptType The type of interrupt to disable.
 */
void disableInterrupt(InterruptType interruptType);

#endif // EVENT_H
