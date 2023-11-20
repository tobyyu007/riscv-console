#include <graphic.h>
uint32_t Control;
int createObject(SpriteSize size,int palette, int x, int y, int z, int dataId){
    Control = setControl(palette, x, y, z, dataId);
    int objectId = CreateControlSprite(size, Control);
    return objectId;
}

void controlObject(SpriteSize size,int palette, int x, int y, int z, int dataId, int index){
    Control = setControl(palette, x, y, z, dataId);
    int sucessControl = ControlSprite(size, Control, index);
}

static uint32_t setControl(uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index){
    return (((uint32_t)index)<<24) | (((uint32_t)z)<<21) | (((uint32_t)y+32)<<12) | (((uint32_t)x+32)<<2) | (palette & 0x3);
}
