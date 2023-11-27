#ifndef GROUP2_H
#define GROUP2_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// Screen dimensions in characters
#define SCREEN_COLS 64
#define SCREEN_ROWS 36

// Event API
typedef enum
{
    DirectionPad,
    ToggleButtons,
} ControllerType;

typedef enum
{
    DirectionUp,
    DirectionDown,
    DirectionLeft,
    DirectionRight
} EventType;

typedef struct
{
    ControllerType controllerType;
} Controller;

typedef struct
{
    EventType direction;
} Event;

// Event API End

// Graphic API
typedef enum
{
    GRAPHICS_MODE = 1,
    TEXT_MODE = 0
} DisplayMode;

struct windowSize
{
    int width;
    int height;
};

typedef enum
{
    LARGE_SPRITE = 0,
    MEDIUM_SPRITE = 1,
    SMALL_SPRITE = 2
} SpriteSizeCategory;

typedef enum
{
    LARGE_SPRITE_SIZE = 64,  // Large sprites are 64 x 64 pixels
    MEDIUM_SPRITE_SIZE = 32, // Medium sprites are 32 x 32 pixels
    SMALL_SPRITE_SIZE = 16   // Small sprites are up to 16 x 16 pixels
} SpriteSize;

typedef enum
{
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

typedef enum
{
    FULLY_OPAQUE = 0,     // Palette 0
    SEMI_OPAQUE_60 = 1,   // Palette 1
    SEMI_OPAQUE_30 = 2,   // Palette 2
    FULLY_TRANSPARENT = 3 // Palette 3
} OpacityLevel;

// Declaration only, no memory allocation
extern volatile char *VIDEO_MEMORY;

// Graphic API End

// System calls

// Event API
uint32_t checkControllerStatus(void);
uint32_t DirectionPadUp(void);
uint32_t DirectionPadDown(void);
uint32_t DirectionPadRight(void);
uint32_t DirectionPadLeft(void);
uint32_t ToggleButtonsUp(void);
uint32_t ToggleButtonsRight(void);
uint32_t ToggleButtonsLeft(void);
uint32_t ToggleButtonsDown(void);

uint32_t EnableCMDInterrupt(void);
uint32_t CMDInterrupted(void);
uint32_t DisableCMDInterrupt(void);

// Event API End

// Graphic API
uint32_t CreateControlSprite(uint32_t, uint32_t);
uint32_t ControlSprite(uint32_t, uint32_t, int);
uint32_t FreeControlSprite(uint32_t, uint32_t);
uint32_t ChangeMode(uint32_t);

uint32_t CreateSprite(uint32_t, uint8_t *, uint32_t);
uint32_t FreeSprite(uint32_t, uint32_t);
uint32_t ClearTextData();
uint32_t ShowTextToLine(const char *, uint32_t);
// Graphic API End

// Thread API
uint32_t InitThread();
uint32_t SwitchThread();
// Thread API End

// Timer API
uint32_t StartTimer(void);
uint32_t EndTimer(void);
uint32_t TimeElpased(void);
uint32_t ResetTimer(void);
uint32_t GetCurrentTime(void);
// Timer API End

// Event API
/**
 *  Checks if a controller event has been triggered.
 *
 * @return true if a controller event has been triggered, false otherwise.
 */
bool controllerEventTriggered();

/**
 *  Checks if a specific direction trigger has been activated for a given controller type and event type.
 *
 * @param controllerType The type of controller.
 * @param eventType The type of event.
 * @return true if the direction trigger has been activated, false otherwise.
 */
bool checkDirectionTrigger(ControllerType controllerType, EventType eventType);

/**
 *  Enables the interrupt for the CMD button.
 */
void enableCMDInterrupt();

/**
 * Checks if the CMD button is currently pressed.
 *
 * @return true if the CMD button is pressed, false otherwise.
 */
bool CMDPressed();

/**
 *  Disables the interrupt for the CMD button.
 */
void disableCMDInterrupt();
// Event API End

// Graphic API
/**
 * Creates a new object with the specified size, palette, position, and data ID.
 *
 * @param size The size of the canvas.
 * @param palette The palette index to use for the sprite.
 * @param x The x-coordinate of the object's position.
 * @param y The y-coordinate of the object's position.
 * @param z The z-coordinate of the object's position.
 * @param dataId The ID of the canvas's data.
 * @return The ID of the created object.
 */
int createObject(SpriteSize size, int palette, int x, int y, int z, int dataId);

/**
 * Controls an existing object with the specified size, palette, position, data ID, and index.
 *
 * @param size The size of the canvas.
 * @param palette The palette index to use for the canvas.
 * @param x The x-coordinate of the object's position.
 * @param y The y-coordinate of the object's position.
 * @param z The z-coordinate of the object's position.
 * @param dataId The ID of the canvas's data.
 * @param index The index of the object to control.
 */
void controlObject(SpriteSize size, int palette, int x, int y, int z, int dataId, int index);

/**
 * Frees an object with the specified size and ID.
 *
 * @param size The size of the sprite.
 * @param objectId The ID of the object to free.
 * @return 0 if successful, -1 otherwise.
 */
int freeObject(SpriteSize size, int objectId);

/**
 * Creates a new canvas with the specified size and buffer.
 *
 * @param size The size of the canvas.
 * @param buffer The buffer to use for the canvas.
 * @param bufferSize The size of the buffer.
 * @return The ID of the created canvas.
 */
int createCanvas(SpriteSize size, uint8_t *buffer, uint32_t bufferSize);

/**
 * Frees a canvas with the specified size and ID.
 *
 * @param size The size of the canvas.
 * @param canvasId The ID of the canvas to free.
 * @return 0 if successful, -1 otherwise.
 */
int freeCanvas(SpriteSize size, int canvasId);

/**
 * Sets the display mode.
 *
 * @param mode The display mode to set.
 */
void displayMode(DisplayMode mode);

/**
 * Retrieves the size of the window.
 *
 * @return The size of the window.
 */
struct windowSize getWindowSize();

/**
 * Clears the text data.
 */
void clearTextData();

/**
 * Shows text on a specific line of the display.
 *
 * @param text The text to show.
 * @param line The line number to show the text on.
 */
void showTextToLine(const char *text, int line);

/**
 * Sets the properties of an object with the specified size, palette, position, and index.
 *
 * @param size The size of the sprite.
 * @param palette The palette index to use for the sprite.
 * @param x The x-coordinate of the sprite's position.
 * @param y The y-coordinate of the sprite's position.
 * @param z The z-coordinate of the sprite's position.
 * @param index The index of the object to set.
 * @return The ID of the object.
 */
static uint32_t setObject(SpriteSize size, uint8_t palette, int16_t x, int16_t y, uint8_t z, uint8_t index);
// Graphic API End

// Memory API
/**
 *  Allocates a block of memory of the specified size.
 *
 * @param size The size of the memory block to allocate.
 * @return A pointer to the allocated memory block, or nullptr if the allocation fails.
 */
char *AllocateMemory(int size);

/**
 *  Frees the memory allocated by the given pointer.
 *
 * @param memory Pointer to the memory to be freed.
 */
void FreeMemory(char *memory);
// Memory API End

// Thread API
/**
 *  Initializes a thread.
 *
 * @return The thread ID.
 */
uint32_t initThread();

/**
 *  Switches the execution context to a different thread.
 *
 * This function is responsible for switching the execution context from the current thread
 * to a different thread.
 */
void switchThread();
// Thread API End

// Timer API
/**
 *  Starts the timer.
 */
void startTimer();

/**
 *  Ends the timer.
 */
void endTimer();

/**
 *  Returns the elapsed time.
 * @return The elapsed time.
 */
uint32_t timeElapsed();

/**
 *  Resets the timer.
 */
void resetTimer();

/**
 * @brief Get the current system time.
 *
 * @return The current system time.
 */
uint32_t getCurrentTime();
// Timer API End

#endif // GROUP2_H