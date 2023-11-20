#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <stdint.h>
#include <sprite_sizes.h>
uint32_t CreateControlSprite(uint32_t, uint32_t);
uint32_t ControlSprite(uint32_t, uint32_t,int);

int createObject(SpriteSize size,int palette, int x, int y, int z, int dataId);
void controlObject(SpriteSize size,int palette, int x, int y, int z, int dataId, int index);


static uint32_t setControl(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index);
#endif // GRAPHIC_H