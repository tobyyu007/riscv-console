# include "group2.h"

int timerStart = 0;
int timerEnd = 0;

void startTimer()
{
    timerStart = getTicks();
}

void endTimer()
{
    timerEnd = getTicks();
}

uint32_t timeElapsed()
{
    if (timerEnd == 0) {
        return (getTicks()-timerStart);
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

uint32_t getCurrentTick(){
    return getTicks();
}