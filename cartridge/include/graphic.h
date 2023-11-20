#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <stdint.h>
#include <sprite_sizes.h>
uint32_t CreateControlSprite(uint32_t, uint32_t);
uint32_t ControlSprite(uint32_t, uint32_t,int);
uint32_t FreeControlSprite(uint32_t, uint32_t);

uint32_t CreateSprite(uint32_t, uint8_t*, uint32_t);
uint32_t FreeSprite(uint32_t, uint32_t);

int createObject(SpriteSize size,int palette, int x, int y, int z, int dataId);
void controlObject(SpriteSize size,int palette, int x, int y, int z, int dataId, int index);
int freeObject(SpriteSize size, int objectId);
int createCanvas(SpriteSize size, uint8_t* buffer, uint32_t bufferSize);
int freeCanvas(SpriteSize size, int canvasId);

static uint32_t setObject(SpriteSize size, uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index);
#endif // GRAPHIC_H