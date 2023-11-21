#include <graphic.h>
#include <sprite_sizes.h>

int createObject(SpriteSize size,int palette, int x, int y, int z, int canvasId){
    uint32_t Object = setObject(size,palette, x, y, z, canvasId);
    int objectId = CreateControlSprite(size, Object);
    return objectId;
}

void controlObject(SpriteSize size,int palette, int x, int y, int z, int canvasId, int objectId){
    uint32_t Object = setObject(size, palette, x, y, z, canvasId);
    int sucessControl = ControlSprite(size, Object, objectId);
}

int freeObject(SpriteSize size, int objectId){
    int sucessFree = FreeControlSprite(size, objectId);
    return sucessFree;
}

static uint32_t setObject(SpriteSize size, uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index){
    uint32_t offset = 0;
    switch (size) {
        case LARGE_SPRITE:
            offset = 64;
            break;
        case MEDIUM_SPRITE:
            offset = 32;
            break;
        case SMALL_SPRITE:
            offset = 16;
            break;
        default:
            return -1; // Invalid sprite size
    }
    return (((uint32_t)index)<<24) | (((uint32_t)z)<<21) | (((uint32_t)y+offset)<<12) | (((uint32_t)x+offset)<<2) | (palette & 0x3);
}

int createCanvas(SpriteSize size, uint8_t* buffer, uint32_t bufferSize){
    int canvasId = CreateSprite(size, buffer, bufferSize);
    return canvasId;
}

int freeCanvas(SpriteSize size, int canvasId){
    int sucessFree = FreeSprite(size, canvasId);
    return sucessFree;
}

void displayMode(DisplayMode mode){
    if(mode == GRAPHICS_MODE){
        ChangeMode(1);
    }
    else if (mode == TEXT_MODE){
        ChangeMode(0);
    }
}