#include "timer.h"

int timerStart = 0;
int timerEnd = 0;

void startTimer()
{
    timerStart = GetTicks();
}

void endTimer()
{
    timerEnd = GetTicks();
}

uint32_t timeElapsed()
{
    if (timerEnd == 0) {
        return (GetTicks()-timerStart);
    }
    else{
        return (timerEnd-timerStart);
    }
}

void resetTimer()
{
    timerStart = 0;
    timerEnd = 0;
}

uint32_t getCurrentTime()
{
    return GetCurrentTime();
}