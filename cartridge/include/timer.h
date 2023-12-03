#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>
#include <stdlib.h>

uint32_t GetTicks(void);

/**
 * @brief Starts the timer.
 */
void startTimer();

/**
 * @brief Ends the timer.
 */
void endTimer();

/**
 * @brief Returns the elapsed tick.
 * @return The elapsed tick.
 */
uint32_t timeElapsed();

/**
 * @brief Resets the timer.
 */
void resetTimer();

/**
 * @brief Get the current tick value.
 *
 * This function returns the current tick value of the timer.
 *
 * @return The current tick value.
 */
uint32_t getCurrentTick();

#endif // TIMER_H