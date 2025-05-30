#ifndef SPRITE_H
#define SPRITE_H
#include "../io/video/video.h"

// Sprite
typedef struct Sprite{
    uint16_t height;
    uint16_t width;
    uint32_t *colors;
} Sprite;

/**
 * @brief Loads a sprite from an XPM map
 * @param xpm The XPM image map to load
 * @return A pointer to the Sprite object
 */
Sprite *loadSprite(xpm_map_t xpm);

/**
 * @brief Draws the sprite on the screen 
 * @param sprite A pointer to the sprite 
 * @param x The x-coordinate 
 * @param y The y-coordinate
 * @return 0 on success, non-zero otherwise
 */
int drawSprite(Sprite *sprite, int x, int y);

/**
 * @brief Gets the color of the pixel 
 * @param sprite Pointer to the Sprite struct
 * @param x The x-coordinate within the sprite
 * @param y The y-coordinate within the sprite
 * @return uint32_t Color of the pixel 
 */
uint32_t getPixelColor(Sprite* sprite, uint16_t x, uint16_t y);

/**
 * @brief Loads all digit sprites (0-9)
 * @return Array of Sprite pointers for digits 0-9
 */
Sprite **loadDigitSprites();

/**
 * @brief Draws a number on the screen
 * @param digits Array of digit sprites (0-9)
 * @param number The number to draw
 * @param x The x-coordinate for the leftmost digit
 * @param y The y-coordinate for the digits
 * @return 0 on success, non-zero otherwise
 */
int drawNumber(Sprite **digits, int number, int x, int y);

/**
 * @brief Frees memory associated with the Sprite.
 * @param sprite A pointer to the Sprite object.
 */
void destroySprite(Sprite *sprite);

#endif

