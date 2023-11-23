#ifndef THREAD_H
#define THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t InitThread();
uint32_t SwitchThread();

uint32_t initThread();
void switchThread();

#endif // THREAD_H