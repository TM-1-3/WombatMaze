#ifndef DINGOE_H
#define DINGOE_H
#include <lcom/lcf.h>
#include "../io/video/video.h"
#include "../display/sprite.h"
#include "maze.h"
#include "wombat.h"
#include "../io/KBC/i8042.h"

typedef struct {
    uint16_t x;
    uint16_t y;
    Sprite *dingoeSprite;
} Dingoe;

/**
 * @brief Loads a dingoe object with a sprite
 * @param x The initial x-coordinate 
 * @param y The initial y-coordinate 
 * @param xpm The XPM map used to load the dingoe's sprite
 * @return A pointer to the Dingoe structure
 */
Dingoe* loadDingoe(uint16_t x, uint16_t y, Sprite* sprite);

/**
 * @brief Gets the x-coordinate of the dingoe
 * @param dingoe A pointer to the dingoe object
 * @return The x-coordinate of the dingoe
 */
uint16_t getDingoeX(Dingoe* dingoe);


/**
 * @brief Sets the x-coordinate of the dingoe
 * @param dingoe A pointer to the dingoe object
 * @param x The new x-coordinate of the dingoe
 */
void setDingoeX(Dingoe* dingoe, uint16_t x);

/**
 * @brief Gets the y-coordinate of the dingoe
 * @param dingoe A pointer to the dingoe object
 * @return The y-coordinate of the dingoe
 */
uint16_t getDingoeY(Dingoe* dingoe);

/**
 * @brief Sets the y-coordinate of the dingoe
 * @param dingoe A pointer to the dingoe object
 * @param y The new y-coordinate of the dingoe
 */
void setDingoeY(Dingoe* dingoe, uint16_t y);

/**
 * @brief Draws the dingoe on the screen
 * @param dingoe A pointer to the dingoe object 
 * @return 0 on success, non-zero otherwise
 */
int drawDingoe(Dingoe* dingoe);

/**
 * @brief Moves the dingoe in a specified direction
 * @param dingoe A pointer to the dingoe object
 * @param moveDirection The direction to move the dingoe 
 */
void moveDingoe(Dingoe* dingoe, int moveDirection, Maze* maze);

/**
 * @brief Checks if the dingoe can see the wombat
 * @param dingoe Pointer to the Dingoe object
 * @param wombat Pointer to the Wombat object
 * @param maze Pointer to the Maze object
 * @return true if it can, false otherwise
 */
int seeWombat(Dingoe* dingoe, Wombat* wombat, Maze* maze);

/**
 * @brief Checks collision with Wombat 
 * @param wombat Pointer to the Wombat
 * @param dingoe Pointer to the Dingoe
 * @return true if collide, false otherwise
 */
bool check_collision( Dingoe* dingoe, Wombat* womba);

#endif
