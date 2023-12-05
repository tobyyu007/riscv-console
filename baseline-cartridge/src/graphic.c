#include <graphic.h>
#include <sprite_sizes.h>
#include <stdio.h>
#include <string.h>

int createBackgroundObject(BackgroundType type,int palette, int x, int y, int z, int canvasId){
    uint32_t Object = setBackgroundObject(type,palette, x, y, z, canvasId);
    int objectId = CreateBackgroundObject(type, Object);
    return objectId;
}

void controlBackgroundObject(BackgroundType type,int palette, int x, int y, int z, int canvasId, int objectId){
    uint32_t Object = setBackgroundObject(type, palette, x, y, z, canvasId);
    int sucessControl = ControlBackgroundObject(type, Object, objectId);
}

int freeBackgroundObject(BackgroundType type, int objectId){
    int sucessFree = FreeBackgroundObject(type, objectId);
    return sucessFree;
}

static uint32_t setBackgroundObject(BackgroundType type, uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index){
    uint32_t y_offset = 0;
    uint32_t x_offset = 0;
    uint32_t mode = 0;
    uint32_t index_position = 0;
    switch (type) {
        case BACKGROUND_PIXEL:
            y_offset = 288;
            x_offset = 512;
            mode = 0;
            index_position = 29;
            break;
        case BACKGROUND_TILE:
            y_offset = 288;
            x_offset = 512;
            mode = 1;
            index_position = 28;
            break;
        default:
            return -1; // Invalid sprite size
    }
    return (((uint32_t)mode)<<31) | (((uint32_t)index)<<index_position) | (((uint32_t)z)<<22) | (((uint32_t)y+y_offset)<<12) | (((uint32_t)x+x_offset)<<2) | (palette & 0x3);
}

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

int createBackgroundCanvas(BackgroundType type, uint8_t* buffer, uint32_t bufferSize){
    int backgroundId = CreateBackgroundCanvas(type, buffer, bufferSize);
    return backgroundId;
}

int createBackgroundTileEntry(int tileIndex, int entryIndex, uint8_t* buffer, uint32_t bufferSize){
    int sucessCreate = CreateBackgroundTileEntry(tileIndex, entryIndex, buffer, bufferSize);
    return sucessCreate;
}

int freeBackgroundCanvas(BackgroundType type, int backgroundIndex){
    int sucessFree = FreeBackgroundCanvas(type, backgroundIndex);
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

// getWindowSize() is a function that returns the size of the window in pixels

struct windowSize getWindowSize() {
    struct windowSize size;
    size.width = 512;
    size.height = 288;
    return size;
}


void clearTextData() {
    ClearTextData();
}

void showTextToLine(const char* text, int line) {
    ShowTextToLine(text, line);
}