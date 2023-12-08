#ifndef MEMORY_H
#define MEMORY_H
#include <stdlib.h>

char* AllocateMemory(int size);
void FreeMemory(char* memory);

#endif // MEMORY_H