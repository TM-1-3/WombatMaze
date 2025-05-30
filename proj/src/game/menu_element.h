#ifndef MENU_ELEMENT_H
#define MENU_ELEMENT_H
#include <lcom/lcf.h>
#include "../io/video/video.h"
#include "../display/sprite.h"

typedef struct {
    uint16_t x;
    uint16_t y;
    Sprite *menuElementSprite;
} MenuElement;

/**
 * @brief Loads a menu element at specified (x, y) coordinates from an XPM map
 * @param x Horizontal position to place the menu element
 * @param y Vertical position to place the menu element
 * @param xpm The XPM image data for the menu element
 * @return Pointer to the created MenuElement object
 */
MenuElement* loadMenuElement(uint16_t x, uint16_t y, xpm_map_t xpm);

/**
 * @brief Gets the current X position of the menu element
 * @param menuElement Pointer to the MenuElement object
 * @return The X coordinate as an int
 */
int getMenuElementX(MenuElement* menuElement);

/**
 * @brief Sets the X position of the menu element
 * @param menuElement Pointer to the MenuElement object
 * @param x New X coordinate
 */
void setMenuElementX(MenuElement* menuElement, uint16_t x);

/**
 * @brief Gets the current Y position of the menu element
 * @param menuElement Pointer to the MenuElement object
 * @return The Y coordinate as an int
 */
int getMenuElementY(MenuElement* menuElement);

/**
 * @brief Sets the Y position of the menu element
 * @param menuElement Pointer to the MenuElement object
 * @param y New Y coordinate
 */
void setMenuElementY(MenuElement* menuElement, uint16_t y);

/**
 * @brief Draws the menu element on the screen at its current position
 * @param menuElement Pointer to the MenuElement object
 * @return 0 on success, non-zero on failure
 */
int drawMenuElement(MenuElement* menuElement);

#endif


