#include "api.h"

uint32_t GetTicks(void) { return SystemCall(0, 0, 0, 0, 0, 1); }
uint32_t GetController(void) { return SystemCall(0, 0, 0, 0, 0, 2); }

uint32_t GetTimeStart(void) { return SystemCall(0, 0, 0, 0, 0, 19); };
uint32_t GetTimeEnd(uint64_t start) {
    return SystemCall(start, 0, 0, 0, 0, 20);
};
int EnableTextMode(void) { return SystemCall(0, 0, 0, 0, 0, 5); };
int EnableGraphicsMode(void) { return SystemCall(0, 0, 0, 0, 0, 6); };
int CreateSmallSprite(void) { return SystemCall(0, 0, 0, 0, 0, 7); };
int SetSmallPalette(uint32_t index, uint32_t color) {
    return SystemCall(index, color, 0, 0, 0, 8);
};
int SetSmallControl(uint32_t index, uint32_t palette, uint32_t x, uint32_t y,
                    uint32_t z) {
    return SystemCall(index, palette, x, y, z, 9);
};

int CreateMediumSprite() { return SystemCall(0, 0, 0, 0, 0, 10); };
int SetMediumPalette(uint32_t index, uint32_t color) {
    return SystemCall(index, color, 0, 0, 0, 11);
};
int SetMediumControl(uint32_t index, uint32_t palette, uint32_t x, uint32_t y,
                     uint32_t z) {
    return SystemCall(index, palette, x, y, z, 12);
};

int CreateLargeSprite(void) { return SystemCall(0, 0, 0, 0, 0, 13); };
int SetLargePalette(uint32_t index, uint32_t color) {
    return SystemCall(index, color, 0, 0, 0, 14);
};
int SetLargeControl(uint32_t index, uint32_t palette, uint32_t x, uint32_t y,
                    uint32_t z) {
    return SystemCall(index, palette, x, y, z, 15);
};

int SetBackgroundData(uint32_t x, uint32_t y) {
    return SystemCall(x, y, 0, 0, 0, 16);
};
int SetBackgroundPalette(uint32_t index, uint32_t color) {
    return SystemCall(index, color, 0, 0, 0, 17);
};
int SetBackgroundControl(uint32_t index, uint32_t palette, uint32_t x,
                         uint32_t y, uint32_t z) {
    return SystemCall(index, palette, x, y, z, 18);
};