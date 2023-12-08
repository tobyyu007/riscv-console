#ifndef COLORS_INDEX_H
#define COLORS_INDEX_H

typedef enum {
    NO_COLOR = 0,
    ALICE_BLUE = 1, 
    AMARANTH, 
    AMBER, 
    AMETHYST, 
    APPLE_GREEN, 
    APPLE_RED, 
    APRICOT, 
    AQUAMARINE, 
    AZURE, 
    BABY_BLUE, 
    BEIGE, 
    BRICK_RED, 
    BLACK, 
    BLUE, 
    BLUE_GREEN, 
    BLUE_VIOLET, 
    BLUSH, 
    BRONZE, 
    BROWN, 
    BURGUNDY, 
    BYZANTIUM, 
    CARMINE, 
    CERISE, 
    CERULEAN, 
    CHAMPAGNE, 
    CHARTREUSE_GREEN, 
    CHOCOLATE, 
    COBALT_BLUE, 
    COFFEE, 
    COPPER, 
    CORAL, 
    CRIMSON, 
    CYAN, 
    DESERT_SAND, 
    ELECTRIC_BLUE, 
    EMERALD, 
    ERIN, 
    GOLD, 
    GRAY, 
    GREEN, 
    HARLEQUIN, 
    INDIGO, 
    IVORY, 
    JADE, 
    JUNGLE_GREEN, 
    LAVENDER, 
    LEMON, 
    LILAC, 
    LIME, 
    MAGENTA, 
    MAGENTA_ROSE, 
    MAROON, 
    MAUVE, 
    NAVY_BLUE, 
    OCHRE, 
    OLIVE, 
    ORANGE, 
    ORANGE_RED, 
    ORCHID, 
    PEACH, 
    PEAR, 
    PERIWINKLE, 
    PERSIAN_BLUE, 
    PINK, 
    PLUM, 
    PRUSSIAN_BLUE, 
    PUCE, 
    PURPLE, 
    RASPBERRY, 
    RED, 
    RED_VIOLET, 
    ROSE, 
    RUBY, 
    SALMON, 
    SANGRIA, 
    SAPPHIRE, 
    SCARLET, 
    SILVER, 
    SLATE_GRAY, 
    SPRING_BUD, 
    SPRING_GREEN, 
    TAN, 
    TAUPE, 
    TEAL, 
    TURQUOISE, 
    ULTRAMARINE, 
    VIOLET, 
    VIRIDIAN, 
    WHITE, 
    YELLOW
} ColorIndex;

typedef enum {
    FULLY_OPAQUE = 0,     // Palette 0
    SEMI_OPAQUE_60 = 1,   // Palette 1
    SEMI_OPAQUE_30 = 2,   // Palette 2
    FULLY_TRANSPARENT = 3 // Palette 3
} OpacityLevel;

typedef struct {
    uint32_t palette_number;
    uint32_t color;
    uint32_t entry_number;
} PaletteParams;

PaletteParams smallParams[] = {
    {0, 0x00000000, NO_COLOR},
    {0, 0xFFFF6600, ORANGE},
};

PaletteParams largeParams[] = {
    {0, 0x00000000, NO_COLOR},
    {0, 0xFFFF6600, ORANGE},
    {0, 0xFFFFFFFF, WHITE},
    {0, 0xFFFFFF00, YELLOW},
    {3, 0x00000000, NO_COLOR},
    {3, 0x00000000, ORANGE},
    {3, 0x00000000, WHITE},
    {3, 0x00000000, YELLOW}
};

PaletteParams backgroundParams[] = {
    {0, 0x00000000, BLACK},
    {1, 0xFFFF6600, RASPBERRY},
};
#endif 
