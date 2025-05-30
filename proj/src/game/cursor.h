#ifndef CURSOR_H
#define CURSOR_H
#include <lcom/lcf.h>
#include "../io/video/video.h"
#include "../display/sprite.h"

typedef struct {
    int x;
    int y;
    Sprite *cursorSprite;
} Cursor;

/**
 * @brief Loads a cursor at given (x, y) coordinates from an XPM map
 * @param x Horizontal position to place the cursor
 * @param y Vertical position to place the cursor
 * @param xpm The XPM image data for the cursor
 * @return Pointer to the created Cursor object
 */
Cursor* loadCursor(uint16_t x, uint16_t y, xpm_map_t xpm);

/**
 * @brief Gets the current X position of the cursor
 * @param cursor Pointer to the Cursor object
 * @return The X coordinate as an int
 */
int getCursorX(Cursor* cursor);

/**
 * @brief Sets the X position of the cursor
 * @param cursor Pointer to the Cursor object
 * @param x New X coordinate
 */
void setCursorX(Cursor* cursor, uint16_t x);

/**
 * @brief Gets the current Y position of the cursor
 * @param cursor Pointer to the Cursor object
 * @return The Y coordinate as an int
 */
int getCursorY(Cursor* cursor);

/**
 * @brief Sets the Y position of the cursor
 * @param cursor Pointer to the Cursor object
 * @param y New Y coordinate
 */
void setCursorY(Cursor* cursor, uint16_t y);

/**
 * @brief Draws the cursor on screen at its current position
 * @param cursor Pointer to the Cursor object
 * @return 0 on success, non-zero on failure
 */
int drawCursor(Cursor* cursor);

/**
 * @brief Frees memory associated with the Cursor and its Sprite.
 * @param cursor A pointer to the Cursor object.
 */
void destroyCursor(Cursor* cursor);


#endif


