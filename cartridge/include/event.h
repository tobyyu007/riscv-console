#ifndef EVENT_H
#define EVENT_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

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

uint32_t checkControllerStatus(void);
uint32_t DirectionPadUp(void);
uint32_t DirectionPadDown(void);
uint32_t DirectionPadRight(void);
uint32_t DirectionPadLeft(void);
uint32_t ToggleButtonsUp(void);
uint32_t ToggleButtonsRight(void);
uint32_t ToggleButtonsLeft(void);
uint32_t ToggleButtonsDown(void);

uint32_t EnableCMDInterrupt(void);
uint32_t CMDInterrupted(void);
uint32_t DisableCMDInterrupt(void);

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
 * @brief Enables the interrupt for the CMD button.
 */
void enableCMDInterrupt();

/**
 * @brief Checks if the CMD button is currently pressed.
 *
 * @return true if the CMD button is pressed, false otherwise.
 */
bool CMDPressed();

/**
 * @brief Disables the interrupt for the CMD button.
 */
void disableCMDInterrupt();

#endif // EVENT_H
