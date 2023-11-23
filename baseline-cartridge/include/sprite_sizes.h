#ifndef SPRITE_SIZES_H
#define SPRITE_SIZES_H

typedef enum {
    LARGE_SPRITE = 0,
    MEDIUM_SPRITE = 1,
    SMALL_SPRITE = 2
} SpriteSizeCategory;

typedef enum {
    LARGE_SPRITE_SIZE = 64,  // Large sprites are 64 x 64 pixels
    MEDIUM_SPRITE_SIZE = 32, // Medium sprites are 32 x 32 pixels
    SMALL_SPRITE_SIZE = 16   // Small sprites are up to 16 x 16 pixels
} SpriteSize;
#endif 