#include "api.h"

uint32_t getTicks(void) { return SystemCall(0, 0, 0, 0, 0, 1); }
uint32_t getController(void) { return SystemCall(0, 0, 0, 0, 0, 2); }

uint32_t getTimeStart(void) { return SystemCall(0, 0, 0, 0, 0, 19); };
uint32_t getTimeEnd(uint64_t start) {
    return SystemCall(start, 0, 0, 0, 0, 20);
};
uint32_t enableTextMode(void) { return SystemCall(0, 0, 0, 0, 0, 5); };
uint32_t enableGraphicsMode(void) { return SystemCall(0, 0, 0, 0, 0, 6); };
uint32_t createSmallSprite(enum ShapeType shapeType) {
    return SystemCall(shapeType, 0, 0, 0, 0, 7);
};
uint32_t setSmallPalette(uint32_t index, uint32_t color) {
    return SystemCall(index, color, 0, 0, 0, 8);
};
uint32_t setSmallControl(uint32_t index, uint32_t palette, uint32_t x,
                         uint32_t y, uint32_t z) {
    return SystemCall(index, palette, x, y, z, 9);
};

uint32_t createMediumSprite(enum ShapeType shapeType) {
    return SystemCall(shapeType, 0, 0, 0, 0, 10);
};
uint32_t setMediumPalette(uint32_t index, uint32_t color) {
    return SystemCall(index, color, 0, 0, 0, 11);
};
uint32_t setMediumControl(uint32_t index, uint32_t palette, uint32_t x,
                          uint32_t y, uint32_t z) {
    return SystemCall(index, palette, x, y, z, 12);
};

uint32_t createLargeSprite(enum ShapeType shapeType) {
    return SystemCall(shapeType, 0, 0, 0, 0, 13);
};
uint32_t setLargePalette(uint32_t index, uint32_t color) {
    return SystemCall(index, color, 0, 0, 0, 14);
};
uint32_t setLargeControl(uint32_t index, uint32_t palette, uint32_t x,
                         uint32_t y, uint32_t z) {
    return SystemCall(index, palette, x, y, z, 15);
};

uint32_t createBackground() { return SystemCall(0, 0, 0, 0, 0, 16); };
uint32_t setBackgroundPalette(uint32_t index, uint32_t color) {
    return SystemCall(index, color, 0, 0, 0, 17);
};
uint32_t setBackgroundControl(uint32_t index, uint32_t palette, uint32_t x,
                              uint32_t y, uint32_t z) {
    return SystemCall(index, palette, x, y, z, 18);
};
uint32_t getCMDInterruptCount() { return SystemCall(0, 0, 0, 0, 0, 22); };
uint32_t getVideoInterruptCount() { return SystemCall(0, 0, 0, 0, 0, 23); };
uint32_t getSmallSpriteCount() { return SystemCall(0, 0, 0, 0, 0, 24); };
uint32_t getMediumSpriteCount() { return SystemCall(0, 0, 0, 0, 0, 25); };
uint32_t getLargeSpriteCount() { return SystemCall(0, 0, 0, 0, 0, 26); };
