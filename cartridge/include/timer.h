#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>
#include <stdlib.h>

uint32_t StartTimer(void);
uint32_t EndTimer(void);
uint32_t TimeElpased(void);
uint32_t ResetTimer(void);
uint32_t GetCurrentTime(void);
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
 * @brief Returns the elapsed time in milliseconds.
 * @return The elapsed time in milliseconds.
 */
uint32_t timeElapsed();

/**
 * @brief Resets the timer.
 */
void resetTimer();

uint32_t getCurrentTime();

#endif // TIMER_H