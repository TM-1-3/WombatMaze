#ifndef WOMBAT_H
#define WOMBAT_H
#include <lcom/lcf.h>
#include "../io/video/video.h"
#include "../display/sprite.h"
#include "maze.h"
#include "../io/KBC/i8042.h"

typedef struct {
    uint16_t x;
    uint16_t y;
    Sprite *wombatSprite;
} Wombat;

/**
 * @brief Loads a wombat object with a sprite
 * @param x The initial x-coordinate 
 * @param y The initial y-coordinate 
 * @param xpm The XPM map used to load the wombat's sprite
 * @return A pointer to the Wombat structure
 */
Wombat* loadWombat(uint16_t x, uint16_t y, Sprite* sprite);

/**
 * @brief Gets the x-coordinate of the wombat
 * @param wombat A pointer to the wombat object
 * @return The x-coordinate of the wombat
 */
uint16_t getWombatX(Wombat* wombat);


/**
 * @brief Sets the x-coordinate of the wombat
 * @param wombat A pointer to the wombat object
 * @param x The new x-coordinate of the wombat
 */
void setWombatX(Wombat* wombat, uint16_t x);

/**
 * @brief Gets the y-coordinate of the wombat
 * @param wombat A pointer to the wombat object
 * @return The y-coordinate of the wombat
 */
uint16_t getWombatY(Wombat* wombat);

/**
 * @brief Sets the y-coordinate of the wombat
 * @param wombat A pointer to the wombat object
 * @param y The new y-coordinate of the wombat
 */
void setWombatY(Wombat* wombat, uint16_t y);

/**
 * @brief Draws the wombat on the screen
 * @param wombat A pointer to the wombat object 
 * @return 0 on success, non-zero otherwise
 */
int drawWombat(Wombat* wombat);

/**
 * @brief Handles movement input for the wombat
 * @param scanCode The scan code from the keyboard input
 * @return A value representing the movement direction 
 */
int moveHandler(uint8_t scanCode);

/**
 * @brief Moves the wombat in a specified direction
 * @param wombat A pointer to the wombat object
 * @param moveDirection The direction to move the wombat 
 */
void moveWombat(Wombat* wombat, int moveDirection, Maze* maze);

#endif
