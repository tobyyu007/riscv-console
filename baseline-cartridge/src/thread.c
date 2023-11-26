# include "group2.h"


uint32_t initThread(){
    return InitThread();
}


void switchThread(uint32_t *mainThread, uint32_t *otherThread){
    SwitchThread(&mainThread,otherThread);
}