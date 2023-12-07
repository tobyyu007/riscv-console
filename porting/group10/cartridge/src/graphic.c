# include "graphic.h"

void displayMode(DisplayMode mode){
    if(mode == GRAPHICS_MODE){
        enableGraphicsMode();
    }
    else if (mode == TEXT_MODE){
        enableTextMode();
    }
}

struct windowSize getWindowSize() {
    struct windowSize size;
    size.width = 512;
    size.height = 288;
    return size;
}

// int createObject(SpriteSize size,int palette, int x, int y, int z, ShapeType shape){
//     switch (size) {
//         case LARGE_SPRITE:
//             break;
//         case MEDIUM_SPRITE:
//             break;
//         case SMALL_SPRITE:
//             break;
//         default:
//             return -1; // Invalid sprite size
//     }
//     uint32_t Object = setObject(size,palette, x, y, z, shape);
//     int objectId = CreateControlSprite(size, Object);
//     return objectId;
// }

// void controlObject(SpriteSize size,int palette, int x, int y, int z, int canvasId, int objectId){
//     uint32_t Object = setObject(size, palette, x, y, z, canvasId);
//     int sucessControl = ControlSprite(size, Object, objectId);
// }