#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdint.h>
#include "../display/sprite.h"
#include "cursor.h"

// Obstacle structure definition
typedef struct {
    uint16_t x, y;          // Obstacle position coordinates
    uint8_t active;         // Flag indicating if obstacle is active (1) or removed (0)
    Sprite* obstacleSprite; // Pointer to obstacle's sprite
} Obstacle;

/**
 * @brief Creates a new obstacle instance
 * @param x X-coordinate position
 * @param y Y-coordinate position
 * @param xpm XPM map for the obstacle sprite
 * @return Pointer to created Obstacle, NULL on failure
 */
Obstacle* load_obstacle(uint16_t x, uint16_t y, xpm_map_t xpm);

/**
 * @brief Marks an obstacle for removal (sets as inactive)
 * @param obstacle Pointer to the obstacle to remove
 */
void remove_obstacle(Obstacle* obstacle);

/**
 * @brief Checks if an obstacle is active
 * @param obstacle Pointer to the obstacle to check
 * @return 1 if active, 0 if inactive or NULL
 */
uint8_t is_obstacle_active(Obstacle* obstacle);

/**
 * @brief Draws the obstacle on the screen if it is active
 * @param obstacle Pointer to the obstacle to be drawn
 * @return 0 on success, 1 on failure
 */
int draw_obstacle(Obstacle* obstacle);

/**
 * @brief Updates the obstacle state based on the cursor position and mouse click
 * If the cursor is over the obstacle and the left mouse button is pressed the obstacle is marked as inactive (removed)
 * @param obstacle Pointer to the obstacle to update
 * @param cursor Pointer to the cursor
 * @param pp Pointer to the mouse packet with the latest mouse data
 */
void update_obstacle_with_cursor(Obstacle* obstacle, Cursor* cursor, struct packet *pp);

/**
 * @brief Checks if the specified coordinates collide with a given obstacle.
 * @param obstacle A pointer to the obstacle to check
 * @param x The x-coordinate to check
 * @param y The y-coordinate to check
 * @return 1 if the coordinates collide with the obstacle, 0 otherwise
 */
int isObstacle(Obstacle* obstacle, uint16_t x, uint16_t y);


/**
 * @brief Checks if the specified coordinates collide with any obstacle in the array.
 * @param obstacles An array of pointers to obstacle objects
 * @param num_obstacles The number of obstacles in the array
 * @param x The x-coordinate to check
 * @param y The y-coordinate to check
 * @return 1 if any obstacle is present at the coordinates, 0 otherwise
 */
int isAnyObstacle(Obstacle* obstacles[], int num_obstacles, uint16_t x, uint16_t y);

#endif
